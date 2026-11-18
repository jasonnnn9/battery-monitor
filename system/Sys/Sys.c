#include "main.h"
#include "sys.h"




/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(uint32_t plln)
{
		HAL_StatusTypeDef ret = HAL_ERROR;
    RCC_OscInitTypeDef rcc_osc_init = {0};
    RCC_ClkInitTypeDef rcc_clk_init = {0};
		RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
		
		
    rcc_osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSE;        
    rcc_osc_init.HSEState = RCC_HSE_ON;                          
    rcc_osc_init.HSEPredivValue = RCC_HSE_PREDIV_DIV1;           
    rcc_osc_init.PLL.PLLState = RCC_PLL_ON;                      
    rcc_osc_init.PLL.PLLSource = RCC_PLLSOURCE_HSE;              
    rcc_osc_init.PLL.PLLMUL = plln;                              
    ret = HAL_RCC_OscConfig(&rcc_osc_init);                      

    if (ret != HAL_OK)
    {
        while (1);                                               
    }

     
    rcc_clk_init.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    rcc_clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;         
    rcc_clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;                
    rcc_clk_init.APB1CLKDivider = RCC_HCLK_DIV2;                 
    rcc_clk_init.APB2CLKDivider = RCC_HCLK_DIV1;                 
    ret = HAL_RCC_ClockConfig(&rcc_clk_init, FLASH_LATENCY_2);   

    if (ret != HAL_OK)
    {
        while (1);                                               
    }
		
		PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
		PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
		if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
		{
			Error_Handler();
		}
}



/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	
  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pin : PB1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
}





#ifdef  USE_FULL_ASSERT
 
 
 
void assert_failed(uint8_t* file, uint32_t line)
{ 
	while (1)
	{
	}
}
#endif








