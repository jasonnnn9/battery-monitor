#include "ds18b20.h"

enum {
    CONVERSION_MS = 750,
    CONVERSION_TIMEOUT_MS = 800,
    POLL_INTERVAL_MS = 25,
    RETRY_INTERVAL_MS = 100
};

static uint32_t startedAt, lastActionAt;
static uint8_t converting, valid;
static int16_t lastTemperature;

static uint8_t ResetAndDetect(void)
{
    DS18B20_DQ_OUT(Lo);
    delay_us(500);
    uint32_t primask = __get_PRIMASK();
    __disable_irq();
    DS18B20_DQ_OUT(Hi);
    delay_us(70);
    uint8_t present = DS18B20_DQ_IN == GPIO_PIN_RESET;
    __set_PRIMASK(primask);
    delay_us(410);
    return present && DS18B20_DQ_IN == GPIO_PIN_SET;
}

static uint8_t ReadBit(void)
{
    uint32_t primask = __get_PRIMASK();
    __disable_irq();
    DS18B20_DQ_OUT(Lo);
    delay_us(2);
    DS18B20_DQ_OUT(Hi);
    delay_us(8);
    uint8_t value = DS18B20_DQ_IN == GPIO_PIN_SET;
    __set_PRIMASK(primask);
    delay_us(55);
    return value;
}

static uint8_t ReadByte(void)
{
    uint8_t value = 0;
    for (unsigned bit = 0; bit < 8; ++bit)
        value |= ReadBit() << bit;
    return value;
}

static void WriteByte(uint8_t value)
{
    for (unsigned bit = 0; bit < 8; ++bit) {
        uint32_t primask = __get_PRIMASK();
        __disable_irq();
        DS18B20_DQ_OUT(Lo);
        delay_us((value & 1u) ? 2 : 60);
        DS18B20_DQ_OUT(Hi);
        __set_PRIMASK(primask);
        delay_us((value & 1u) ? 63 : 5);
        value >>= 1;
    }
}

static uint8_t ReadTemperature(int16_t *temperature)
{
    if (!ResetAndDetect())
        return 0;

    WriteByte(0xCC); // Skip ROM. The V9 bus has one sensor.
    WriteByte(0xBE); // Read Scratchpad.
    uint8_t data[9];
    uint8_t crc = 0;
    for (unsigned i = 0; i < sizeof(data); ++i) {
        data[i] = ReadByte();
        crc ^= data[i];
        for (unsigned bit = 0; bit < 8; ++bit)
            crc = (uint8_t)((crc >> 1) ^ ((crc & 1u) ? 0x8C : 0));
    }

    // Reject a line held low as well as bad CRC and out-of-range samples.
    if (crc || data[5] != 0xFF || data[7] != 0x10)
        return 0;

    int16_t raw = (int16_t)((data[1] << 8) | data[0]);
    if (raw < -55 * 16 || raw > 125 * 16)
        return 0;

    *temperature = (int16_t)(raw * 10 / 16);
    return 1;
}

void DS18B20_Init(void)
{
    GPIO_InitTypeDef gpio = {0};
    __HAL_RCC_GPIOA_CLK_ENABLE();
    DS18B20_DQ_OUT(Hi);
    gpio.Pin = BS18B20_GPIO_DATA;
    gpio.Mode = GPIO_MODE_OUTPUT_OD;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(BS18B20_GPIO_PORT, &gpio);
    lastActionAt = HAL_GetTick() - RETRY_INTERVAL_MS;
}

uint8_t DS18B20_Update(int16_t *temperature)
{
    uint32_t now = HAL_GetTick();

    if (!converting) {
        if (now - lastActionAt < RETRY_INTERVAL_MS)
            goto report;
        lastActionAt = now;
        if (!ResetAndDetect()) {
            valid = 0;
            goto report;
        }
        WriteByte(0xCC);
        WriteByte(0x44); // Convert T. The V9 sensor has a dedicated VCC pin.
        delay_ms(1); // Allow the probe to enter conversion before polling its busy bit.
        converting = !ReadBit(); // A conversion must report busy before it reports done.
        startedAt = HAL_GetTick();
        lastActionAt = startedAt;
        if (!converting)
            valid = 0;
    } else if (now - startedAt >= CONVERSION_MS && now - lastActionAt >= POLL_INTERVAL_MS) {
        lastActionAt = now;
        if (ReadBit()) {
            converting = 0;
            valid = ReadTemperature(&lastTemperature);
        } else if (now - startedAt >= CONVERSION_TIMEOUT_MS) {
            converting = 0;
            valid = 0;
        }
    }

report:
    if (valid)
        *temperature = lastTemperature;
    return valid;
}
