#include "main.h"
#include "timer.h"

extern volatile u8 warningFlag;

TIM_HandleTypeDef htim1,htim2,htim3,htim4;
uint8_t UR1_flg,UR2_flg,UR3_flg,UR4_flg;
uint16_t timer1_flag_1;
uint16_t timer2_flag_1,timer2_flag_2;
/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
	
 
 
 
 
 
/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
	
#if(Enable_TIM1)			
void TIM1_Init(u16 arr,u16 psc)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = psc;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = arr;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  HAL_TIM_Base_Init(&htim1);
	HAL_TIM_Base_Start_IT(&htim1);  
}

#endif	

#if(Enable_TIM2)	
/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
void TIM2_Init(u16 arr,u16 psc)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = psc;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = arr;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  HAL_TIM_Base_Init(&htim2);
	HAL_TIM_Base_Start_IT(&htim2);  

}
#endif	

#if(Enable_TIM3)	
/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
void TIM3_Init(u16 arr,u16 psc)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = psc;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = arr;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  HAL_TIM_Base_Init(&htim3);
	HAL_TIM_Base_Start_IT(&htim3);  
}
#endif	

#if(Enable_TIM4)	
/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
void TIM4_Init(u16 arr,u16 psc)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = psc;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = arr;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  HAL_TIM_Base_Init(&htim4);
	HAL_TIM_Base_Start_IT(&htim4);  

}

#endif	


/**
* @brief TIM_Base MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param htim_base: TIM_Base handle pointer
* @retval None
*/

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_Initure;

	if(htim->Instance==TIM1)
		{
		 __HAL_RCC_TIM1_CLK_ENABLE();          
		 HAL_NVIC_SetPriority(TIM1_BRK_IRQn,1,0);    
		 HAL_NVIC_EnableIRQ(TIM1_BRK_IRQn); 		
		}	
		
	if(htim->Instance==TIM2)
		{
		
		 __HAL_RCC_TIM2_CLK_ENABLE();            
		 HAL_NVIC_SetPriority(TIM2_IRQn,2,0);    
		 HAL_NVIC_EnableIRQ(TIM2_IRQn);          
		}	
		
	if(htim->Instance==TIM3)
		{
		 __HAL_RCC_TIM3_CLK_ENABLE();            
		 HAL_NVIC_SetPriority(TIM3_IRQn,3,0);    
		 HAL_NVIC_EnableIRQ(TIM3_IRQn);          
		}	
	if(htim->Instance==TIM4)
		{
		 __HAL_RCC_TIM4_CLK_ENABLE();            
		 HAL_NVIC_SetPriority(TIM4_IRQn,4,0);    
		 HAL_NVIC_EnableIRQ(TIM4_IRQn);          
		}	
}

 
 
 
 
 

void MX_TIMx_Init(void)
{
 
    TIM2_Init(7999,10);   //0.01S
//	TIM3_Init(1999,143);   //0.004S
		TIM4_Init(7999,10);
}



 
void TIM1_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim1);
}

 
void TIM2_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim2);
	
}
 
void TIM3_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim3);
	
} 
void TIM4_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim4);
	
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{ 
	
	 
	if(htim->Instance==htim1.Instance){
		timer_1_handle();
		HAL_TIM_Base_Stop_IT(&htim1);	
	}
	if(htim->Instance==htim2.Instance){
	  timer_2_handle();
//	  HAL_TIM_Base_Stop_IT(&htim2);
	}
	if(htim->Instance==htim3.Instance){
	  timer_3_handle();
//	  HAL_TIM_Base_Stop_IT(&htim3);
	}	
	if(htim->Instance==htim4.Instance){
		timer_4_handle();
//		HAL_TIM_Base_Stop_IT(&htim4);
	}	
	
}


void timer_1_handle(void) 
{
	 timer1_flag_1=1;
}	

void timer_2_handle(void)
{
    /* Keep status feedback responsive during foreground display and UART work. */
    LED(warningFlag == 0 || ((HAL_GetTick() / 300u) & 1u));
	 timer2_flag_1=1;
		KeyScan();
}

void timer_3_handle(void)
{
	
}

void timer_4_handle(void)
{
	
}






























 
#if(Enable_Capture)	
void Capture_TIMx_Init(void)
{
	TIM2_Init(0xFFFF,1);    
}	

void TIM2_Init(uint16_t arr,uint16_t psc )
{   
  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_IC_InitTypeDef sConfigIC = {0};
  
  htim2.Instance = TIM2;                                                
  htim2.Init.Prescaler = psc;                                             
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;                          
  htim2.Init.Period = arr;                                              
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;                    
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;        
  HAL_TIM_Base_Init(&htim2);

    
   sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
   sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
   sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
   sConfigIC.ICFilter = 0;
   HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_1);  
/*-------------------------------------------------*/ 
  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_EXTERNAL1;                         
  sSlaveConfig.InputTrigger = TIM_TS_TI1F_ED;                               
  sSlaveConfig.TriggerFilter = 0;                                           
  HAL_TIM_SlaveConfigSynchro(&htim2, &sSlaveConfig);
  
  
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;       	 
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;   
  HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig); 
/*-------------------------------------------------*/ 
	HAL_TIM_Base_MspInit(&htim2);
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_IC_Start(&htim2, TIM_CHANNEL_1);
//	

}
#endif


	



