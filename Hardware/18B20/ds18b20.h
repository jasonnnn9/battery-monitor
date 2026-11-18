#ifndef __DS18B20_H
#define __DS18B20_H
#include "main.h"



#define BS18B20_GPIO_PORT            	    GPIOA
#define BS18B20_GPIO_DATA                 GPIO_PIN_8

#define DS18B20_DQ_OUT(x)  	  do{ x ? \
																HAL_GPIO_WritePin(BS18B20_GPIO_PORT, BS18B20_GPIO_DATA, GPIO_PIN_SET) : \
																HAL_GPIO_WritePin(BS18B20_GPIO_PORT, BS18B20_GPIO_DATA, GPIO_PIN_RESET); \
																}while(0)  

#define DS18B20_DQ_IN       HAL_GPIO_ReadPin(BS18B20_GPIO_PORT, BS18B20_GPIO_DATA)      
															
															
 
//#define DS18B20_IO_IN()  {GPIOA->CRL&=0XFFFFFFF0;GPIOA->CRL|=8<<0;}
//#define DS18B20_IO_OUT() {GPIOA->CRL&=0XFFFFFFF0;GPIOA->CRL|=3<<0;}

 
 
 

 
 
   	
u8 DS18B20_Init(void);			 
short DS18B20_Get_Temp(void);	 
void DS18B20_Start(void);		 
void DS18B20_Write_Byte(u8 dat); 
u8 DS18B20_Read_Byte(void);		 
u8 DS18B20_Read_Bit(void);		 
u8 DS18B20_Check(void);			 
void DS18B20_Rst(void);			 
void OutConfig(void);
void InConfig(void);
void xx(void);
void DS18B20_Process(void);
void DS18B20_Start_Conv(void);
extern uint8_t  ds18_start_flag ;

extern uint16_t ds18_delay_cnt;
#endif
