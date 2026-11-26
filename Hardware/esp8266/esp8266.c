#include "main.h"
#include "esp8266.h"

/* ESP owns Wi-Fi; STM32 validates and persists all protection settings. */
#if ESP8266_THINGER_BRIDGE
#include "bms_config.h"
#include "bms_control.h"
static uint32_t cloudLastSend;
static uint32_t cloudSequence;
static volatile uint32_t cloudTxErrors, cloudRxRejected;
static char cloudFrame[192];
static char rxLine[80], pendingLine[80];
static unsigned rxLength;
static uint8_t rxDiscard;
static volatile uint8_t pendingReady;
static BmsControlCommand lastCommand;
static uint8_t lastAck;

extern float voltage, volMax;
extern uint16_t current, currentMax;
extern int16_t temperatureDeciC;
extern uint8_t elQuantity, load, tempMax;
extern volatile uint8_t warningFlag;

void ESP8266_Init(void) {}

void ESP8266_ResetRx(void)
{
    rxLength = 0;
    rxDiscard = 0;
}

void ESP8266_RxByte(uint8_t byte)
{
    if (byte == '\n') {
        if (!rxDiscard && rxLength && !pendingReady) {
            rxLine[rxLength] = '\0';
            memcpy(pendingLine, rxLine, rxLength + 1);
            __DMB();
            pendingReady = 1;
        } else if (rxDiscard || pendingReady) {
            ++cloudRxRejected;
        }
        ESP8266_ResetRx();
    } else if (byte != '\r' && !rxDiscard) {
        if (byte >= ' ' && byte <= '~' && rxLength < sizeof(rxLine) - 1)
            rxLine[rxLength++] = (char)byte;
        else
            rxDiscard = 1;
    }
}

void UsartSendReceiveData(void)
{
    uint32_t now = HAL_GetTick();
    if ((uint32_t)(now - cloudLastSend) < 2000u) return;
    cloudLastSend = now;
    int mv = (int)(voltage * 1000.0f + 0.5f);
    int ma = current == INA219_CURRENT_INVALID ? -1 : (int)current;
    unsigned warnings = warningFlag;
    int temp = (warnings & BMS_WARNING_TEMPERATURE_SENSOR) ? 0 : temperatureDeciC / 10;
    char legacyTemp[8];
    if (warnings & BMS_WARNING_TEMPERATURE_SENSOR)
        strcpy(legacyTemp, "ERR");
    else
        snprintf(legacyTemp, sizeof(legacyTemp), "%d", temp);
    int length = snprintf(cloudFrame, sizeof(cloudFrame),
        "BMS,TEMP,%s\nBMS,V1,%lu,%d,%d,%d,%u,%u,%u\n",
        legacyTemp, (unsigned long)cloudSequence++, mv, ma, temp,
        (unsigned)elQuantity, warnings, (unsigned)load);
    char state[96];
    int stateLength = snprintf(state, sizeof(state), "BMS,CFG,%lu,%u,%u,%lu,%lu,%u",
        (unsigned long)(volMax * 1000.0f + 0.5f), (unsigned)currentMax, (unsigned)tempMax,
        (unsigned long)bmsConfigRevision, (unsigned long)lastCommand.id, (unsigned)lastAck);
    stateLength = stateLength > 0 && (size_t)stateLength < sizeof(state) ?
        bms_finish_line(state, sizeof(state)) : 0;
    if (length <= 0 || !stateLength || (size_t)(length + stateLength) >= sizeof(cloudFrame)) {
        ++cloudTxErrors;
        return;
    }
    memcpy(cloudFrame + length, state, (size_t)stateLength);
    length += stateLength;
    cloudFrame[length] = '\0';
    if (HAL_UART_Transmit(&UART1_Handler, (uint8_t *)cloudFrame, (uint16_t)length, 25) != HAL_OK)
        ++cloudTxErrors;
}

void UsartReceive(void)
{
    if (!pendingReady) return;
    char line[sizeof(pendingLine)];
    memcpy(line, pendingLine, sizeof(line));
    __DMB();
    pendingReady = 0;
    BmsControlCommand command;
    if (!bms_parse_command(line, &command)) {
        ++cloudRxRejected;
        return;
    }
    if (command.id == lastCommand.id) {
        if (memcmp(&command, &lastCommand, sizeof(command)) != 0)
            lastAck = BMS_ACK_ID_CONFLICT;
        // A retry never reapplies or rewrites a previously handled request.
    } else {
        lastCommand = command;
        if (!bms_limits_valid(command.low_mv, command.high_ma, command.high_c))
            lastAck = BMS_ACK_RANGE;
        else if (setn >= 2 && setn <= 4)
            lastAck = BMS_ACK_EDITING;
        else if (command.revision != bmsConfigRevision)
            lastAck = BMS_ACK_REVISION;
        else {
            lastAck = BmsConfig_Apply(command.low_mv, command.high_ma, command.high_c);
            if (lastAck == BMS_ACK_APPLIED) CotorFun();
        }
    }
    cloudLastSend = HAL_GetTick() - 2000u;
}
#else
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

extern u8 mode,load;
extern volatile u8 warningFlag;
extern float voltage;
extern float volMax;
extern uint16_t current;
extern uint16_t currentMax;
extern u8 elQuantity;
extern int16_t temperatureDeciC;
extern uint8_t loadBlocked;
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
                temperatureDeciC / 10, elQuantity, warningFlag);

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
			if(!loadBlocked)
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

#endif /* ESP8266_THINGER_BRIDGE */
