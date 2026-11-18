#ifndef __LCD1602B_H__
#define	__LCD1602B_H__
 
#include "main.h"

 
 
#define SIIC_SCL_RCC_CLK_ENABLE()         __HAL_RCC_GPIOC_CLK_ENABLE()
#define SIIC_SCL_GPIO_PIN                 GPIO_PIN_13
#define SIIC_SCL_GPIO                     GPIOC

#define SIIC_SDA_RCC_CLK_ENABLE()         __HAL_RCC_GPIOC_CLK_ENABLE()
#define SIIC_SDA_GPIO_PIN                 GPIO_PIN_14
#define SIIC_SDA_GPIO                     GPIOC

#define SIIC_SCL_ON                       HAL_GPIO_WritePin(SIIC_SCL_GPIO,SIIC_SCL_GPIO_PIN,GPIO_PIN_SET)     
#define SIIC_SCL_OFF                      HAL_GPIO_WritePin(SIIC_SCL_GPIO,SIIC_SCL_GPIO_PIN,GPIO_PIN_RESET)   

#define SIIC_SDA_ON                       HAL_GPIO_WritePin(SIIC_SDA_GPIO,SIIC_SDA_GPIO_PIN,GPIO_PIN_SET)     
#define SIIC_SDA_OFF                      HAL_GPIO_WritePin(SIIC_SDA_GPIO,SIIC_SDA_GPIO_PIN,GPIO_PIN_RESET)   


 
 
 
 
#define IIC_SCL(n)  (n?HAL_GPIO_WritePin(SIIC_SCL_GPIO,SIIC_SCL_GPIO_PIN,GPIO_PIN_SET):HAL_GPIO_WritePin(SIIC_SCL_GPIO,SIIC_SCL_GPIO_PIN,GPIO_PIN_RESET)) //SCL
#define IIC_SDA(n)  (n?HAL_GPIO_WritePin(SIIC_SDA_GPIO,SIIC_SDA_GPIO_PIN,GPIO_PIN_SET):HAL_GPIO_WritePin(SIIC_SDA_GPIO,SIIC_SDA_GPIO_PIN,GPIO_PIN_RESET)) //SDA
#define READ_SDA    HAL_GPIO_ReadPin(SIIC_SDA_GPIO,SIIC_SDA_GPIO_PIN)   

 
void IIC_Init(void);                 
void IIC_Start(void);				 
void IIC_Stop(void);	  			 
void IIC_Send_Byte(u8 txd);			 
u8 IIC_Read_Byte(unsigned char ack); 
u8 IIC_Wait_Ack(void); 				 
void IIC_Ack(void);					 
void IIC_NAck(void);				 

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	 


///////////PCF8574/////////////////////////////////////////////////////////////////////////////

 

#define PCF8574_ADDR 	0X4e	 

 
#define RS_IO         0		  //RS
#define RW_IO       1   	//RW
#define CS_IO    2    	//CS
#define BL_IO      3    	//P3,BackLight
#define P4_IO      		  4    	//P4
#define P5_IO      5   	//P5
#define P6_IO     6    	//P6
#define P7_IO    7    	//P7

u8 PCF8574_Init(u8 addr); 
u8 PCF8574_ReadOneByte(void); 
void PCF8574_WriteOneByte(u8 DataToWrite);
void PCF8574_WriteBit(u8 bit,u8 sta);
u8 PCF8574_ReadBit(u8 bit);


void LCD1602test_Init(void);
void LCD_Write_Com(unsigned char command); 
void LCD_Write_Data(unsigned char value);
void LCD_Write_String(unsigned char x,unsigned char y,unsigned char *s) ;
void LcdCurMove(uint8_t x,uint8_t y,uint8_t blink_en);

#endif /* __BSP_I2C_H__ */
 






 
