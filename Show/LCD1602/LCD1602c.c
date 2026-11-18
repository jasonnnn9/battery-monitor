 








 

#include "main.h"
#include "lcd1602b.h"
 
 


 

 
 
 
 







//I2C//////////////////////////////////////////////////////////////////////////////
unsigned char LCD_data=0x08;
 

void SDA_OUT(void)
{  
  	GPIO_InitTypeDef GPIO_InitStruct;
		SIIC_SDA_RCC_CLK_ENABLE();
		GPIO_InitStruct.Pin = SIIC_SDA_GPIO_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(SIIC_SCL_GPIO, &GPIO_InitStruct);
}	

void SDA_IN(void)	
{	
		GPIO_InitTypeDef GPIO_InitStruct;
		SIIC_SDA_RCC_CLK_ENABLE();
		GPIO_InitStruct.Pin = SIIC_SDA_GPIO_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
		HAL_GPIO_Init(SIIC_SCL_GPIO, &GPIO_InitStruct);
}

void IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	SIIC_SCL_RCC_CLK_ENABLE();
	SIIC_SDA_RCC_CLK_ENABLE();

	GPIO_Initure.Pin=SIIC_SCL_GPIO_PIN|SIIC_SDA_GPIO_PIN;
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_OD;   
	GPIO_Initure.Pull=GPIO_PULLUP;           
	GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;      
	HAL_GPIO_Init(SIIC_SCL_GPIO,&GPIO_Initure);

	IIC_SDA(1);
	IIC_SCL(1);
}

 
void IIC_Start(void)
{
	SDA_OUT();      
	IIC_SDA(1);
	IIC_SCL(1);
	delay_us(4);
	IIC_SDA(0);//START:when CLK is high,DATA change form high to low
	delay_us(4);
	IIC_SCL(0); 
}
 
void IIC_Stop(void)
{
	SDA_OUT(); 
	IIC_SCL(0);
	IIC_SDA(0);//STOP:when CLK is high DATA change form low to high
	delay_us(4);
	IIC_SCL(1);
	delay_us(4);
	IIC_SDA(1); 
}
 
 
 
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();       
	IIC_SDA(1);
	delay_us(1);
	IIC_SCL(1);
	delay_us(1);
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL(0); 
	return 0;
}
 
void IIC_Ack(void)
{
	IIC_SCL(0);
	SDA_OUT();
	IIC_SDA(0);
	delay_us(2);
	IIC_SCL(1);
	delay_us(2);
	IIC_SCL(0);
}
 
void IIC_NAck(void)
{
	IIC_SCL(0);
	SDA_OUT();
	IIC_SDA(1);
	delay_us(2);
	IIC_SCL(1);
	delay_us(2);
	IIC_SCL(0);
}
 
 
 
 
void IIC_Send_Byte(u8 txd)
{
	u8 t;
	SDA_OUT();
	IIC_SCL(0); 
	for(t=0; t<8; t++)
	{
		IIC_SDA((txd&0x80)>>7);
		txd<<=1;
		delay_us(2);    
		IIC_SCL(1);
		delay_us(2);
		IIC_SCL(0);
		delay_us(2);
	}
}
 
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN(); 
	for(i=0; i<8; i++ )
	{
		IIC_SCL(0);
		delay_us(2);
		IIC_SCL(1);
		receive<<=1;
		if(READ_SDA)receive++;
		delay_us(1);
	}
	if (!ack)
		IIC_NAck(); 
	else
		IIC_Ack();  
	return receive;
}


//END I2C//////////////////////////////////////////////////////////////////////////////


void Enable_LCD_write()
{
	LCD_data|=(1<<(3-1));//E=1;
	PCF8574_WriteOneByte(LCD_data);
	LCD_data&=~(1<<(3-1));//E=0;
	PCF8574_WriteOneByte(LCD_data);
}


void LCD_Write_Com(unsigned char command) 
{ 
	delay_us(16);
	LCD_data&=~(1<<(1-1));//RS=0;
	LCD_data&=~(1<<(2-1));//RW=0;
	//LCD_data&=~(1<<(4-1));
	delay_us(2);
	PCF8574_WriteOneByte(LCD_data);
	
	LCD_data&=0X0f; //????
	LCD_data|=command & 0xf0; //????
	delay_us(2);
	PCF8574_WriteOneByte(LCD_data);
	Enable_LCD_write();
	
	command=command<<4; //????????
	LCD_data&=0x0f; //????
	LCD_data|=command&0xf0; //????
	delay_us(2);
	PCF8574_WriteOneByte(LCD_data);
	delay_us(2);
	Enable_LCD_write();
}

void LCD_Write_Data(unsigned char value) 
{ 
	delay_us(16);
	LCD_data|=(1<<(1-1));//RS=1;
	LCD_data&=~(1<<(2-1));//RW=0;
	delay_us(2);
	PCF8574_WriteOneByte(LCD_data);
	
	LCD_data&=0X0f; //????
	LCD_data|=value&0xf0; //????
	delay_us(2);
	PCF8574_WriteOneByte(LCD_data);
	delay_us(2);
	Enable_LCD_write();         
	
	value=value<<4; //????????
	LCD_data&=0x0f; //????
	LCD_data|=value&0xf0; //????
	delay_us(2);
	PCF8574_WriteOneByte(LCD_data);
	delay_us(2);
	Enable_LCD_write();
}

void LCD1602test_Init(void) 
{		
	while(PCF8574_Init(PCF8574_ADDR))		 
	PCF8574_WriteOneByte(0x08); 
	LCD_Write_Com(0x0F);
	delay_us(40);
	LCD_Write_Com(0x28); //4???!!!!!!!!!!!!!!!!!!
	PCF8574_WriteOneByte(0x0c); //
	PCF8574_WriteOneByte(0x08);  
	LCD_Write_Com(0x28); 
	LCD_Write_Com(0x01); 
	LCD_Write_Com(0x0C); 
	LCD_Write_Com(0x06);
	delay_ms(10);
}
void LCD_Write_String(unsigned char x,unsigned char y,unsigned char *s) 
{ if(x == 0) x +=1;
	if ((y == 1)| (y == 0))
	{     
		LCD_Write_Com(0x80 + x-1);     //
	}
	else if (y == 2) 
	{      
		LCD_Write_Com(0xC0 + x-1);      //
	}    
	else if (y == 3) 
	{      
		LCD_Write_Com(0x94 + x-1);      //
	}  
	else if (y == 4) 
	{      
		LCD_Write_Com(0xD4 + x-1);      //
	}  	
	while (*s) 
	{     
		LCD_Write_Data( *s);     
		s ++;     
	}
}

 
void LcdCurMove(uint8_t x,uint8_t y,uint8_t blink_en)
{  if(x == 0) x +=1;
	if ((y == 1)| (y == 0))
	{     
		LCD_Write_Com(0x80 + x-1);     //
	}
	else if (y == 2) 
	{      
		LCD_Write_Com(0xC0 + x-1);      //
	}    
	else if (y == 3) 
	{      
		LCD_Write_Com(0x94 + x-1);      //
	}  
	else if (y == 4) 
	{      
		LCD_Write_Com(0xD4 + x-1);      //
	}  	
	if(blink_en) LCD_Write_Com(0x0F);
	else LCD_Write_Com(0x0C);
} 


 
u8 PCF8574_Init(u8 addr)
{
	u8 temp=0;
	
//    GPIO_InitTypeDef GPIO_Initure;
 
//	
//    GPIO_Initure.Pin=GPIO_PIN_12;           //PB12
 
 
 
 
	
	IIC_Init();					             
	 
	IIC_Start();    
	IIC_Send_Byte(addr);             
	temp=IIC_Wait_Ack();		             
	IIC_Stop();					             
	PCF8574_WriteOneByte(0XFF);	             
	
	return temp;
}


 
 
u8 PCF8574_ReadOneByte(void)
{				  
	u8 temp=0;		  	    																 
	IIC_Start();    	 	   
	IIC_Send_Byte(PCF8574_ADDR|0X01);    
	IIC_Wait_Ack();	 
	temp=IIC_Read_Byte(0);		   
	IIC_Stop();							 
	return temp;
}
 
 
void PCF8574_WriteOneByte(u8 DataToWrite)
{				   	  	    																 
	IIC_Start();  
	IIC_Send_Byte(PCF8574_ADDR);    
	IIC_Wait_Ack();	    										  		   
	IIC_Send_Byte(DataToWrite);    	 	 
	IIC_Wait_Ack();   
	IIC_Stop();							 
	HAL_Delay(1);	 
}

 
 
 
void PCF8574_WriteBit(u8 bit,u8 sta)
{
	u8 data;
	data=PCF8574_ReadOneByte();  
	if(sta==0)data&=~(1<<bit);     
	else data|=1<<bit;
	PCF8574_WriteOneByte(data);  
}

 
 
 
u8 PCF8574_ReadBit(u8 bit)
{
	u8 data;
	data=PCF8574_ReadOneByte();  
	if(data&(1<<bit))return 1;
	else return 0;   
}  


