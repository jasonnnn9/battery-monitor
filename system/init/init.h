#ifndef __INIT_h__
#define __INIT_h__
#include "main.h"

/* Set to 1 to enable buzzer feedback after bench testing. */
#define BEEP_ENABLED 0

#define RELAY_GPIO_PORT            	 	GPIOA																			 
#define RELAY_GPIO_PIN                GPIO_PIN_12
#define RELAY_GPIO_CLK_ENABLE()       do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0) 

#define RELAY1_GPIO_PORT            	 GPIOB																		 
#define RELAY1_GPIO_PIN                GPIO_PIN_5
#define RELAY1_GPIO_CLK_ENABLE()       do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0) 

#define RELAY2_GPIO_PORT            	 GPIOB																		
#define RELAY2_GPIO_PIN                GPIO_PIN_10
#define RELAY2_GPIO_CLK_ENABLE()       do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0) 

#define RELAY3_GPIO_PORT            	 GPIOB																		
#define RELAY3_GPIO_PIN                GPIO_PIN_11
#define RELAY3_GPIO_CLK_ENABLE()       do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)

#define LED_GPIO_PORT            	 GPIOB																		
#define LED_GPIO_PIN                GPIO_PIN_9
#define LED_GPIO_CLK_ENABLE()       do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)

#define BEEP_GPIO_PORT            	 GPIOA
#define BEEP_GPIO_PIN                GPIO_PIN_11
#define BEEP_GPIO_CLK_ENABLE()       do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0) 


#define RELAY(x)  	  do{ x ? \
												HAL_GPIO_WritePin(RELAY_GPIO_PORT, RELAY_GPIO_PIN, GPIO_PIN_SET) : \
												HAL_GPIO_WritePin(RELAY_GPIO_PORT, RELAY_GPIO_PIN, GPIO_PIN_RESET); \
												}while(0)  

#define RELAY1(x)  	  do{ x ? \
												HAL_GPIO_WritePin(RELAY1_GPIO_PORT, RELAY1_GPIO_PIN, GPIO_PIN_SET) : \
												HAL_GPIO_WritePin(RELAY1_GPIO_PORT, RELAY1_GPIO_PIN, GPIO_PIN_RESET); \
												}while(0)  
#define RELAY2(x)  	  do{ x ? \
												HAL_GPIO_WritePin(RELAY2_GPIO_PORT, RELAY2_GPIO_PIN, GPIO_PIN_SET) : \
												HAL_GPIO_WritePin(RELAY2_GPIO_PORT, RELAY2_GPIO_PIN, GPIO_PIN_RESET); \
												}while(0) 
#define RELAY3(x)  	  do{ x ? \
												HAL_GPIO_WritePin(RELAY3_GPIO_PORT, RELAY3_GPIO_PIN, GPIO_PIN_SET) : \
												HAL_GPIO_WritePin(RELAY3_GPIO_PORT, RELAY3_GPIO_PIN, GPIO_PIN_RESET); \
												}while(0) 
#define LED(x)  	  do{ x ? \
												HAL_GPIO_WritePin(LED_GPIO_PORT,LED_GPIO_PIN, GPIO_PIN_SET) : \
												HAL_GPIO_WritePin(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_PIN_RESET); \
												}while(0) 
#define BEEP(x)  	  do{ x ? \
												HAL_GPIO_WritePin(BEEP_GPIO_PORT, BEEP_GPIO_PIN, GPIO_PIN_SET) : \
												HAL_GPIO_WritePin(BEEP_GPIO_PORT, BEEP_GPIO_PIN, GPIO_PIN_RESET); \
												}while(0)  

void MX_Hard_Init(void);

uint8_t Key_Scan(void) ;
void GPIO_EXTI_Init(void);
void BEEP_DOING(void);
void GPIO_RELAY_Init(void);	
void BuzzerRingsNum(u8 num);
#endif





