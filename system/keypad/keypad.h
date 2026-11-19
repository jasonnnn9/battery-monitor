
#ifndef __KEYPAD_H__
#define __KEYPAD_H__
#include "main.h"

#define key_pad_Set  OFF
#define button_Set	 ON

 
#define ROW1_PIN    GPIO_PIN_0
#define ROW2_PIN    GPIO_PIN_1
#define ROW3_PIN    GPIO_PIN_2
#define ROW4_PIN    GPIO_PIN_3
#define ROW_PORT    GPIOA

 
#define COL1_PIN    GPIO_PIN_6
#define COL2_PIN    GPIO_PIN_7
#define COL3_PIN    GPIO_PIN_8
#define COL4_PIN    GPIO_PIN_9
#define COL_PORT    GPIOB







#define KEYPAD_ROWS 4
#define KEYPAD_COLS 4

 
#define KEY_NONE   0xFF
#define KEY_1      0
#define KEY_2      1
#define KEY_3      2
#define KEY_reg    3
#define KEY_4      4
#define KEY_5      5
#define KEY_6      6
#define KEY_del    7
#define KEY_7      8
#define KEY_8      9
#define KEY_9      10
#define KEY_fin    11
#define KEY_set    12
#define KEY_0      13
#define KEY_ent    14
#define KEY_can    15

 
typedef void (*KeyActionCallback)(void);

 
void Keypad_Init(void);
uint8_t Keypad_GetKey(void);
void KEY_Init(void);
 
void Keypad_RegisterCallback(uint8_t key, KeyActionCallback callback);
void Keypad_HandleKeyPress(uint8_t key);
void Keypad_Handle(void);
void Key1_Action(void);
void Key2_Action(void);
void Key3_Action(void);
void Key4_Action(void);

void Key5_Action(void);
void Key6_Action(void);
void Key7_Action(void);
void Key8_Action(void);

void Key9_Action(void);
void Key10_Action(void);
void Key11_Action(void);
void Key12_Action(void);


void Key13_Action(void);
void Key14_Action(void);
void Key15_Action(void);
void Key16_Action(void);



uint8_t Check_Password(void);
void Set_Password(void);



/******************************************************************************************/
 

#define KEY0_GPIO_PORT                  GPIOB
#define KEY0_GPIO_PIN                   GPIO_PIN_12
#define KEY0_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)    

#define KEY1_GPIO_PORT                  GPIOB
#define KEY1_GPIO_PIN                   GPIO_PIN_13
#define KEY1_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)    

#define KEY2_GPIO_PORT                  GPIOB
#define KEY2_GPIO_PIN                   GPIO_PIN_14
#define KEY2_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)    

#define KEY3_GPIO_PORT                  GPIOB
#define KEY3_GPIO_PIN                   GPIO_PIN_15
#define KEY3_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)    

//#define WKUP_GPIO_PORT                  GPIOC
//#define WKUP_GPIO_PIN                   GPIO_PIN_0
 

/******************************************************************************************/

#define KEY0        HAL_GPIO_ReadPin(KEY0_GPIO_PORT, KEY0_GPIO_PIN)      
#define KEY1        HAL_GPIO_ReadPin(KEY1_GPIO_PORT, KEY1_GPIO_PIN)      
#define KEY2        HAL_GPIO_ReadPin(KEY2_GPIO_PORT, KEY2_GPIO_PIN)      
#define KEY3        HAL_GPIO_ReadPin(KEY3_GPIO_PORT, KEY3_GPIO_PIN)      
 


#define KEY0_PRES    1               
#define KEY1_PRES    2               
#define KEY2_PRES    3               
#define KEY3_PRES    4               
#define WKUP_PRES    5               

void key_init(void);                 
uint8_t key_scan(uint8_t mode);      
void KeyScanHandle(void);   
void KeyScan(void); 
void UpdateMeasurements(void);
void Display(void);
void DISplaysetvalue(void);
void CotorFun(void);
extern uint8_t key_num[4];
extern uint8_t Key_num[4];
extern uint8_t setn;
#endif







