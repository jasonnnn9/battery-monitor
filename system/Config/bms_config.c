#include "main.h"
#include "bms_config.h"
#include "bms_control.h"
#include <string.h>
#include <stddef.h>

extern uint32_t __bms_config_start;
extern float volMax;
extern uint16_t currentMax;
extern uint8_t tempMax;

#define CONFIG_PAGE_SIZE 1024u
#define CONFIG_MAGIC 0x424d5301u
#define CONFIG_COMMIT 0xc0decafeu

typedef struct {
    uint32_t magic, revision;
    uint16_t low_mv, high_ma, high_c, reserved;
    uint32_t crc, commit;
} ConfigRecord;
_Static_assert(sizeof(ConfigRecord) == 24, "Configuration record layout changed");

uint32_t bmsConfigRevision;
static uint32_t lastRecordAddress;

static int record_valid(const ConfigRecord *record)
{
    return record->magic == CONFIG_MAGIC && record->commit == CONFIG_COMMIT &&
        record->reserved == 0 && bms_limits_valid(record->low_mv, record->high_ma, record->high_c) &&
        record->crc == bms_crc16((const char *)record, offsetof(ConfigRecord, crc));
}

void BmsConfig_Init(void)
{
    uint32_t base = (uint32_t)&__bms_config_start;
    ConfigRecord newest = {0};
    lastRecordAddress = 0;
    bmsConfigRevision = 0;
    for (unsigned page = 0; page < 2; ++page) {
        for (unsigned slot = 0; slot < CONFIG_PAGE_SIZE / sizeof(ConfigRecord); ++slot) {
            uint32_t address = base + page * CONFIG_PAGE_SIZE + slot * sizeof(ConfigRecord);
            ConfigRecord record;
            memcpy(&record, (const void *)address, sizeof(record));
            if (record_valid(&record) && (!lastRecordAddress ||
                (int32_t)(record.revision - newest.revision) > 0)) {
                newest = record;
                lastRecordAddress = address;
            }
        }
    }
    if (lastRecordAddress) {
        volMax = newest.low_mv / 1000.0f;
        currentMax = newest.high_ma;
        tempMax = (uint8_t)newest.high_c;
        bmsConfigRevision = newest.revision;
    }
}

static uint32_t find_blank_record(uint32_t page)
{
    for (unsigned slot = 0; slot < CONFIG_PAGE_SIZE / sizeof(ConfigRecord); ++slot) {
        uint32_t address = page + slot * sizeof(ConfigRecord);
        const uint32_t *words = (const uint32_t *)address;
        unsigned i = 0;
        while (i < sizeof(ConfigRecord) / sizeof(uint32_t) && words[i] == UINT32_MAX) ++i;
        if (i == sizeof(ConfigRecord) / sizeof(uint32_t)) return address;
    }
    return 0;
}

uint8_t BmsConfig_Apply(uint32_t low_mv, uint32_t high_ma, uint32_t high_c)
{
    if (!bms_limits_valid(low_mv, high_ma, high_c)) return BMS_ACK_RANGE;
    if (lastRecordAddress && (uint32_t)(volMax * 1000.0f + 0.5f) == low_mv &&
        currentMax == high_ma && tempMax == high_c) return BMS_ACK_APPLIED;
    uint32_t base = (uint32_t)&__bms_config_start;
    uint32_t page = lastRecordAddress ? lastRecordAddress & ~(CONFIG_PAGE_SIZE - 1u) : base;
    uint32_t address = find_blank_record(page);
    int erase = address == 0;
    if (erase) {
        // Erase only the other page; the last committed configuration stays intact.
        page = page == base ? base + CONFIG_PAGE_SIZE : base;
        address = page;
    }
    ConfigRecord record = {CONFIG_MAGIC, bmsConfigRevision + 1,
        (uint16_t)low_mv, (uint16_t)high_ma, (uint16_t)high_c, 0, 0, CONFIG_COMMIT};
    record.crc = bms_crc16((const char *)&record, offsetof(ConfigRecord, crc));
    if (HAL_FLASH_Unlock() != HAL_OK) return BMS_ACK_SAVE_ERROR;
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);
    HAL_StatusTypeDef result = HAL_OK;
    if (erase) {
        FLASH_EraseInitTypeDef operation = {0};
        uint32_t error = 0;
        operation.TypeErase = FLASH_TYPEERASE_PAGES;
        operation.PageAddress = page;
        operation.NbPages = 1;
        result = HAL_FLASHEx_Erase(&operation, &error);
    }
    for (unsigned i = 0; result == HAL_OK && i < sizeof(record) / sizeof(uint32_t); ++i) {
        uint32_t word;
        memcpy(&word, (const uint8_t *)&record + i * sizeof(word), sizeof(word));
        // The commit marker is programmed last, so incomplete records are ignored.
        result = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address + i * sizeof(word), word);
    }
    HAL_FLASH_Lock();
    if (result != HAL_OK || memcmp((const void *)address, &record, sizeof(record)) != 0)
        return BMS_ACK_SAVE_ERROR;
    volMax = low_mv / 1000.0f;
    currentMax = (uint16_t)high_ma;
    tempMax = (uint8_t)high_c;
    bmsConfigRevision = record.revision;
    lastRecordAddress = address;
    return BMS_ACK_APPLIED;
}
