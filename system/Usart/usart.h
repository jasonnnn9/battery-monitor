#ifndef _USART_H
#define _USART_H

#include "main.h"


#define USART_REC_LEN  			512  	   
#define PORTn  			        4  	     
#define TX_TIMEOUT          ((uint32_t)100)	  


extern UART_HandleTypeDef UART1_Handler,UART2_Handler,UART3_Handler;  
extern u16 UART1_Rx_cnt;
extern u16 UART2_Rx_cnt;
extern u16 UART3_Rx_cnt;

#define RXBUFFERSIZE   1  

extern u8 USART1Buf[RXBUFFERSIZE]; 
extern u8 USART2Buf[RXBUFFERSIZE]; 
extern u8 USART3Buf[RXBUFFERSIZE]; 

extern u8 U1Buffer[USART_REC_LEN]; 
extern u8 U2Buffer[USART_REC_LEN]; 
extern u8 U3Buffer[USART_REC_LEN]; 

#define S_RX_BUF_SIZE		30
#define S_TX_BUF_SIZE		128


void Serial_Printf(const char *fmt, ...);
void UART_Setinit(uint32_t bound1,uint32_t bound2,uint32_t bound3);
void Uart_Init(void);
void Serial_PutString(char *p_string);
void Serial_PutHex(uint8_t *data, uint16_t len);

void USART1_send(u8* buf,u8 len);
void USART2_send(u8* buf,u8 len);
void USART3_send(u8* buf,u8 len);	
#endif
