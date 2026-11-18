#include "main.h"
#include "ina219.h"

IIC_HardWareTypedef iicina219;

uint16_t ina219_calibrationValue;        
uint16_t ina219_currentDivider_mA;       
float ina219_powerMultiplier_mW;         

float ina219_currentDivider_uA;          
float ina219_powerMultiplier_mW;         

 





uint16_t INA219_ReadDataForRegister_16Bits(uint8_t Bddr, uint8_t registerAddress)
{
    uint8_t Value[2];
    IIC_Read_Device_2Byte_Len(Bddr, registerAddress, 2, Value, iicina219);
    return ((Value[0] << 8) | Value[1]);
}

 





void INA219_WriteDataToRegister_16Bits(uint8_t Bddr, uint8_t registerAddress, uint16_t Value)
{
    IIC_Write_Device_2Byte(Bddr, registerAddress, 2, Value, iicina219);
}

 




uint16_t INA219_Readwatt(uint8_t Bddr)
{	
    uint16_t result = INA219_ReadBusVoltage(INA219_ADDRESS)*INA219_ReadCurrent_mA(INA219_ADDRESS)/1000;    
    return result ;
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

 




uint16_t INA219_ReadCurrent_mA(uint8_t Bddr)
{
    uint16_t result = 0, result1 = 0;
    HAL_Delay(10);
    result = INA219_ReadCurrentRaw(Bddr);
    if (result >= 0x3332) result = 0;           
    return result1 = (result / ina219_currentDivider_mA);
}

 




uint16_t INA219_ReadCurrent_uA(uint8_t Bddr)
{
    uint16_t result = 0, result1 = 0;
    HAL_Delay(10);
    result = INA219_ReadCurrentRaw(Bddr);
    if (result >= 0x3332) result = 0;           
    return result1 = (result / ina219_currentDivider_uA);
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

 




void INA219_SetCalibration(uint8_t Bddr, uint16_t calibrationData)
{
    uint8_t regAddr[2] = {0, 0};    
    INA219_WriteDataToRegister_16Bits(Bddr, INA219_REG_CALIBRATION, calibrationData);
}

 




uint16_t INA219_GetConfigInfo(uint8_t Bddr)
{
    uint16_t result = INA219_ReadDataForRegister_16Bits(Bddr, INA219_REG_CONFIG);
    return result;
}

 




void INA219_SetConfig(uint8_t Bddr, uint16_t configData)
{
    INA219_WriteDataToRegister_16Bits(Bddr, INA219_REG_CONFIG, configData);
}

 




void INA219_SetCalibration_16V_8A(uint8_t Bddr)
{
    uint16_t configInfo = INA219_CONFIG_VOLTAGE_RANGE_16V |
                          INA219_CONFIG_GAIN_1_40MV |
                          INA219_CONFIG_BADCRES_12BIT |
                          INA219_CONFIG_SADCRES_12BIT_1S_532US |
                          INA219_CONFIG_MODE_SVOLT_CONTINUOUS;
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
	
		INA219_Reset(INA219_ADDRESS);
		HAL_Delay(50);
		INA219_SetCalibration_16V_8A(INA219_ADDRESS);
		HAL_Delay(50);
}

 




















