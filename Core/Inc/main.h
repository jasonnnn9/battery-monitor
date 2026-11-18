/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

 
/*----------------------------sys_start--------------------------------*/
#include "sys.h"
#include "iic.h"
#include "spi.h"
#include "adc.h"
#include "timer.h"
#include "usart.h"
#include "wdg.h"
#include "delay.h"
#include "init.h"
#include "algo.h"
#include "flash.h"
#include "keypad.h"
//#include "cJSON.h"
/*-----------------------------sys_end---------------------------------*/




 
/*---------------------------C or C++_start----------------------------*/
#include "stdint.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "math.h"
#include "float.h"
#include "stdarg.h"

/*----------------------------C or C++_end-----------------------------*/




/*----------------------------Hardware_start---------------------------*/
 
 




#include "SD1306.h"                                         
 
 
/*----------------------------------------------------------------------*/
 
//#include "esp32.h"                                       //WIFI
#include "esp8266.h"                                      //WIFI
//#include "esp8266_al.h"
 
//#include "BC26.h"                                         //NBIOT 
 
/*----------------------------------------------------------------------*/
 
 
 
 
 


/*----------------------------------------------------------------------*/
 
 
 
 
 
 
//#include "dustle.h"																				//pm2.5
 
 
//#include "herle.h"                                        //
 
 
 
 
 
 
 
 
/*----------------------------------------------------------------------*/
 
#include "ds18b20.h"                                         
 
 
 
 
//#include "ds1302.h"																					//RTC
 
 
#include "ina219.h"                                          
 
 
 
 





















/*----------------------------Hardware_end-----------------------------*/





#define TRUE    										(1U)
#define FALSE   									  (0U)
#define ON                          (1U)
#define OFF                         (0U)
#define Hi                          (1U)
#define Lo                          (0U)  

#define  Regaddr      								 			0x02     


#define  English_Display              			 OFF   
#define  TEST                         			 OFF   
#define  WDT_RUN                      			 OFF   



#define  Enable_UART1                        ON 
#define  Enable_UART2                        OFF 
#define  Enable_UART3                        OFF 

#define  Enable_TIM1                         OFF 
#define  Enable_TIM2                         ON 
#define  Enable_TIM3                         OFF 
#define  Enable_TIM4                         ON 

#define  SPI_1                            	 OFF 	//
#define  SPI_2                            	 ON 	//
#define  SPI_3                            	 OFF 	//
#define  Hard_SPI                         	 ON 	//
#define  Soft_SPI                         	 OFF  //


#define  Ina219              					 			ON  	 
#define  AT24CXX              				 		 	OFF  	 
#define  mlx906															OFF  	 
#define  bmp280															OFF  	 
#define  sd1306s														ON    
#define  adxl345														OFF    
#define  max30102												  	OFF    
#define  pcf8574												  	OFF     
#define  mpu6050												  	OFF    





void Error_Handler(void);
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
