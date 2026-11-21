#include "main.h"
#include "usart.h"
#include <stdarg.h>

#define UARTx_Handler UART1_Handler
#define GPSRec	  0               
#define as608 		0
#define air780Rec 0
#define esp32Rec  0
#define esp8266 		1
extern TIM_HandleTypeDef htim1,htim2,htim3,htim4;

 

u16 UART1_Rx_cnt=0,UART2_Rx_cnt=0,UART3_Rx_cnt=0;  
UART_HandleTypeDef UART1_Handler,UART2_Handler,UART3_Handler;

#define RXBUFFERSIZE   1  

u8 USART1Buf[RXBUFFERSIZE]; 
u8 USART2Buf[RXBUFFERSIZE]; 
u8 USART3Buf[RXBUFFERSIZE]; 

u8 U1Buffer[USART_REC_LEN]; 
u8 U2Buffer[USART_REC_LEN]; 
u8 U3Buffer[USART_REC_LEN]; 

u8 Send_answer = 0;

u8 UART3_Rx_Do;


 
 
void UART_Setinit(uint32_t bound1,uint32_t bound2,uint32_t bound3)
{	
	 
#if(Enable_UART1)	
    UART1_Handler.Instance = USART1;
    UART1_Handler.Init.BaudRate = bound1;                    
    UART1_Handler.Init.WordLength = UART_WORDLENGTH_8B;      
    UART1_Handler.Init.StopBits = UART_STOPBITS_1;           
    UART1_Handler.Init.Parity = UART_PARITY_NONE;            
    UART1_Handler.Init.Mode = UART_MODE_TX_RX;               
    UART1_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;      
//	HAL_UART_DeInit(&UART1_Handler);
    HAL_UART_Init(&UART1_Handler);
	__HAL_UART_ENABLE_IT(&UART1_Handler,UART_IT_RXNE);
	
    HAL_UART_Receive_IT(&UART1_Handler,(u8 *)USART1Buf, 1);
#endif	
	
#if(Enable_UART2)		
		UART2_Handler.Instance = USART2;
		UART2_Handler.Init.BaudRate = bound2;                   
		UART2_Handler.Init.WordLength = UART_WORDLENGTH_8B;     
		UART2_Handler.Init.StopBits = UART_STOPBITS_1;           
		UART2_Handler.Init.Parity = UART_PARITY_NONE;           
		UART2_Handler.Init.Mode = UART_MODE_TX_RX;              
		UART2_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;      
//  HAL_UART_DeInit(&UART2_Handler);
		HAL_UART_Init(&UART2_Handler);
	__HAL_UART_ENABLE_IT(&UART2_Handler,UART_IT_RXNE);

		HAL_UART_Receive_IT(&UART2_Handler, (u8 *)USART2Buf, 1);
#endif	

#if(Enable_UART3)	
		UART3_Handler.Instance = USART3;
		UART3_Handler.Init.BaudRate = bound3;                   
		UART3_Handler.Init.WordLength = UART_WORDLENGTH_8B;     
		UART3_Handler.Init.StopBits = UART_STOPBITS_1;            
		UART3_Handler.Init.Parity = UART_PARITY_NONE;           
		UART3_Handler.Init.Mode = UART_MODE_TX_RX;              
		UART3_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;     
//	HAL_UART_DeInit(&UART3_Handler);
		HAL_UART_Init(&UART3_Handler);
	__HAL_UART_ENABLE_IT(&UART3_Handler,UART_IT_RXNE);
	
		HAL_UART_Receive_IT(&UART3_Handler, (u8 *)USART3Buf, 1);
#endif


}



void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
     
	GPIO_InitTypeDef GPIO_Initure;
#if(Enable_UART1)		
	if(huart->Instance==USART1)//USB
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();						 
		__HAL_RCC_USART1_CLK_ENABLE();					 
	
		GPIO_Initure.Pin=GPIO_PIN_9;						//PA9
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;			 
		GPIO_Initure.Pull=GPIO_PULLUP;					 
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH; 
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	 

		GPIO_Initure.Pin=GPIO_PIN_10;						//PA10
		GPIO_Initure.Mode=GPIO_MODE_AF_INPUT;			//PA10                     
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	 
		

		HAL_NVIC_EnableIRQ(USART1_IRQn);				 
		HAL_NVIC_SetPriority(USART1_IRQn,3,3);	 
	}	
#endif		
	
#if(Enable_UART2)	
		if(huart->Instance==USART2)								//SYS
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();							 
		__HAL_RCC_USART2_CLK_ENABLE();						 
	
		GPIO_Initure.Pin=GPIO_PIN_2;							//PA2
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;				 
		GPIO_Initure.Pull=GPIO_PULLUP;						 
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;	 
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   		 

		GPIO_Initure.Pin=GPIO_PIN_3;							//PA3
		GPIO_Initure.Mode=GPIO_MODE_AF_INPUT;			//PA3
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   		 
		

		HAL_NVIC_EnableIRQ(USART2_IRQn);					 
		HAL_NVIC_SetPriority(USART2_IRQn,3,2);		 
	}
#endif		
	
#if(Enable_UART3)		
		if(huart->Instance==USART3)								//WIFI
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();							 
		__HAL_RCC_USART3_CLK_ENABLE();						 
	
		GPIO_Initure.Pin=GPIO_PIN_10;							//PB10
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;				 
		GPIO_Initure.Pull=GPIO_PULLUP;						 
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;	 
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   		 

		GPIO_Initure.Pin=GPIO_PIN_11;							//PB11
		GPIO_Initure.Mode = GPIO_MODE_INPUT; 			 
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   		 
		

		HAL_NVIC_EnableIRQ(USART3_IRQn);					 
		HAL_NVIC_SetPriority(USART3_IRQn,3,1);		 
	}
#endif		
	
}


 
 
 
void USART1_IRQHandler(void)                	
{ 
	HAL_UART_IRQHandler(&UART1_Handler);	
} 
//uart
void USART2_IRQHandler(void)      	
{ 	
	HAL_UART_IRQHandler(&UART2_Handler);		
} 
//485
void USART3_IRQHandler(void)                	
{ 
	HAL_UART_IRQHandler(&UART3_Handler);
} 


void USART1_send(u8* buf,u8 len)
{
	HAL_UART_Transmit(&UART1_Handler,buf,len,0xffff);
}	

void USART2_send(u8* buf,u8 len)
{
	HAL_UART_Transmit(&UART2_Handler,buf,len,0xffff);
}

void USART3_send(u8* buf,u8 len)
{
	HAL_UART_Transmit(&UART3_Handler,buf,len,0xffff);
}

#define FRAME_HEADER      0x9C
#define FRAME_MIN_LENGTH  14       
#define UART1_BUFFER_SIZE 128     

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	
#if(esp32Rec)	
 
	if(huart->Instance==USART3)
	{
			if (UART3_Rx_cnt < USART_REC_LEN - 1) 
				{
        U3Buffer[UART3_Rx_cnt] = *USART3Buf;
        UART3_Rx_cnt++;
        
         
        if (*USART3Buf == '\n') 
				{
            U3Buffer[UART3_Rx_cnt] = '\0';   
            UART3_Rx_Do = 1;         
        }
				} else {
         
        UART3_Rx_cnt = 0;
    }
    
    HAL_UART_Receive_IT(&UART3_Handler, USART3Buf, 1);
  }
#endif	
	
	
#if(air780Rec)	
 
	if(huart->Instance==USART2)
	{
			if(UART2_Rx_cnt<1024)		 
		{
			U2Buffer[UART2_Rx_cnt]=*USART2Buf;		 
			++UART2_Rx_cnt;			
		} 
		HAL_UART_Receive_IT(&UART2_Handler,(u8 *)USART2Buf,1);	
  }
#endif


#if(ZigebRec)
// Zigbee receive interrupt handling.
        uint8_t receivedByte = *USART1Buf;
        
         
        if(UART1_Rx_cnt == 0 && receivedByte != FRAME_HEADER)
        {
            HAL_UART_Receive_IT(&UART1_Handler, (u8 *)USART1Buf, 1);
            return;
        }
        
         
        U1Buffer[UART1_Rx_cnt] = receivedByte;
        UART1_Rx_cnt++;
        
         
        if(UART1_Rx_cnt >= FRAME_MIN_LENGTH)
        {
            UR1_flg = 1;
            UART1_Rx_cnt = 0;
        }
        else if(UART1_Rx_cnt >= UART1_BUFFER_SIZE)
        {
            UART1_Rx_cnt = 0;
        }
        
         
        HAL_UART_Receive_IT(&UART1_Handler, (u8 *)USART1Buf, 1);
    }
#endif
#if(as608)
	if(huart->Instance==USART1)                	 
{
		U1Buffer[UART1_Rx_cnt]=*USART1Buf;
		++UART1_Rx_cnt;
		if(UART1_Rx_cnt>255)UART1_Rx_cnt=0;
		HAL_UART_Receive_IT(&UART1_Handler,(u8 *)USART1Buf,1);		
} 
#endif
#if(esp8266)
	if(huart->Instance==USART1)                	 
{
		#if ESP8266_THINGER_BRIDGE
        ESP8266_RxByte(*USART1Buf);
#else
        U1Buffer[UART1_Rx_cnt]=*USART1Buf;
		++UART1_Rx_cnt;
		if(UART1_Rx_cnt>255)UART1_Rx_cnt=0;
#endif
		HAL_UART_Receive_IT(&UART1_Handler,(u8 *)USART1Buf,1);		
} 
#endif
 
//{
//		U1Buffer[UART1_Rx_cnt]=*USART1Buf;
//		++UART1_Rx_cnt;
//		if(UART1_Rx_cnt>255)UART1_Rx_cnt=0;
//		HAL_UART_Receive_IT(&UART1_Handler,(u8 *)USART1Buf,1);		
//} 

#if(GPSRec)
if(huart->Instance == USART1)
    {
        uint8_t Res = *USART1Buf;  
        
         
        if(Res == '$')
        {
            UART1_Rx_cnt = 0;
        }
        
        U1Buffer[UART1_Rx_cnt++] = Res;
        
         
        if(U1Buffer[0] == '$' && U1Buffer[4] == 'M' && U1Buffer[5] == 'C')
        {
            if(Res == '\n')  
            {
                memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);  
                memcpy(Save_Data.GPS_Buffer, U1Buffer, UART1_Rx_cnt);  
                Save_Data.isGetData = true;
                UART1_Rx_cnt = 0;
                memset(U1Buffer, 0, sizeof(U1Buffer));  
            }
        }
        
         
        if(UART1_Rx_cnt >= sizeof(U1Buffer))
        {
            UART1_Rx_cnt = 0;  
        }
        
         
        HAL_UART_Receive_IT(&UART1_Handler, USART1Buf, 1);
    }
#endif		


	
//	if(huart->Instance==USART2)
//	{
//	 if(0 == UART2_Rx_cnt)
//	   {
//		   __HAL_TIM_CLEAR_FLAG(&htim3,TIM_FLAG_UPDATE);
 
//	   }
 
//		 
//		U2Buffer[UART2_Rx_cnt++]=*USART2Buf;

//		if(UART2_Rx_cnt>99)UART2_Rx_cnt=0;		 
//		HAL_UART_Receive_IT(&UART2_Handler,(u8 *)USART2Buf,1);

//	}
	
	
//	if(huart->Instance==USART3)
//	{
//	 if(0 == UART3_Rx_cnt)
//	   {
//		   __HAL_TIM_CLEAR_FLAG(&htim4,TIM_FLAG_UPDATE);
 
//	   }
 
//		
//		U3Buffer[UART3_Rx_cnt]=*USART3Buf;
//		++UART3_Rx_cnt;
//		if(UART3_Rx_cnt>99)UART3_Rx_cnt=0;
//		HAL_UART_Receive_IT(&UART3_Handler,(u8 *)USART3Buf,1);
//	}	
}
 
 
 
 
 
//wifibound=115200
void Uart_Init(void)
{
	UART_Setinit(115200,115200,115200);
}







/**
  * @brief  Print a string on the HyperTerminal
  * @param  p_string: The string to be printed
  * @retval None
  */
//void Serial_PutString(uint8_t *p_string)
//{
//  uint16_t length = 0;

//  while (p_string[length] != '\0')
//  {
//    length++;
//  }
//  HAL_UART_Transmit(&UART1_Handler, p_string, length, TX_TIMEOUT);
//}

 



void Serial_PutString(char *str) {
    uint16_t len = 0;
    while (str[len] != '\0' && len < 0xFFFF) len++;  
    
    if (len > 0) {
        HAL_UART_Transmit(&UARTx_Handler, (void *)str, len, TX_TIMEOUT);
    }
}
#define UART_TX_BUF_SIZE 1024

 





void Serial_Printf(const char *fmt, ...)
{
    static char tx_buf[UART_TX_BUF_SIZE];
    va_list args;
    int required_len;
    
     
    va_start(args, fmt);
    required_len = vsnprintf(NULL, 0, fmt, args) + 1; // +1 for null-terminator
    va_end(args);
    
     
    if(required_len <= UART_TX_BUF_SIZE) {
        va_start(args, fmt);
        vsnprintf(tx_buf, sizeof(tx_buf), fmt, args);
        va_end(args);
        
        HAL_UART_Transmit(&UARTx_Handler, (uint8_t*)tx_buf, strlen(tx_buf), HAL_MAX_DELAY);
    } else {
         
        char *large_buf = malloc(required_len);   
        if(large_buf) {
            va_start(args, fmt);
            vsnprintf(large_buf, required_len, fmt, args);
            va_end(args);
            
            for(int i=0; i<required_len; i+= UART_TX_BUF_SIZE-1) {
                int chunk_len = (required_len-i) > (UART_TX_BUF_SIZE-1) ? 
                              (UART_TX_BUF_SIZE-1) : (required_len-i);
                HAL_UART_Transmit(&UARTx_Handler, (uint8_t*)&large_buf[i], chunk_len, HAL_MAX_DELAY);
            }
            free(large_buf);   
        } else {
            HAL_UART_Transmit(&UARTx_Handler, (uint8_t*)"[MEM_ERR]\r\n", 11, HAL_MAX_DELAY);
        }
    }
}


 




void Serial_PutHex(uint8_t *data, uint16_t len) {
    const char hexTable[] = "0123456789ABCDEF";
    uint8_t hexBuf[3] = {0};  
    
    for (uint16_t i = 0; i < len; i++) {
         
        hexBuf[0] = hexTable[(data[i] >> 4) & 0x0F];
        hexBuf[1] = hexTable[data[i] & 0x0F];
        
        HAL_UART_Transmit(&UARTx_Handler, hexBuf, 2, TX_TIMEOUT);
        
         
        if (i < len - 1) {
            uint8_t space = ' ';
            HAL_UART_Transmit(&UARTx_Handler, &space, 1, TX_TIMEOUT);
        }
    }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */

  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */

  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}


void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
#if ESP8266_THINGER_BRIDGE
    if (huart->Instance == USART1) {
        ESP8266_ResetRx();
        __HAL_UART_CLEAR_OREFLAG(huart);
        HAL_UART_Receive_IT(huart, USART1Buf, 1);
    }
#else
    (void)huart;
#endif
}