 









#include "main.h"
extern uint8_t data_index;

int main(void)
{
         
    HAL_Init();
    SystemClock_Config(RCC_PLL_MUL9);
    delay_init(72);

     
    MX_IIC_Init();
    HAL_Delay(20);   

    OLED_Init();
    OLED_CLS(); 
    Uart_Init();
    MX_ADC_Init();
    MX_SPIx_Init();
    MX_TIMx_Init();
    MX_Hard_Init();  
#if(WDT_RUN)
    IWDG_init(IWDG_PRESCALER_64, 625*6);
    IWDG_feed();
#endif

    while (1)
    {
			
			  UpdateMeasurements();
              CotorFun();
              KeyScanHandle();
			  Display();	
				
        if(setn == 1)
        {
					
            UsartSendReceiveData();
            UsartReceive();
        }
        

#if(WDT_RUN)
        IWDG_feed();
#endif
    }
}

void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
