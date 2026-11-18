#include "main.h"
#include "keypad.h"

#if(button_Set)


 




void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    KEY0_GPIO_CLK_ENABLE();                                      
    KEY1_GPIO_CLK_ENABLE();                                      
    KEY2_GPIO_CLK_ENABLE();                                      
    KEY3_GPIO_CLK_ENABLE();                                      
	
	
    GPIO_InitStruct.Pin = KEY0_GPIO_PIN;                        
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;                     
    GPIO_InitStruct.Pull = GPIO_PULLUP;                         
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;               
    HAL_GPIO_Init(KEY0_GPIO_PORT, &GPIO_InitStruct);            

    GPIO_InitStruct.Pin = KEY1_GPIO_PIN;                        
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;                     
    GPIO_InitStruct.Pull = GPIO_PULLUP;                         
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;               
    HAL_GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStruct);            

    GPIO_InitStruct.Pin = KEY2_GPIO_PIN;                        
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;                     
    GPIO_InitStruct.Pull = GPIO_PULLUP;                         
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;               
    HAL_GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStruct);            	

    GPIO_InitStruct.Pin = KEY3_GPIO_PIN;                        
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;                     
    GPIO_InitStruct.Pull = GPIO_PULLUP;                         
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;               
    HAL_GPIO_Init(KEY3_GPIO_PORT, &GPIO_InitStruct);            	
		
 
 
 
 
 

}



uint8_t key_num[4];
uint8_t Key_num[4];





void Keypad_Handle(void)
{
#if(key_pad_Set)	
	Keypad_HandleKeyPress(Keypad_GetKey());	
#endif	
#if(button_Set)	
//	  Process_Button_Events();
#endif	
}	





float voltage,power;
float volMax = 2.5;
uint16_t current;
uint16_t currentMax = 180;

uint8_t keyNum[4],setn = 0;
u8 mode = 0,warningFlag = 0,load = 0,led=0;
uint8_t temperature=25;
uint8_t tempMax = 35;
char display[16]; 
uint8_t data_index = 0;
char str[50];


u8 elQuantity = 0; 
void GetBatteryCapacity(void)	 
{
	u8 Vol;
	Vol = (u16)(voltage * 1000) / 42;
	if(Vol >= 100)
		elQuantity = 100;	
	else{
		elQuantity =Vol;
	}
}



void KeyScanHandle(void)   
{
	 if(keyNum[0] == 1)  
	 {
		HAL_Delay(100);
		if(keyNum[0] == 1)
		{
			setn ++;
			if(setn <= 4)
			{
				OLED_CLS(); 
				DISplaysetvalue();
			}else{
				setn = 1;
				OLED_CLS();     
			}
			keyNum[0] = 0;
		}
	 }
	 if(keyNum[1] == 1)  
	 {
		HAL_Delay(100);
		if(keyNum[1] == 1)
		{
			if(setn == 2 && volMax < 25)volMax+=0.5;
			if(setn == 3 && currentMax< 3200)currentMax+=10;
			if(setn == 4 && tempMax < 99)tempMax++;
			DISplaysetvalue();    
			keyNum[1] = 0;
		}
	 }
	 if(keyNum[2] == 1)  
	 {
		HAL_Delay(100);
		if(keyNum[2] == 1)
		{
			if(setn == 1 && (warningFlag == 0))
			{
				HAL_Delay(50);
				load = !load;
				if(load == 0)
				{
				RELAY(OFF);
				LED(0);	
				}
				else
				{
				RELAY(ON);
				LED(1);	
				}					
			}
			if(setn == 2 && volMax > 0.5)volMax-=0.5;
			if(setn == 3 && currentMax > 10)currentMax-=10;
			if(setn == 4 && tempMax > 0)tempMax--;
			DISplaysetvalue();
				keyNum[2] = 0;
		}
	 }
}

void KeyScan(void) 
{
	if(KEY0 == 0)
	{
		keyNum[0] = 1;
	}
	if(KEY1 == 0)
	{
		keyNum[1] = 1;
	}
	if(KEY2 == 0)
	{
		keyNum[2]= 1;
	}
	if(KEY3 == 0)
	{
		keyNum[3]= 1;
	}
}
#endif






u16 last_current = 0;
u8 last_setn = 0; 


void Display(void)
{
    u16 curr;

    if(setn == 1)
    {
			
			
			  INA219a_Init(); 
        voltage = INA219_ReadBusVoltage(INA219_ADDRESS) / 1000.0f;
				GetBatteryCapacity();				
				OLED_ShowStr(0, 4, (uint8_t *)"Temp:", 2, 0);
				OLED_ShowNum(48,4,temperature,2,0);
				OLED_ShowCentigrade(70,4);
			
        OLED_ShowStr(0, 0, (uint8_t *)"Volt:", 2, 0);
				if(voltage < 0.1f)voltage=0;	
				OLED_ShowFormatStr(48, 0, "%.2f", voltage);
			
        OLED_ShowChar(90,0,'V',2,0);

											 
					OLED_ShowStr(84, 7, (uint8_t *)"     ", 1, 0);

					 
					if(elQuantity == 100)
					{
							OLED_DrawBMP(84, 7, 100, 8, 5);
					}
					else if(elQuantity >= 93)
					{
							OLED_DrawBMP(84, 7, 100, 8, 0);
					}
					else if(elQuantity >= 71)
					{
							OLED_DrawBMP(84, 7, 100, 8, 1);
					}
					else if(elQuantity >= 41)
					{
							OLED_DrawBMP(84, 7, 100, 8, 2);
					}
					else if(elQuantity >= 21)
					{
							OLED_DrawBMP(84, 7, 100, 8, 3);
					}
					else
					{
							OLED_DrawBMP(84, 7, 100, 8, 4);
					}

					 
					OLED_ShowNum(103, 7, elQuantity, 1, 0);
					OLED_ShowChar(119, 7, '%', 1, 0);
							
        current = INA219_ReadCurrent_mA(INA219_ADDRESS);
        OLED_ShowStr(0, 2, (uint8_t *)"Curr:", 2, 0);
        curr = current;
				
					
				if(curr != last_current || last_setn != 1)
				{
						 
						OLED_ShowChar(48, 2, ' ', 2, 0);  // 32+16
						OLED_ShowChar(58, 2, ' ', 2, 0);  // 42+16
						OLED_ShowChar(68, 2, ' ', 2, 0);  // 52+16

						 
						if(curr >= 100) OLED_ShowNum(48, 2, curr/100,        2, 0);
						if(curr >= 10)  OLED_ShowNum(58, 2, (curr%100)/10,  2, 0);
													 OLED_ShowNum(68, 2, curr%10,         2, 0);

						last_current = curr;
				}

				 
				OLED_ShowChar(80, 2, 'm', 2, 0);  // 64+16
				OLED_ShowChar(90, 2, 'A', 2, 0);  // 74+16
		
       temperature = DS18B20_Get_Temp()/10;
    }
    
    last_setn = setn;
}
u16 last_currentMax = 0;
void DISplaysetvalue(void)
{
	if(setn == 2) 
	{
		OLED_ShowStr(0, 0, (uint8_t *)"Low Voltage", 2, 0);
		OLED_ShowFormatStr(44, 3, "%.1f", volMax);
	  OLED_ShowChar(80,3,'V', 2,0);
	}		
	if(setn == 3)
	{
		OLED_ShowStr(0, 0, (uint8_t *)"High Current", 2, 0);
		if(currentMax != last_currentMax || last_setn != 3)
		{		
			OLED_ShowChar(44,3,' ',2,0);
			OLED_ShowChar(54,3,' ',2,0);
			OLED_ShowChar(64,3,' ',2,0);
			OLED_ShowChar(74,3,' ',2,0);
			
			if(currentMax>=1000)OLED_ShowNum(44,3,currentMax/1000,2,0);
			if(currentMax>=100) OLED_ShowNum(54,3,(currentMax%1000)/100,2,0);
			if(currentMax>=10)  OLED_ShowNum(64,3,(currentMax%100)/10,2,0);
			                   OLED_ShowNum(74,3,currentMax%10,2,0);	
			
			OLED_ShowChar(84,3,'m',2,0);
			OLED_ShowChar(94,3,'A',2,0);
			
			last_currentMax = currentMax;
			
		}
	}	
	
		if(setn == 4) 
	{
		OLED_ShowStr(0, 0, (uint8_t *)"High Temp", 2, 0);
		OLED_ShowNum(44,3,tempMax,2,0);
		OLED_ShowCentigrade(80,3);
	}		
	
}

void CotorFun(void)
{	

		if(setn == 1)
		{
			if(voltage < volMax)
				{
					BEEP_DOING();
					RELAY(OFF);
					LED(0);
					OLED_ShowStr(48,0,(uint8_t *)"         ",2,0);
					warningFlag |= (1 << 0); 
				}else{
					warningFlag &= ~(1 << 0); 
				}
				if(current > currentMax)
				{
					BEEP_DOING();
					RELAY(OFF);
					LED(0);
					OLED_ShowStr(48,2,(uint8_t *)"          ",2,0);	
					warningFlag |= (1 << 1); 
				}else{
					warningFlag &= ~(1 << 1); 
				}
				if(temperature > tempMax)
				{
					BEEP_DOING(); 
					RELAY(OFF);
					RELAY1(ON);
					LED(0);
					OLED_ShowStr(48,4,(uint8_t *)"          ",2,0);	
					warningFlag |= (1 << 2); 
				}else{
					warningFlag &= ~(1 << 2); 
					RELAY1(OFF);
				}
		}
}