#include "main.h"
#include "init.h"
 

extern uint16_t speter_flag;

 
//void EXTI9_5_IRQHandler(void) {
//    if (__HAL_GPIO_EXTI_GET_IT(SPET_OUT_PIN) != RESET) {	
//					speter_flag++;	
//          if(speter_flag>0xffff)speter_flag=0; 			
//        __HAL_GPIO_EXTI_CLEAR_IT(SPET_OUT_PIN);
//    }
//}



 
//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

//}

 




void GPIO_EXTI_Init(void)
{
//	GPIO_InitTypeDef gpio_init = {0};
//	
 
//    GPIO_InitTypeDef GPIO_InitStruct = {0};
//    GPIO_InitStruct.Pin = TCS3200_OUT_PIN;
 
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    HAL_GPIO_Init(TCS3200_OUT_PORT, &GPIO_InitStruct);

 
//    HAL_NVIC_SetPriority(TCS3200_EXTI_IRQn, 0, 0);
//    HAL_NVIC_EnableIRQ(TCS3200_EXTI_IRQn);
	
//		GPIO_InitTypeDef gpio_init = {0};
//	
 
//    GPIO_InitTypeDef GPIO_InitStruct = {0};
//    GPIO_InitStruct.Pin = SPET_OUT_PIN;
 
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    HAL_GPIO_Init(SPET_OUT_PORT, &GPIO_InitStruct);

 
//    HAL_NVIC_SetPriority(SPET_EXTI_IRQn, 0, 0);
//    HAL_NVIC_EnableIRQ(SPET_EXTI_IRQn);	
	
	
	
}


void GPIO_RELAY_Init(void)	
{	
		GPIO_InitTypeDef GPIO_InitStruct;
		RELAY_GPIO_CLK_ENABLE();
		GPIO_InitStruct.Pin = RELAY_GPIO_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(RELAY_GPIO_PORT, &GPIO_InitStruct);
	
		RELAY1_GPIO_CLK_ENABLE();
		GPIO_InitStruct.Pin = RELAY1_GPIO_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(RELAY1_GPIO_PORT, &GPIO_InitStruct);
	
		RELAY2_GPIO_CLK_ENABLE();
		GPIO_InitStruct.Pin = RELAY2_GPIO_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(RELAY2_GPIO_PORT, &GPIO_InitStruct);
		
		RELAY3_GPIO_CLK_ENABLE();
		GPIO_InitStruct.Pin = RELAY3_GPIO_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(RELAY3_GPIO_PORT, &GPIO_InitStruct);
		
		LED_GPIO_CLK_ENABLE();
		GPIO_InitStruct.Pin = LED_GPIO_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);
}	

void GPIO_BEEP_Init(void)	
{	
		GPIO_InitTypeDef GPIO_InitStruct;
		BEEP_GPIO_CLK_ENABLE();
		GPIO_InitStruct.Pin = BEEP_GPIO_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(BEEP_GPIO_PORT, &GPIO_InitStruct);
}	


void BEEP_DOING(void)
{
	BEEP(Hi);  
	delay_ms(200);
	BEEP(Lo); 	
}	

void BuzzerRingsNum(u8 num) 
{
	  for(uint8_t i=0; i<num; i++)
		{
				BEEP(Hi);
			  delay_ms(100);
			 	BEEP(Lo);
			  if(num!=1)
				delay_ms(50);
		}
}



void MX_Hard_Init(void)
{
 
  	//LCD1602_Init();  //LCD
//		OLED_Init();
//    OLED_CLS();	
		ESP8266_Init();
		KEY_Init();       
		GPIO_RELAY_Init();
		GPIO_BEEP_Init();
		INA219a_Init();	
		DS18B20_Init();
//	LCD_Init();
//	HAL_Delay(20);
 
	
	
	
	
	
	
	
	
	
	
}	