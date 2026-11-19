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
uint16_t current = INA219_CURRENT_INVALID;
uint16_t currentMax = 180;

uint8_t keyNum[4],setn = 0;
enum {
    WARNING_LOW_VOLTAGE = 1u << 0,
    WARNING_HIGH_CURRENT = 1u << 1,
    WARNING_HIGH_TEMP = 1u << 2,
    WARNING_CURRENT_SENSOR = 1u << 3
};
u8 mode = 0, warningFlag = WARNING_CURRENT_SENSOR, load = 0, led = 0;
uint8_t temperature=25;
uint8_t tempMax = 35;
char display[16]; 
uint8_t data_index = 0;
char str[50];


u8 elQuantity = 0; 
void GetBatteryCapacity(void)	 
{
	uint16_t Vol;
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
			if(setn == 2 && volMax > 0.5)volMax-=0.5;
			if(setn == 3 && currentMax > 10)currentMax-=10;
			if(setn == 4 && tempMax > 0)tempMax--;
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
				}
				else
				{
				RELAY(ON);
				}					
			}
			if(setn == 2 && volMax < 25)volMax+=0.5;
			if(setn == 3 && currentMax < 3200)currentMax+=10;
			if(setn == 4 && tempMax < 99)tempMax++;
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
    if(setn == 1)
    {
			
			
        voltage = ADC_ReadBatteryVoltage();
				GetBatteryCapacity();				
				OLED_ShowStr(0, 4, (uint8_t *)"Temp:", 2, 0);
				OLED_ShowNum(48,4,temperature,2,0);
				OLED_ShowCentigrade(70,4);
			
        OLED_ShowStr(0, 0, (uint8_t *)"Volt:", 2, 0);
				if(voltage < 0.1f)voltage=0;	
				OLED_ShowFormatStr(48, 0, "%-5.2f", voltage);
			
        OLED_ShowChar(90,0,'V',2,0);

											 
					OLED_ShowStr(84, 7, (uint8_t *)"     ", 1, 0);

					 
					if(elQuantity >= 93)
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

					 
					char batteryText[5];
                    snprintf(batteryText, sizeof batteryText, "%3u%%", (unsigned)elQuantity);
                    OLED_ShowStr(103, 7, (uint8_t *)batteryText, 1, 0);
							
        current = INA219_ReadCurrent_mA(INA219_ADDRESS);
        OLED_ShowStr(0, 2, (uint8_t *)"Curr:", 2, 0);
        if (current != last_current || last_setn != 1)
        {
            if (current == INA219_CURRENT_INVALID)
                OLED_ShowStr(48, 2, (uint8_t *)" ERR", 2, 0);
            else
                OLED_ShowFormatStr(48, 2, "%4u", (unsigned)current);
            last_current = current;
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
    if (setn != 1)
        return;

    warningFlag = 0;
    if (voltage < volMax)
        warningFlag |= WARNING_LOW_VOLTAGE;
    if (current == INA219_CURRENT_INVALID)
        warningFlag |= WARNING_CURRENT_SENSOR;
    else if (current > currentMax)
        warningFlag |= WARNING_HIGH_CURRENT;
    if (temperature > tempMax)
        warningFlag |= WARNING_HIGH_TEMP;

    if (warningFlag)
    {
        RELAY(OFF);
        load = 0;
    }
    LED(warningFlag != 0);
    RELAY1((warningFlag & WARNING_HIGH_TEMP) != 0);
    if (warningFlag & (WARNING_LOW_VOLTAGE | WARNING_HIGH_CURRENT | WARNING_HIGH_TEMP))
        BEEP_DOING();
}