#include "main.h"
#include "keypad.h"

#if(key_pad_Set)

#define PASSWORD_LENGTH 6    

 
uint16_t default_password[PASSWORD_LENGTH] = {1, 2, 3, 4, 5, 6};   
uint16_t current_password[PASSWORD_LENGTH];   
uint16_t input_password[PASSWORD_LENGTH];   
uint8_t input_index = 0;   

uint8_t is_password_set = 0;   
uint8_t password_set_flag = 0;   

 
static const uint8_t Keypad_Map[KEYPAD_ROWS][KEYPAD_COLS] = {
	{KEY_reg,    KEY_7,    KEY_8,    KEY_9},
	{KEY_del,    KEY_4,    KEY_5,    KEY_6},
	{KEY_fin,    KEY_1,    KEY_2,    KEY_3},
	{KEY_set,    KEY_ent,  KEY_0,    KEY_can}
};

 
static uint8_t debounce_counter = 0;
static uint8_t last_key = KEY_NONE;
static uint8_t current_key = KEY_NONE;

 
static KeyActionCallback keyCallbacks[16] = {NULL};

 
void Keypad_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};


    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();


    GPIO_InitStruct.Pin = ROW1_PIN | ROW2_PIN | ROW3_PIN | ROW4_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ROW_PORT, &GPIO_InitStruct);


    GPIO_InitStruct.Pin = COL1_PIN | COL2_PIN | COL3_PIN | COL4_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;  
    HAL_GPIO_Init(COL_PORT, &GPIO_InitStruct);


    HAL_GPIO_WritePin(ROW_PORT, ROW1_PIN | ROW2_PIN | ROW3_PIN | ROW4_PIN, GPIO_PIN_SET);
}

 
uint8_t Keypad_Scan(void) 
{
    uint8_t row, col;
    const uint16_t ROW_PINS[4] = {ROW1_PIN, ROW2_PIN, ROW3_PIN, ROW4_PIN};
    const uint16_t COL_PINS[4] = {COL1_PIN, COL2_PIN, COL3_PIN, COL4_PIN};

    for (row = 0; row < 4; row++) {
         
        HAL_GPIO_WritePin(ROW_PORT, 
            ROW1_PIN | ROW2_PIN | ROW3_PIN | ROW4_PIN, 
            GPIO_PIN_SET);
        HAL_GPIO_WritePin(ROW_PORT, ROW_PINS[row], GPIO_PIN_RESET);
        
         
        for(volatile int i=0; i<50; i++);

         
        for (col = 0; col < 4; col++) {
             
            uint8_t sample = 0;
            for(int i=0; i<3; i++) {
                if(HAL_GPIO_ReadPin(COL_PORT, COL_PINS[col]) == GPIO_PIN_RESET) {
                    sample++;
                }
                for(volatile int j=0; j<10; j++);  
            }
            
             
            if(sample >= 2) {
                 
                HAL_GPIO_WritePin(ROW_PORT, ROW_PINS[row], GPIO_PIN_SET);
                return Keypad_Map[row][col];
            }
        }
        
         
        HAL_GPIO_WritePin(ROW_PORT, ROW_PINS[row], GPIO_PIN_SET);
    }
    return KEY_NONE;
}
uint8_t key;
 
uint8_t Keypad_GetKey(void) {
    key = Keypad_Scan();

    if (key != KEY_NONE) {
        if (key == last_key) {
            if (++debounce_counter >= 5) {   
                debounce_counter = 0;
                if (current_key != key) {
                    current_key = key;
                    return key;
                }
            }
        } else {
            debounce_counter = 0;
            last_key = key;
        }
    } else {
        debounce_counter = 0;
        last_key = KEY_NONE;
        current_key = KEY_NONE;
    }
    return KEY_NONE;
}

 
void Keypad_RegisterCallback(uint8_t key, KeyActionCallback callback) {
    if (key < 16) keyCallbacks[key] = callback;
}

void Keypad_HandleKeyPress(uint8_t key) {
    if (key != KEY_NONE && keyCallbacks[key] != NULL) {
        keyCallbacks[key]();
    }
		Set_Password();
		if(Check_Password())
		{
			
		}	
}




 
void Key1_Action(void) { 
    if (password_set_flag == 1 && input_index < PASSWORD_LENGTH) {
        input_password[input_index++] = 1;
    }
}
void Key2_Action(void) { 
    if (password_set_flag == 1 && input_index < PASSWORD_LENGTH) {
        input_password[input_index++] = 2;
    }
}
void Key3_Action(void) { 
    if (password_set_flag == 1 && input_index < PASSWORD_LENGTH) {
        input_password[input_index++] = 3;
    }
}
void Key4_Action(void) { 
    if (password_set_flag == 1 && input_index < PASSWORD_LENGTH) {
        input_password[input_index++] = 4;
    }
}
void Key5_Action(void) { 
    if (password_set_flag == 1 && input_index < PASSWORD_LENGTH) {
        input_password[input_index++] = 5;
    }
}
void Key6_Action(void) { 
    if (password_set_flag == 1 && input_index < PASSWORD_LENGTH) {
        input_password[input_index++] = 6;
    }
}
void Key7_Action(void) { 
    if (password_set_flag == 1 && input_index < PASSWORD_LENGTH) {
        input_password[input_index++] = 7;
    }
}
void Key8_Action(void) { 
    if (password_set_flag == 1 && input_index < PASSWORD_LENGTH) {
        input_password[input_index++] = 8;
    }
}
void Key9_Action(void) { 
    if (password_set_flag == 1 && input_index < PASSWORD_LENGTH) {
        input_password[input_index++] = 9;
    }
}
void Key10_Action(void) { 
    if (password_set_flag == 1 && input_index < PASSWORD_LENGTH) {
        input_password[input_index++] = 0;
    }
}

 
void Key11_Action(void) 
{ 
    if(is_password_set == 0 && password_set_flag == 0) 
		{   
        password_set_flag = 1;   
        input_index = 0;   
        memset(input_password, 0, sizeof(input_password));   
         
    }
}

 
void Key12_Action(void) 
{ 
    if(is_password_set == 0 && password_set_flag == 0)
		{
        password_set_flag = 1;
        input_index = 0;
        memset(input_password, 0, sizeof(input_password));
    }
}

 
void Key13_Action(void)
{ 
    if (is_password_set == 0 && password_set_flag == 0) {
        password_set_flag = 1;
        input_index = 0;
        memset(input_password, 0, sizeof(input_password));
    }
}

 
void Key14_Action(void) { 
    if (is_password_set == 0 && password_set_flag == 0) {
        password_set_flag = 1;
        input_index = 0;
        memset(input_password, 0, sizeof(input_password));
    }
}

 
void Key15_Action(void) 
{
    if (password_set_flag == 1 && input_index == PASSWORD_LENGTH) {
        Set_Password();   
    } else {
         
    }
}

 
void Key16_Action(void) 
{
    if(input_index > 0)
		{
			input_index--;   
			input_password[input_index] = 0;   
    }
}



 
void Set_Password(void) 
{
    if (input_index == PASSWORD_LENGTH) 
		{
        memcpy(current_password, input_password, PASSWORD_LENGTH * sizeof(uint16_t));   
        is_password_set = 1;   
        password_set_flag = 0;   
         
    }
}

 
uint8_t Check_Password(void) 
{
     
    uint16_t* password_to_check = (is_password_set) ? current_password : default_password;

     
    for (uint8_t i = 0; i < PASSWORD_LENGTH; i++) 
	  {
        if (input_password[i] != password_to_check[i]) {
            return 0;   
        }
    }
    return 1;   
}


void KEY_Init(void)
{
	  Keypad_Init();
    Keypad_RegisterCallback(KEY_1, Key1_Action);
    Keypad_RegisterCallback(KEY_2, Key2_Action);
		Keypad_RegisterCallback(KEY_3, Key3_Action);	
		Keypad_RegisterCallback(KEY_4, Key4_Action);	

    Keypad_RegisterCallback(KEY_5, Key5_Action);
    Keypad_RegisterCallback(KEY_6, Key6_Action);
		Keypad_RegisterCallback(KEY_7, Key7_Action);	
		Keypad_RegisterCallback(KEY_8, Key8_Action);	

    Keypad_RegisterCallback(KEY_9, Key9_Action);
    Keypad_RegisterCallback(KEY_0, Key10_Action);
		Keypad_RegisterCallback(KEY_reg, Key11_Action);	   
		Keypad_RegisterCallback(KEY_del, Key12_Action);	   

    Keypad_RegisterCallback(KEY_fin, Key13_Action);    
    Keypad_RegisterCallback(KEY_set, Key14_Action);    
		Keypad_RegisterCallback(KEY_ent, Key15_Action);	   
		Keypad_RegisterCallback(KEY_can, Key16_Action);		 
	
}	
  


#endif



#if(button_Set)


 
#define DEBOUNCE_TIME       15
#define SHORT_CLICK_TIME    300
#define LONG_HOLD_TIME      2000

 
typedef enum {
    BTN_STATE_RELEASED = 0,
    BTN_STATE_DEBOUNCE,
    BTN_STATE_PRESSED,
    BTN_STATE_WAIT_LONG
} ButtonState;

 
typedef struct {
    ButtonState state;
    uint32_t press_tick;
    uint8_t long_triggered;
} ButtonCB;

 
static ButtonCB button_cb[4] = {0};  

 
void KEY_Init(void) 
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
#if(Set_button_1)
    Button1_GPIO_CLK_ENABLE();
    GPIO_InitStruct.Pin = Button1_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;  
    HAL_GPIO_Init(Button1_GPIO_PORT, &GPIO_InitStruct);
#endif

#if(Set_button_2)
    BUTTON2_GPIO_CLK_ENABLE();
    GPIO_InitStruct.Pin = BUTTON2_GPIO_PIN;
    HAL_GPIO_Init(BUTTON2_GPIO_PORT, &GPIO_InitStruct);
#endif

#if(Set_button_3)
    BUTTON3_GPIO_CLK_ENABLE();
    GPIO_InitStruct.Pin = BUTTON3_GPIO_PIN;
    HAL_GPIO_Init(BUTTON3_GPIO_PORT, &GPIO_InitStruct);
#endif

#if(Set_button_4)
    BUTTON4_GPIO_CLK_ENABLE();
    GPIO_InitStruct.Pin = BUTTON4_GPIO_PIN;
    HAL_GPIO_Init(BUTTON4_GPIO_PORT, &GPIO_InitStruct);
#endif
}

 
uint8_t Button_GetState(uint8_t button_id)
{
    GPIO_PinState pin_state;
    
    switch(button_id) {
#if(Set_button_1)
        case 1:
            pin_state = HAL_GPIO_ReadPin(Button1_GPIO_PORT, Button1_GPIO_PIN);
            return (pin_state == GPIO_PIN_RESET) ? 1 : 0;
#endif
#if(Set_button_2)
        case 2:
            pin_state = HAL_GPIO_ReadPin(BUTTON2_GPIO_PORT, BUTTON2_GPIO_PIN);
            return (pin_state == GPIO_PIN_RESET) ? 1 : 0;
#endif
#if(Set_button_3)
        case 3:
            pin_state = HAL_GPIO_ReadPin(BUTTON3_GPIO_PORT, BUTTON3_GPIO_PIN);
            return (pin_state == GPIO_PIN_RESET) ? 1 : 0;
#endif
#if(Set_button_4)
        case 4:
            pin_state = HAL_GPIO_ReadPin(BUTTON4_GPIO_PORT, BUTTON4_GPIO_PIN);
            return (pin_state == GPIO_PIN_RESET) ? 1 : 0;
#endif
        default:
            return 0;
    }
}

 
ButtonEvent Button_GetEvent(uint8_t button_id) 
{
    if(button_id == 0 || button_id > 4) return BTN_EVENT_NONE;
    
    uint8_t idx = button_id - 1;
    ButtonCB* cb = &button_cb[idx];
    uint32_t now = HAL_GetTick();
    uint8_t is_pressed = Button_GetState(button_id);

    switch(cb->state) {
        case BTN_STATE_RELEASED:
            if(is_pressed) {
                cb->state = BTN_STATE_DEBOUNCE;
                cb->press_tick = now;
            }
            break;

        case BTN_STATE_DEBOUNCE:
            if((now - cb->press_tick) >= DEBOUNCE_TIME) {
                if(is_pressed) {
                    cb->state = BTN_STATE_PRESSED;
                    return BTN_EVENT_PRESS_DOWN;
                } else {
                    cb->state = BTN_STATE_RELEASED;
                }
            }
            break;

        case BTN_STATE_PRESSED:
            if(!is_pressed) {
                cb->state = BTN_STATE_RELEASED;
                return ((now - cb->press_tick) < SHORT_CLICK_TIME) ? 
                       BTN_EVENT_SHORT_CLICK : BTN_EVENT_LONG_CLICK;
            } 
            else if((now - cb->press_tick) >= LONG_HOLD_TIME) {
                cb->state = BTN_STATE_WAIT_LONG;
                cb->long_triggered = 1;
                return BTN_EVENT_LONG_HOLD;
            }
            break;

        case BTN_STATE_WAIT_LONG:
            if(!is_pressed) {
                cb->state = BTN_STATE_RELEASED;
                cb->long_triggered = 0;
                return BTN_EVENT_LONG_CLICK;
            }
            break;
    }

    return BTN_EVENT_NONE;
}


uint16_t key_event[5];
void Process_Button_Events(void)
{
#if(Set_button_1)
    ButtonEvent evt1 = Button_GetEvent(1);
    if(evt1 == BTN_EVENT_SHORT_CLICK)
		{
			key_event[1]++;
			if(key_event[1]>=2)key_event[1]=0;
		}			
//    if(evt1 == BTN_EVENT_LONG_HOLD) ;
#endif

#if(Set_button_2)
    ButtonEvent evt2 = Button_GetEvent(2);
     
#endif
}

#endif




void Keypad_Handle(void)
{
#if(key_pad_Set)	
	Keypad_HandleKeyPress(Keypad_GetKey());	
#endif	
#if(button_Set)	
	  Process_Button_Events();
#endif	
}	



#if(1)	
 
 



uint8_t Key_Scan(void) 
{
    static uint8_t key_up = 1;   

    if (key_up && (HAL_GPIO_ReadPin(Button1_GPIO_PORT, Button1_GPIO_PIN) == GPIO_PIN_RESET)) 
			{
        HAL_Delay(10);   
        key_up = 0;       
        if (HAL_GPIO_ReadPin(Button1_GPIO_PORT, Button1_GPIO_PIN) == GPIO_PIN_RESET) 
				{
            return 1;     
        }
      } 
			else if (HAL_GPIO_ReadPin(Button1_GPIO_PORT, Button1_GPIO_PIN) == GPIO_PIN_SET)
				{
        key_up = 1;       
        }
    return 0;
}




 
uint8_t Button_GetState(void)
{
	uint8_t state = HAL_GPIO_ReadPin(Button1_GPIO_PORT, Button1_GPIO_PIN);
	
	if(state != last_state) 
	{
		debounce_counter++;
		if(debounce_counter >= 5) {   
			debounce_counter = 0;
			last_state = state;
			current_state = state;
		}
	}else{
		debounce_counter = 0;
	}
	
	return current_state;
}

 
uint8_t Button_GetEvent(void)
{
	static uint8_t last_btn_state = BUTTON_RELEASED;
	uint8_t btn_state = Button_GetState();
	uint8_t event = BTN_EVENT_NONE;
	
	 
	if (btn_state != last_btn_state) {
		if (btn_state == BUTTON_PRESSED) {
			 
			event = BTN_EVENT_PRESSED;
			press_start_time = HAL_GetTick();
			long_press_detected = 0;
		} else {
			 
			event = BTN_EVENT_RELEASED;
			if (!long_press_detected && (HAL_GetTick() - press_start_time < 1000)) {
				event = BTN_EVENT_SHORT_PRESS;
			}
		}
		last_btn_state = btn_state;
	} 
	 
	else if (btn_state == BUTTON_PRESSED && !long_press_detected) {
		if (HAL_GetTick() - press_start_time >= 1000) {   
			event = BTN_EVENT_LONG_PRESS;
			long_press_detected = 1;
		}
	}
	
	return event;
}

uint16_t key_x;
 



void Handle_ButtonEvent(uint8_t btn_event)
{
    switch (btn_event) {
        case BTN_EVENT_PRESSED:   
						key_x=1;
            break;
        
        case BTN_EVENT_RELEASED:	 
            key_x=2;
            break;
        
        case BTN_EVENT_SHORT_PRESS:  
             key_x=3;
            
            break;
        
        case BTN_EVENT_LONG_PRESS:	 
             key_x=4;
             
            break;
        
        default:
            break;
    }
}
#endif	