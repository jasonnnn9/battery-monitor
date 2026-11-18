#include "ds18b20.h"
#include "main.h"
 

void OutConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
 __HAL_RCC_GPIOA_CLK_ENABLE();

		GPIO_InitStruct.Pin = BS18B20_GPIO_DATA;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(BS18B20_GPIO_PORT, &GPIO_InitStruct);
}


void InConfig(void)
{
		GPIO_InitTypeDef GPIO_InitStruct = {0};
	
		GPIO_InitStruct.Pin = BS18B20_GPIO_DATA;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
		HAL_GPIO_Init(BS18B20_GPIO_PORT, &GPIO_InitStruct);
		
}

 
	void DS18B20_Rst(void)	   
{                 
 
	OutConfig();
	DS18B20_DQ_OUT(Lo);
	delay_us(750);       
	DS18B20_DQ_OUT(Hi);
	delay_us(10);       //15US
}

 
 
 
u8 DS18B20_Check(void) 	   
{   
	u8 retry=0;

	InConfig();
    while (DS18B20_DQ_IN&&retry<200)
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=200)return 1;
	else retry=0;
    while (!DS18B20_DQ_IN&&retry<240)
	{
		retry++;
		delay_us(1);
	};
	if(retry>=240)return 1;	    
	return 0;
}

 
 
u8 DS18B20_Read_Bit(void) 
{
	u8 data;
 
	OutConfig();
	DS18B20_DQ_OUT(Lo); 
	delay_us(2);
	DS18B20_DQ_OUT(Hi); 
 
	InConfig();
	delay_us(5);
	if(DS18B20_DQ_IN)data=1;
	else data=0;	 
	delay_us(50);           
	return data;
}

 
 
u8 DS18B20_Read_Byte(void)   
{        
	u8 i,j,dat;
	dat=0;
	for (i=1;i<=8;i++) 
	{
        j=DS18B20_Read_Bit();
        dat=(j<<7)|(dat>>1);
    }						    
	return dat;
}

 
 
void DS18B20_Write_Byte(u8 dat)     
 {             
    u8 j;
    u8 testb;
 
	  OutConfig();
    for (j=1;j<=8;j++) 
	{
        testb=dat&0x01;
        dat=dat>>1;
        if(testb)        
        {
					
            DS18B20_DQ_OUT(Lo); 
            delay_us(2);                            
            DS18B20_DQ_OUT(Hi); 
            delay_us(60);             
        }
        else             
        {
            DS18B20_DQ_OUT(Lo); 
            delay_us(60);             
            DS18B20_DQ_OUT(Hi); 
            delay_us(2);                          
        }
    }
}
 
 
void DS18B20_Start(void)
{   						               
    DS18B20_Rst();	   
    DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);// skip rom
    DS18B20_Write_Byte(0x44);// convert
}

 
 
 
u8 DS18B20_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOA_CLK_ENABLE();			 
	
    GPIO_Initure.Pin=BS18B20_GPIO_DATA;      //PA0
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_OD;   
    GPIO_Initure.Pull=GPIO_PULLUP;           
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH; 
    HAL_GPIO_Init(BS18B20_GPIO_PORT,&GPIO_Initure);      
 
	DS18B20_Rst();
	return DS18B20_Check();
}

 
 
 
uint8_t tempFlag=0;
double textemp;
uint8_t Gao_H;
uint8_t DIN_L;
short DS18B20_Get_Temp(void)
{
    u8 temp;
    u8 TL,TH;
    short tem;
    DS18B20_Start();
	 
	while(tempFlag==0)
	{
	delay_ms(750);    
	tempFlag++;
	}
    delay_ms(20);              
    
    DS18B20_Rst();
    DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);   // skip rom
    DS18B20_Write_Byte(0xbe);   // convert	    
   DIN_L = TL=DS18B20_Read_Byte();     // LSB   
   Gao_H = TH=DS18B20_Read_Byte();     // MSB   
    if(TH>7)
    {
        TH=~TH;
        TL=~TL; 
        temp=0;				 
    }else temp=1;			 
    tem=TH; 					 
    tem<<=8;    
    tem+=TL;					 
		textemp= tem;
    tem=(double)tem*0.625; 
		
	if(temp)return tem;  
	else return -tem;    
}

//void xx(void)
//{
//	  delay_ms(500);
//	  xDQ_OUT=1;
//	  delay_ms(500);
//	  xDQ_OUT=0;	
//}
