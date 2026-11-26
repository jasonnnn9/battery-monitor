#include "main.h"
#include "keypad.h"
#include "bms_config.h"
#include "bms_control.h"

#if(button_Set)


 




static volatile uint8_t keysReady;

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
    keysReady = 1;
		
 
 
 
 
 

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

uint8_t setn = 1;
static uint8_t protectionEnabled;
enum {
    WARNING_LOW_VOLTAGE = 1u << 0,
    WARNING_HIGH_CURRENT = 1u << 1,
    WARNING_HIGH_TEMP = 1u << 2,
    WARNING_CURRENT_SENSOR = 1u << 3
};
u8 mode = 0, load = 0, led = 0;
volatile u8 warningFlag = WARNING_CURRENT_SENSOR;
uint8_t temperature=25;
uint8_t tempMax = 35;

static float editVolMax;
static uint16_t editCurrentMax;
static uint8_t editTempMax;
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



enum { KEY_DEBOUNCE_MS = 25 };

static volatile uint8_t keyEvents[3];

static void AdvanceSettings(void)
{
    uint8_t result = BMS_ACK_APPLIED;
    uint32_t mv = (uint32_t)(volMax * 1000.0f + 0.5f);
    if (setn == 2)
        result = BmsConfig_Apply((uint32_t)(editVolMax * 1000.0f + 0.5f), currentMax, tempMax);
    else if (setn == 3)
        result = BmsConfig_Apply(mv, editCurrentMax, tempMax);
    else if (setn == 4)
        result = BmsConfig_Apply(mv, currentMax, editTempMax);
    if (result != BMS_ACK_APPLIED) {
        OLED_ShowStr(0, 7, (uint8_t *)"SAVE ERROR", 1, 0);
        return;
    }

    setn = setn == 4 ? 1 : setn + 1;
    if (setn == 2)
        editVolMax = volMax;
    else if (setn == 3)
        editCurrentMax = currentMax;
    else if (setn == 4)
        editTempMax = tempMax;

    CotorFun();
    OLED_CLS();
    DISplaysetvalue();
    if (setn != 1)
        OLED_ShowStr(0, 5, (uint8_t *)"S1- S2 NEXT S3+", 1, 0);
}

static void AdjustSetting(int direction)
{
    if (setn == 2) {
        editVolMax += direction * 0.5f;
        if (editVolMax < 0.5f) editVolMax = 0.5f;
        if (editVolMax > 16.5f) editVolMax = 16.5f;
    } else if (setn == 3) {
        int next = editCurrentMax + direction * 10;
        editCurrentMax = next < 10 ? 10 : next > 3200 ? 3200 : next;
    } else if (setn == 4) {
        int next = editTempMax + direction;
        editTempMax = next < 0 ? 0 : next > 99 ? 99 : next;
    }
    DISplaysetvalue();
}

void KeyScanHandle(void)
{
    uint32_t primask = __get_PRIMASK();
    __disable_irq();
    uint8_t s1 = keyEvents[0];
    uint8_t s2 = keyEvents[1];
    uint8_t s3 = keyEvents[2];
    keyEvents[0] = keyEvents[1] = keyEvents[2] = 0;
    __set_PRIMASK(primask);

    if (s1 && setn != 1) {
        AdjustSetting(-1);
    } else if (s1) {
        protectionEnabled = !protectionEnabled;
        CotorFun();
    }

    if (s3 && setn != 1) {
        AdjustSetting(1);
    } else if (s3 && (load || warningFlag == 0)) {
        load = !load;
        RELAY(load != 0);
    }

    if (s2)
        AdvanceSettings();
}

void KeyScan(void)
{
    static uint8_t lastRaw;
    static uint8_t stable;
    static uint32_t changedAt[3];
    if (!keysReady)
        return;

    uint32_t now = HAL_GetTick();
    uint8_t raw = (KEY0 == 0) | ((KEY1 == 0) << 1) | ((KEY2 == 0) << 2);
    for (unsigned i = 0; i < 3; ++i) {
        uint8_t bit = 1u << i;
        if ((raw ^ lastRaw) & bit)
            changedAt[i] = now;
        if (!((raw ^ stable) & bit) || now - changedAt[i] < KEY_DEBOUNCE_MS)
            continue;
        stable ^= bit;
        if (!(stable & bit))
            keyEvents[i] = 1;
    }
    lastRaw = raw;
}
#endif







u8 last_setn = 0; 


static INA219_Status currentStatus = INA219_LINK_ERROR;

void UpdateMeasurements(void)
{
    voltage = ADC_ReadBatteryVoltage();
    if (voltage < 0.1f)
        voltage = 0;
    currentStatus = INA219_ReadSample(INA219_ADDRESS, &current);
    temperature = DS18B20_Get_Temp() / 10;
    GetBatteryCapacity();
}

void Display(void)
{
    if(setn == 1)
    {
			
			
				OLED_ShowStr(0, 4, (uint8_t *)"Temp:", 2, 0);
				OLED_ShowNum(48,4,temperature,2,0);
				OLED_ShowCentigrade(70,4);
			
        OLED_ShowStr(0, 0, (uint8_t *)"Volt:", 2, 0);
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
							
        OLED_ShowStr(0, 2, (uint8_t *)"Curr:", 2, 0);
        /* Pad the whole field so changing from a fault to a number clears it. */
        static const char *const faultText[] = {
            [INA219_LINK_ERROR] = "I2C   ", [INA219_CONFIG_ERROR] = "CONFIG",
            [INA219_NOT_READY] = "WAIT  ", [INA219_OVERFLOW] = "OVF   "
        };
        if (currentStatus == INA219_OK)
            OLED_ShowFormatStr(48, 2, "%4u mA", (unsigned)current);
        else
            OLED_ShowFormatStr(48, 2, "%-7s", faultText[currentStatus]);

		
    }
    
    OLED_ShowStr(0, 6, (uint8_t *)(protectionEnabled ? "P:ON " : "P:OFF"), 1, 0);
    OLED_ShowStr(45, 6, (uint8_t *)(load ? "L:ON " : "L:OFF"), 1, 0);
    last_setn = setn;
}
u16 last_currentMax = 0;
void DISplaysetvalue(void)
{
	if(setn == 2) 
	{
		OLED_ShowStr(0, 0, (uint8_t *)"Low Voltage", 2, 0);
		OLED_ShowFormatStr(44, 3, "%.1f", editVolMax);
	  OLED_ShowChar(80,3,'V', 2,0);
	}		
	if(setn == 3)
	{
		OLED_ShowStr(0, 0, (uint8_t *)"High Current", 2, 0);
		if(editCurrentMax != last_currentMax || last_setn != 3)
		{		
			OLED_ShowChar(44,3,' ',2,0);
			OLED_ShowChar(54,3,' ',2,0);
			OLED_ShowChar(64,3,' ',2,0);
			OLED_ShowChar(74,3,' ',2,0);
			
			if(editCurrentMax>=1000)OLED_ShowNum(44,3,editCurrentMax/1000,2,0);
			if(editCurrentMax>=100) OLED_ShowNum(54,3,(editCurrentMax%1000)/100,2,0);
			if(editCurrentMax>=10)  OLED_ShowNum(64,3,(editCurrentMax%100)/10,2,0);
			                   OLED_ShowNum(74,3,editCurrentMax%10,2,0);
			
			OLED_ShowChar(84,3,'m',2,0);
			OLED_ShowChar(94,3,'A',2,0);
			
			last_currentMax = editCurrentMax;
			
		}
	}	
	
		if(setn == 4) 
	{
		OLED_ShowStr(0, 0, (uint8_t *)"High Temp", 2, 0);
		OLED_ShowNum(44,3,editTempMax,2,0);
		OLED_ShowCentigrade(80,3);
	}		
	
}

void CotorFun(void)
{
    u8 warnings = 0;
    if (protectionEnabled && voltage < volMax)
        warnings |= WARNING_LOW_VOLTAGE;
    if (current == INA219_CURRENT_INVALID)
        warnings |= WARNING_CURRENT_SENSOR;
    else if (protectionEnabled && current > currentMax)
        warnings |= WARNING_HIGH_CURRENT;
    if (protectionEnabled && temperature > tempMax)
        warnings |= WARNING_HIGH_TEMP;


    warningFlag = warnings;
    if (warningFlag)
    {
        RELAY(OFF);
        load = 0;
    }
    RELAY1((warningFlag & WARNING_HIGH_TEMP) != 0);
    Buzzer_SetAlarm(warningFlag & (WARNING_LOW_VOLTAGE | WARNING_HIGH_CURRENT | WARNING_HIGH_TEMP));
}