#include "main.h"
#include "adc.h"
ADC_HandleTypeDef hadc1;

 
static void ADC_SelectChannel(uint32_t channel, uint32_t samplingTime)
{
  ADC_ChannelConfTypeDef sConfig = {0};
  sConfig.Channel = channel;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = samplingTime;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
}

 
void MX_ADC_Init(void)
{
  RCC_PeriphCLKInitTypeDef adc_clk_init = {0};
  GPIO_InitTypeDef GPIO_InitStruct = {0};

   
  adc_clk_init.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  adc_clk_init.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  HAL_RCCEx_PeriphCLKConfig(&adc_clk_init);

  __HAL_RCC_ADC1_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  HAL_ADC_Init(&hadc1);

   
  HAL_ADCEx_Calibration_Start(&hadc1);

   
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;

  #if ADC_A0
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  #endif

  #if ADC_A1
    GPIO_InitStruct.Pin = GPIO_PIN_1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  #endif

  #if ADC_A2
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  #endif

  #if ADC_A3
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  #endif

  #if ADC_A4
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  #endif

  #if ADC_A5
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  #endif

  #if ADC_A6
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  #endif

  #if ADC_A7
    GPIO_InitStruct.Pin = GPIO_PIN_7;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  #endif

  #if ADC_B0
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  #endif

  #if ADC_B1
    GPIO_InitStruct.Pin = GPIO_PIN_1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  #endif
}

 
uint16_t ADC_GetValue(uint32_t channel)
{
  ADC_SelectChannel(channel, ADC_SAMPLETIME_55CYCLES_5);
  HAL_ADC_Start(&hadc1);
  if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK)
  {
    return (uint16_t)HAL_ADC_GetValue(&hadc1);
  }
  return 0;
}

 
uint16_t Get_ADC_A0(void){return ADC_GetValue(ADC_CHANNEL_0);}
uint16_t Get_ADC_A1(void){return ADC_GetValue(ADC_CHANNEL_1);}
uint16_t Get_ADC_A2(void){return ADC_GetValue(ADC_CHANNEL_2);}
uint16_t Get_ADC_A3(void){return ADC_GetValue(ADC_CHANNEL_3);}
uint16_t Get_ADC_A4(void){return ADC_GetValue(ADC_CHANNEL_4);}
uint16_t Get_ADC_A5(void){return ADC_GetValue(ADC_CHANNEL_5);}
uint16_t Get_ADC_A6(void){return ADC_GetValue(ADC_CHANNEL_6);}
uint16_t Get_ADC_A7(void){return ADC_GetValue(ADC_CHANNEL_7);}
uint16_t Get_ADC_B0(void){return ADC_GetValue(ADC_CHANNEL_8);}
uint16_t Get_ADC_B1(void){return ADC_GetValue(ADC_CHANNEL_9);}

float ADC_BatteryVoltageFromRaw(uint16_t sample)
{
    /* PB1 (ADC1_IN9), 12-bit ADC, nominal 3.3 V reference and 5:1 divider. */
    return sample * (3.3f * 5.0f / 4095.0f);
}

float ADC_ReadBatteryVoltage(void)
{
    return ADC_BatteryVoltageFromRaw(Get_ADC_B1());
}
