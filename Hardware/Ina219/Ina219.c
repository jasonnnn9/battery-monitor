#include "main.h"
#include "ina219.h"

IIC_HardWareTypedef iicina219;

static uint16_t ina219_configValue;
uint16_t ina219_calibrationValue;        
uint16_t ina219_currentDivider_mA;       
float ina219_powerMultiplier_mW;         


 





static bool INA219_ReadRegister(uint8_t address, uint8_t reg, uint16_t *value)
{
    uint8_t data[2];
    if (IIC_Read_Device_2Byte_Len(address, reg, sizeof data, data, iicina219) != 0)
        return false;
    *value = ((uint16_t)data[0] << 8) | data[1];
    return true;
}

uint16_t INA219_ReadDataForRegister_16Bits(uint8_t Bddr, uint8_t registerAddress)
{
    uint16_t value = 0;
    INA219_ReadRegister(Bddr, registerAddress, &value);
    return value;
}

 





void INA219_WriteDataToRegister_16Bits(uint8_t Bddr, uint8_t registerAddress, uint16_t Value)
{
    IIC_Write_Device_2Byte(Bddr, registerAddress, 2, Value, iicina219);
}

 




uint16_t INA219_Readwatt(uint8_t address)
{
    uint16_t current_mA = INA219_ReadCurrent_mA(address);
    if (current_mA == INA219_CURRENT_INVALID)
        return 0;
    return (uint32_t)INA219_ReadBusVoltage(address) * current_mA / 1000u;
}

 




//Array FV={{0},0};
uint16_t INA219_ReadBusVoltage(uint8_t Bddr)
{
    uint16_t result = INA219_ReadDataForRegister_16Bits(Bddr, INA219_REG_BUS_VOLTAGE);
 
     
    return ((result >> 3) * 4);
}

 




uint16_t INA219_ReadCurrentRaw(uint8_t Bddr)
{
    uint16_t result = INA219_ReadDataForRegister_16Bits(Bddr, INA219_REG_CURRENT);
    return (result);
}

 




static bool INA219_ReadCurrentMagnitude(uint8_t address, uint32_t *magnitude)
{
    uint16_t config, calibration, raw;
    if (!ina219_currentDivider_mA ||
        !INA219_ReadRegister(address, INA219_REG_CONFIG, &config) ||
        !INA219_ReadRegister(address, INA219_REG_CALIBRATION, &calibration))
        return false;

    if (config != ina219_configValue || calibration != ina219_calibrationValue)
    {
        /* Restore settings after a sensor brownout. Sample on the next refresh. */
        INA219_SetConfig(address, ina219_configValue);
        INA219_SetCalibration(address, ina219_calibrationValue);
        return false;
    }
    uint16_t bus;
    if (!INA219_ReadRegister(address, INA219_REG_BUS_VOLTAGE, &bus) ||
        (bus & 3u) != 2u || /* conversion ready, no math overflow */
        !INA219_ReadRegister(address, INA219_REG_CURRENT, &raw))
        return false;

    int32_t counts = (int16_t)raw;
    if (counts < 0)
        counts = -counts;
    *magnitude = (uint32_t)counts;
    return true;
}

uint16_t INA219_ReadCurrent_mA(uint8_t address)
{
    uint32_t counts;
    if (!INA219_ReadCurrentMagnitude(address, &counts))
        return INA219_CURRENT_INVALID;
    return (uint16_t)((counts + ina219_currentDivider_mA / 2u) / ina219_currentDivider_mA);
}

 




uint16_t INA219_ReadCurrent_uA(uint8_t address)
{
    uint32_t counts;
    if (!INA219_ReadCurrentMagnitude(address, &counts))
        return INA219_CURRENT_INVALID;
    uint32_t microamps = counts * 1000u / ina219_currentDivider_mA;
    /* The legacy 16-bit uA API reserves its largest value for errors. */
    return microamps >= UINT16_MAX ? UINT16_MAX - 1u : (uint16_t)microamps;
}

 




float INA219_ReadShuntVoltage_mV(uint8_t Bddr)
{
    uint16_t result = 0;
    float result_f = 0;
     

    result = INA219_ReadDataForRegister_16Bits(Bddr, INA219_REG_SHUNT_VOLTAGE);

    if (result > MAX_SHUNT_RANGE)
    {
        result = 65536 - MAX_SHUNT_RANGE;    
    }

     
    return result_f = (float)result / 100;
    __NOP();
}

 



void INA219_Reset(uint8_t Bddr)
{
    INA219_WriteDataToRegister_16Bits(Bddr, INA219_REG_CONFIG, INA219_CONFIG_RESET);
    HAL_Delay(1);
}

 




void INA219_SetCalibration(uint8_t address, uint16_t calibration)
{
    ina219_calibrationValue = calibration;
    INA219_WriteDataToRegister_16Bits(address, INA219_REG_CALIBRATION, calibration);
}

 




uint16_t INA219_GetConfigInfo(uint8_t Bddr)
{
    uint16_t result = INA219_ReadDataForRegister_16Bits(Bddr, INA219_REG_CONFIG);
    return result;
}

 




void INA219_SetConfig(uint8_t address, uint16_t config)
{
    ina219_configValue = config;
    INA219_WriteDataToRegister_16Bits(address, INA219_REG_CONFIG, config);
}

 




/* Legacy entry point. R100 and +/-320 mV give a +/-3.2 A shunt range. */
void INA219_SetCalibration_16V_8A(uint8_t address)
{
    const uint16_t config = INA219_CONFIG_VOLTAGE_RANGE_16V |
                            INA219_CONFIG_GAIN_8_320MV |
                            INA219_CONFIG_BADCRES_12BIT |
                            INA219_CONFIG_SADCRES_12BIT_1S_532US |
                            INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
    /* 0.04096 / (0.0001 A per count * 0.1 ohm) = 4096. */
    ina219_currentDivider_mA = 10;
    ina219_powerMultiplier_mW = 2.0f;
    INA219_SetConfig(address, config);
    INA219_SetCalibration(address, 4096);
}

 




void INA219_SetCalibration_32V_8A(uint8_t Bddr)
{
    uint16_t configInfo = INA219_CONFIG_VOLTAGE_RANGE_32V |
                          INA219_CONFIG_GAIN_8_320MV |
                          INA219_CONFIG_BADCRES_12BIT |
                          INA219_CONFIG_SADCRES_12BIT_1S_532US |
                          INA219_CONFIG_MODE_SVOLT_CONTINUOUS;

     
     
     
    ina219_calibrationValue = 4096;

     
    ina219_currentDivider_mA = 5;

     
    ina219_powerMultiplier_mW = 0.25f;

    INA219_SetConfig(Bddr, configInfo);
    HAL_Delay(10);
    INA219_SetCalibration(Bddr, ina219_calibrationValue);
}

 




void INA219_SetCalibration_16V_800mA(uint8_t Bddr)
{
    uint16_t configInfo = INA219_CONFIG_VOLTAGE_RANGE_16V |
                          INA219_CONFIG_GAIN_1_40MV |
                          INA219_CONFIG_BADCRES_12BIT |
                          INA219_CONFIG_SADCRES_12BIT_1S_532US |
                          INA219_CONFIG_MODE_SVOLT_CONTINUOUS;

     
     
    ina219_calibrationValue = 33554;

     
    ina219_currentDivider_mA = 41;

     
    ina219_powerMultiplier_mW = 0.25f;

    INA219_SetConfig(Bddr, configInfo);
    HAL_Delay(10);
    INA219_SetCalibration(Bddr, ina219_calibrationValue);
}

uint16_t resulto[4] = {0, 0, 0, 0};    

 




uint8_t INA219_Init(uint8_t Bddr)
{
    INA219_Reset(Bddr);
    INA219_SetCalibration_16V_8A(Bddr);    
    return 0;
}

 


void INA219a_Init(void)
{
    INA219_Init(INA219_ADDRESS);
}

 




















