#include "main.h"
#include "esp8266.h"

#if (USART1_ESP)
    #define ESP8266_BUF         U1Buffer
    #define ESP8266_CNT         UART1_Rx_cnt
    #define ESP8266_BUFF_SIZE   USART_REC_LEN
    #define ESP8266_SEND(data, len)  USART1_send((unsigned char *)data, len)
#elif (USART2_ESP)
    #define ESP8266_BUF         U2Buffer
    #define ESP8266_CNT         UART2_Rx_cnt
    #define ESP8266_BUFF_SIZE   USART_REC_LEN
    #define ESP8266_SEND(data, len)  USART2_send((unsigned char *)data, len)
#else
    #error "Please define ESP8266_UART as USART1 or USART2!"
		#error "Please define ESP8266_UART as USART1 or USART2!"
#endif


unsigned short esp8266_cntPre = 0;
uint16_t timeCount1 = 0;
bool usart_send_flag=0;

extern u8 mode,warningFlag,load;
extern float voltage;
extern float volMax;
extern uint16_t current;
extern uint16_t currentMax;
extern u8 elQuantity;
extern uint8_t temperature;
extern uint8_t tempMax;
//==========================================================
 
 
 
 
 
//==========================================================
void UsartSendReceiveData(void)
{
	unsigned char *dataPtr = NULL;
	char SEND_BUF[250];
	int setValue = 0;

	dataPtr = ESP8266_GetIPD(0);

	timeCount1++;
	if (timeCount1 >= 5)
	{
		timeCount1 = 0;
		usart_send_flag = 1;
	}
	if (usart_send_flag == 1)
	{
		usart_send_flag = 0;

		memset(SEND_BUF, 0, sizeof(SEND_BUF));  

		sprintf(SEND_BUF, "voltage%1.2f current%4d temperature%2d elQuantity%3d warningFlag%2d",voltage,
                current == INA219_CURRENT_INVALID ? -1 : (int)current,
                temperature, elQuantity, warningFlag);

		ESP8266_SendData((u8 *)SEND_BUF, strlen(SEND_BUF));
		
		ESP8266_Clear();
	}
}
void UsartReceive(void)
{
	unsigned char *dataPtr = NULL;
	char SEND_BUF[250];
	int setValue = 0;
		dataPtr = ESP8266_GetIPD(0);
		if (dataPtr != NULL)
	{
		setValue = ReceiveData_(dataPtr,"load:");
		if(setValue != 0)
		{
			if(warningFlag == 0)
			{
				load = !load;
				if(load == 0){RELAY(OFF);}
				else{RELAY(ON);}
			}
		}
		setValue = ReceiveData_(dataPtr,"volMax:");
		if(setValue != 0)
		{
			volMax = (float)(setValue - 1)/10;
		}
		setValue = ReceiveData_(dataPtr,"currentMax:");
		if(setValue != 0)
		{
			currentMax = setValue - 1;
		}
		setValue = ReceiveData_(dataPtr,"tempMax:");
		if(setValue != 0)
		{
			tempMax = setValue - 1;
		}
		
		ESP8266_Clear();  
	}
}

//==========================================================
 
 
 
 
 
//==========================================================
void ESP8266_Clear(void)
{
	memset(ESP8266_BUF, 0, sizeof(ESP8266_BUF));
	ESP8266_CNT = 0;
}

//==========================================================
 
 
 
 
 
//==========================================================
_Bool ESP8266_WaitRecive(void)
{
	if(ESP8266_CNT == 0) 							 
		return REV_WAIT;
		
	if(ESP8266_CNT == esp8266_cntPre)				 
	{
		ESP8266_CNT = 0;							 
			
		return REV_OK;								 
	}
		
	esp8266_cntPre = ESP8266_CNT;					 
	
	return REV_WAIT;								 
}

//==========================================================
 
 
 
 
 
 
//==========================================================
_Bool ESP8266_SendCmd(char *cmd, char *res, u16 time)
{	
  ESP8266_SEND((unsigned char *)cmd,strlen((const char *)cmd));
	
	while(time--)
	{
		if(ESP8266_WaitRecive() == REV_OK)							 
		{
			if(strstr((const char *)ESP8266_BUF, res) != NULL)		 
			{
				ESP8266_Clear();									 
				
				return 0;
			}
		}
		
		delay_ms(1);
	}
	
	return 1;

}

//==========================================================
 
 
 
 
 
 
//==========================================================
void ESP8266_SendData(unsigned char *data, unsigned short len)
{

	char cmdBuf[32];
	
	ESP8266_Clear();								 
	sprintf(cmdBuf, "AT+CIPSEND=0,%d\r\n", len);		 
	if(!ESP8266_SendCmd(cmdBuf, ">", 500))				 
	{
			ESP8266_SEND(data , len);          
	}
}

//==========================================================
 
 
 
 
 
 
//==========================================================
unsigned char *ESP8266_GetIPD(unsigned short timeOut)
{

	char *ptrIPD = NULL;
	
	do
	{
		if(ESP8266_WaitRecive() == REV_OK)								 
		{
			ptrIPD = strstr((char *)ESP8266_BUF, "IPD,");				 
			if(ptrIPD == NULL)											 
			{
				//printf("\"IPD\" not found\r\n");
			}
			else
			{
				ptrIPD = strchr(ptrIPD, ':');							 
				if(ptrIPD != NULL)
				{
					ptrIPD++;
					return (unsigned char *)(ptrIPD);
				}
				else
					return NULL;
				
			}
		}
		delay_ms(1);													 
	} while(timeOut--);
	
	return NULL;														 

}

//==========================================================
 
 
 
 
 
//==========================================================
static _Bool ESP8266_InitCommand(char *command)
{
    for (uint8_t attempt = 0; attempt < 3; attempt++)
    {
        ESP8266_Clear();
        if (!ESP8266_SendCmd(command, "OK", 200))
            return 1;
        if (attempt < 2)
            delay_ms(500);
    }
    return 0;
}

void ESP8266_Init(void)
{
    if (!ESP8266_InitCommand("AT\r\n") ||
        !ESP8266_InitCommand("AT+CWMODE=2\r\n") ||
        !ESP8266_InitCommand("AT+CWSAP=\"ESP8266_WIFI\",\"12345678\",5,3\r\n") ||
        !ESP8266_InitCommand("AT+CIPMUX=1\r\n"))
        return;

    ESP8266_InitCommand("AT+CIPSERVER=1,8080\r\n");
}
//==========================================================
 
 
 
 
 
//==========================================================
u16 ReceiveData_(unsigned char *dataPtr,const char *name)
{
	char *str1 = 0, i;
	int setValue = 0;
	char setvalue[3] = {0};
	if(strstr((char *)dataPtr,name)!=NULL)
	{
		BEEP_DOING();
		str1 = strstr((char *)dataPtr,name);
		while (*str1 != ':')
		{
			str1 = str1 + 1;
			delay_ms(10);
		}
		setValue = atoi(setvalue);
		if (setValue >= 0 && setValue <= 999)
		{
			return setValue + 1;
		}
		return 0;
	}
	return 0;
}

//void ESP8266_Sendmessage(unsigned char* SEND_BUF,uint8_t ch2o)
//{
//	
//		sprintf(SEND_BUF, "ch2o%2d",ch2o);

//		ESP8266_SendData((u8 *)SEND_BUF, strlen(SEND_BUF));
//		
//		ESP8266_Clear();
//}	

void ESP8266_Sendmessage(uint16_t value)
{
    static unsigned char SEND_BUF[SEND_BUF_SIZE];   
    
    int len = snprintf((char*)SEND_BUF, SEND_BUF_SIZE, "ch2o%u", value);
    
    if(len > 0 && len < SEND_BUF_SIZE) {
        ESP8266_SendData(SEND_BUF, (u16)len);
    } else {
         
        ESP8266_SendData(SEND_BUF, SEND_BUF_SIZE-1);
    }
    
    ESP8266_Clear();
}
