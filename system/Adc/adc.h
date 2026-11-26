#ifndef __ADC_h__
#define __ADC_h__
#include "main.h"
extern ADC_HandleTypeDef hadc1;

 
 
#define ADC_A0     0    // PA0
#define ADC_A1     0    // PA1
#define ADC_A2     0    // PA2
#define ADC_A3     0    // PA3
#define ADC_A4     0    // PA4
#define ADC_A5     0    // PA5
#define ADC_A6     0    // PA6
#define ADC_A7     0    // PA7
#define ADC_B0     0    // PB0
#define ADC_B1     1    // PB1
// ==============================================================

void MX_ADC1_Init(void);
uint16_t ADC_GetValue(uint32_t channel);

 
uint16_t Get_ADC_A0(void);
uint16_t Get_ADC_A1(void);
uint16_t Get_ADC_A2(void);
uint16_t Get_ADC_A3(void);
uint16_t Get_ADC_A4(void);
uint16_t Get_ADC_A5(void);
uint16_t Get_ADC_A6(void);
uint16_t Get_ADC_A7(void);
uint16_t Get_ADC_B0(void);
uint16_t Get_ADC_B1(void);
float ADC_BatteryVoltageFromRaw(uint16_t sample);
float ADC_ReadBatteryVoltage(void);

void MX_ADC_Init(void);
#endif




