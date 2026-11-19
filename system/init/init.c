#include "main.h"
#include "init.h"
 

extern uint16_t speter_flag;

static volatile uint8_t buzzerReady;
static volatile uint8_t buzzerAlarm;
static volatile uint8_t buzzerFeedbackRequested;

 
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
		

}	

void GPIO_BEEP_Init(void)	
{	
		GPIO_InitTypeDef GPIO_InitStruct;
		BEEP_GPIO_CLK_ENABLE();
		GPIO_InitStruct.Pin = BEEP_GPIO_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		BEEP(Lo);
        HAL_GPIO_Init(BEEP_GPIO_PORT, &GPIO_InitStruct);
        buzzerReady = 1;
}	


void BEEP_DOING(void)
{
    buzzerFeedbackRequested = 1;
}

void Buzzer_SetAlarm(uint8_t active)
{
    buzzerAlarm = (active != 0);
}

/* Called only by TIM2; foreground display/UART delays cannot stretch the pulse. */
void Buzzer_Update(void)
{
    static uint8_t alarmWasActive;
    static uint8_t feedbackActive;
    static uint32_t alarmStarted;
    static uint32_t feedbackStarted;
    uint32_t now = HAL_GetTick();
    uint8_t alarm = buzzerAlarm;
    uint8_t output = 0;

    if (!buzzerReady)
        return;
    if (alarm)
    {
        if (!alarmWasActive)
            alarmStarted = now;
        output = ((uint32_t)(now - alarmStarted) % 500u) < 200u;
        feedbackActive = 0;
        buzzerFeedbackRequested = 0;
    }
    else
    {
        if (alarmWasActive)
            buzzerFeedbackRequested = 0;
        if (buzzerFeedbackRequested)
        {
            buzzerFeedbackRequested = 0;
            feedbackStarted = now;
            feedbackActive = 1;
        }
        if (feedbackActive && (uint32_t)(now - feedbackStarted) >= 200u)
            feedbackActive = 0;
        output = feedbackActive;
    }
    alarmWasActive = alarm;
#if BEEP_ENABLED
    BEEP(output);
#else
    (void)output;
    BEEP(Lo);
#endif
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