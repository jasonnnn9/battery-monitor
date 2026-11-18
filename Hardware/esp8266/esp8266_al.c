#include "main.h"
#include "esp8266_al.h"



unsigned short esp_cntPre = 0;

const char* WIFI ="main";
const char* WIFIASSWORD="88888888";
const char* ClintID="0051";
const char* username="admin";
const char* passwd="admin";
const char* Url="121.40.58.109";
const char* pubtopic="0051j";
const char* subtopic="0051f";

u16 timeCount1 = 0;
bool usart_send_flag=0;

u8 warningFlag,lightMax,light=23;
u16 distanceMax;
float longitude=100.1,latitude=20.5;

void UsartSendReceiveData(void)    
{
    char SEND_BUF[200];
		int setValue = 0;
		
		timeCount1 ++; 
		if(timeCount1 >= 3)
		{
			timeCount1 = 0;
			usart_send_flag = 1;
		}
		
	  if(ESP_WaitRecive() == REV_OK)
		{
			setValue = ReceiveData("disMax:");
			if(setValue != NULL)
			{
				distanceMax = setValue -1;
			}
			setValue = ReceiveData("lightMax:");
			if(setValue != NULL)
			{
				lightMax = setValue - 1;
			}
				
			ESP8266_Clear(); 
		}
	
	   
	  if(usart_send_flag==1)
		{
			usart_send_flag = 0;
			sprintf(SEND_BUF,"AT+MQTTPUB=0,\"%s\",\"lon%6d lat%6d w%2d lig%2d\",0,0\r\n",pubtopic,(int)(longitude*1000),(int)(latitude*1000),warningFlag,light);
			USART1_send((u8 *)SEND_BUF,strlen(SEND_BUF));
			ESP8266_Clear(); 
		}
}

//==========================================================
 
//
 
//
 
//
 
//
 
//==========================================================
_Bool ESP_WaitRecive(void)
{
	if(ESP8266_CNT == 0) 							 
		return REV_WAIT;
		
	if(ESP8266_CNT == esp_cntPre)				 
	{
		ESP8266_CNT = 0;							 
			
		return REV_OK;								 
	}
		
	esp_cntPre = ESP8266_CNT;					 
	
	return REV_WAIT;								 
}

//==========================================================
 
//
 
//
 
 
//
 
//
 
//==========================================================
_Bool ESP8266_SendCmd(char *cmd, char *res, u16 time)
{	
  USART1_send((unsigned char *)cmd,strlen((const char *)cmd));
	
	while(time--)
	{
		if(ESP_WaitRecive() == REV_OK)							 
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
uint8_t Judge=0;
void Esp_Init(int screen) 
{
	
	
	if(screen == 0)
	{
#if(0)		
		OLED_CLS(); 
		OLED_ShowStr(5,0,(u8 *)"Linking...",2,0);
#endif		
		do
		{
			Judge = Esp_Link_Init();
#if(0)				
			OLED_ShowStr(5,0,(u8 *)"error code:    ",2,0);
			OLED_ShowTH(5,3,Judge);
#endif			
		}while(Judge);   
#if(0)		
		OLED_CLS();
#endif		
	}
}

//==========================================================
 
//
 
//
 
//
 
//
 
//==========================================================
void ESP8266_Clear(void)
{
	memset(ESP8266_BUF, 0, sizeof(ESP8266_BUF));
	ESP8266_CNT = 0;
}

char Esp_Link_Init(void)
{
	char tempBuff[50];
	u8 tempNum = 99;
	ESP8266_Clear();
 
	while(ESP8266_SendCmd("AT+RST\r\n", "OK", 200))          
	delay_ms(500);
	
 
	while(ESP8266_SendCmd("ATE0\r\n", "OK", 200))
	delay_ms(500);
	if(strcmp((char *)ESP8266_BUF,"OK"))
		tempNum = 1;
	
 
	while(ESP8266_SendCmd("AT+CWMODE=1\r\n", "OK", 200))
	delay_ms(500);
	if(strcmp((char *)ESP8266_BUF,"OK"))
		tempNum = 2;
	
//	memset(ESP8266_BUF,0,sizeof(ESP8266_BUF));
 
	sprintf(tempBuff,"AT+CWJAP=\"%s\",\"%s\"\r\n",WIFI,WIFIASSWORD);
	while(ESP8266_SendCmd(tempBuff, "OK", 200))
	delay_ms(500);
	if(strcmp((char *)ESP8266_BUF,"OK"))
		tempNum = 3;
	
//	memset(ESP8266_BUF,0,sizeof(ESP8266_BUF));
	sprintf(tempBuff,"AT+MQTTUSERCFG=0,1,\"%s\",\"%s\",\"%s\",0,0,\"\"\r\n",ClintID,username,passwd); 
	while(ESP8266_SendCmd(tempBuff, "OK", 200))
	delay_ms(500);
	if(strcmp((char *)ESP8266_BUF,"OK"))
		tempNum = 4;
	
//	memset(ESP8266_BUF,0,sizeof(ESP8266_BUF));
	sprintf(tempBuff,"AT+MQTTCONN=0,\"%s\",1883,1\r\n",Url);  
	while(ESP8266_SendCmd(tempBuff, "OK", 200))
	delay_ms(500);
	if(strcmp((char *)ESP8266_BUF,"OK"))
		tempNum = 5;
	
	sprintf(tempBuff,"AT+MQTTSUB=0,\"%s\",1\r\n",subtopic);  
	while(ESP8266_SendCmd(tempBuff, "OK", 200))
	delay_ms(500);
	if(strcmp((char *)ESP8266_BUF,"OK"))
		tempNum = 5;
	memset(ESP8266_BUF,0,sizeof(ESP8266_BUF));
	tempNum = 0;
	return tempNum;
}
//==========================================================
 
//
 
//
 
//
 
//
 
//==========================================================
u16 ReceiveData(const char *name)
{
	char *str1 = 0, i;
	int setValue = 0;
	char setvalue[3] = {0};
	if(strstr((char *)ESP8266_BUF,name)!=NULL)
	{
//		BEEP = 1;
//		delay_ms(50);
//		BEEP = 0;
		
		str1 = strstr((char *)ESP8266_BUF,name);
		while (*str1 != ':')
		{
			str1 = str1 + 1;
			delay_ms(10);
		}
		str1 = str1 + 1;
		i = 0;
		while (*str1 >= '0' && *str1 <= '9')
		{
			setvalue[i] = *str1;
			i++;
			str1++;
			if (*str1 == '\r')
				break;
			delay_ms(10);
		}
		setvalue[i] = '\0';  
		setValue = atoi(setvalue);
		if (setValue >= 0 && setValue <= 999)
		{
			return setValue + 1;
		}
		return NULL;
	}
	return NULL;
}

