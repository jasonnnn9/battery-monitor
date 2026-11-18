#include "main.h"
#include "esp8266.h"

 
#define PRODUCEKEY 			"2bWg8Gc7k1" 
#define DEVICENAME 			"ESP8266TEST"			 
#define TOKEN       		"version=2018-10-31&res=products%2F2bWg8Gc7k1%2Fdevices%2FESP8266TEST&et=1780553169&method=md5&sign=%2BTz%2BnruiiY8WrcHIqWusIw%3D%3D" //token
///////////////////////////////////////////////////////////////////////
static const char MQTT_CONFIG_CMD[] = "AT+MQTTUSERCFG=0,1,\"%s\",\"%s\",\"%s\",0,0,\"\"\r\n";
static const char MQTT_SUBSCRIBE_CMD[] = "AT+MQTTSUB=0,\"$sys/%s/%s/thing/property/%s\",%d\r\n";

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

//==========================================================
 
 
 
 
 
//==========================================================
void ESP8266_Clear(void)
{
	memset(ESP8266_BUF, 0, sizeof(ESP8266_BUF));
	ESP8266_CNT = 0;
}

//==========================================================
 
 
 
 
 
//==========================================================
_Bool ESP8266_WaitRecive(uint16_t *received_len) {
    if(ESP8266_CNT == 0) {
        *received_len = 0;   
        return REV_WAIT;
    }
    
    if(ESP8266_CNT == esp8266_cntPre) {
        *received_len = ESP8266_CNT;   
        ESP8266_CNT = 0;               
        esp8266_cntPre = 0;            
        return REV_OK;
    }
    
    esp8266_cntPre = ESP8266_CNT;
    *received_len = 0;   
    return REV_WAIT;
}
//==========================================================
 
 
 
 
 
 
//==========================================================


_Bool ESP8266_SendCmd(char *cmd, char *res, u16 time) 
{
    ESP8266_Clear();
    ESP8266_SEND((unsigned char *)cmd, strlen(cmd));
    HAL_Delay(5000);

    uint16_t last_checked_index = 0;
    uint16_t received_len = 0;
    uint16_t res_len = strlen(res);   

    while(time--) {
        if(ESP8266_WaitRecive(&received_len) == REV_OK) {
            last_checked_index = received_len;   
            
             
            for(uint16_t i = 0; i <= received_len - res_len; i++) {
                if(memcmp(&ESP8266_BUF[i], res, res_len) == 0) {
                    return 0;   
                }
            }
        }
        HAL_Delay(1);
    }
    return 1;   
}


//==========================================================
 
 
 
 
 
//==========================================================

void ESP8266_OneNet_Init(void)
{	
	char cmd[512];  
	
	ESP8266_Clear();

	while(ESP8266_SendCmd("AT+RST\r\n", "ready", 5000))        
	HAL_Delay(1000);
	
	while(ESP8266_SendCmd("AT\r\n", "OK", 200))          
	HAL_Delay(1000);
	
	while(ESP8266_SendCmd("AT+CWMODE=1\r\n", "OK", 200))       
	HAL_Delay(1000);

	while(ESP8266_SendCmd("AT+CWDHCP=1,1\r\n", "OK", 200))       
	HAL_Delay(1000);
	
	while(ESP8266_SendCmd("AT+CWJAP=\"LK\",\"1234567890\"\r\n", "GOT IP", 200)) 
	HAL_Delay(1000);

		
	memset(cmd, 0, 512);
	
	snprintf(cmd, sizeof(cmd), MQTT_CONFIG_CMD, DEVICENAME, PRODUCEKEY, TOKEN);   
	while(ESP8266_SendCmd(cmd, "OK", 2000)) 
	HAL_Delay(1000);
	
	
	while(ESP8266_SendCmd("AT+MQTTCONN=0,\"mqtts.heclouds.com\",1883,1\r\n", "OK", 2000))
	HAL_Delay(1000);	
	
	memset(cmd, 0, 512);

	snprintf(cmd, sizeof(cmd), MQTT_SUBSCRIBE_CMD, PRODUCEKEY, DEVICENAME, "post/reply", 2); 
	while(ESP8266_SendCmd(cmd, "OK", 1000))
  HAL_Delay(1000);
	
	memset(cmd, 0, 512);

	snprintf(cmd, sizeof(cmd), MQTT_SUBSCRIBE_CMD,PRODUCEKEY, DEVICENAME, "set", 2); 
	while(ESP8266_SendCmd(cmd, "OK", 1000))
  HAL_Delay(1000);
	

}
//==========================================================
 
 
 
 
 
//==========================================================
u16 ReceiveData_OneNet(unsigned char *dataPtr,const char *name)
{
	
	char *str1 = 0, i;
	int setValue = 0;
	char setvalue[3] = {0};
	if(strstr((char *)dataPtr,name)!=NULL)
	{
		str1 = strstr((char *)dataPtr,name);
		while (*str1 != ':')
		{
			str1 = str1 + 1;
			HAL_Delay(10);
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
			HAL_Delay(10);
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

 






int ESP8266_MQTTSend(float temp)
{
    char SendArray[512]; 
    
//    snprintf(SendArray, sizeof(SendArray),
//        "AT+MQTTPUB=0,\"$sys/%s/%s/thing/property/post\","
 
//        "{\\\"id\\\": \\\"123\\\"\\,\\\"params\\\":{"
//            "\\\"button\\\":{\\\"value\\\":%.1f}\\,"
//            "\\\"temp\\\":{\\\"value\\\":%d}\\,"
//            "\\\"humi\\\":{\\\"value\\\":%d}\\,"
//            "\\\"light\\\":{\\\"value\\\":%d}\\,"
//            "\\\"soil\\\":{\\\"value\\\":%d}\\,"
//            "\\\"CO2\\\":{\\\"value\\\":%d}"
//        "}}"
 
//        ",0,0\r\n",
//        PRODUCEKEY, DEVICENAME,
//        button_value, temp, humi, light, soil, CO2
//    );

					snprintf(SendArray, sizeof(SendArray),
							"AT+MQTTPUB=0,\"$sys/%s/%s/thing/property/post\","
							"\""   
							"{\\\"id\\\": \\\"123\\\"\\,\\\"params\\\":"
							"{\\\"temp\\\":{\\\"value\\\":%.1f}}}"
							"\""   
							",0,0\r\n",
							PRODUCEKEY, DEVICENAME, temp
					);
	
     
    ESP8266_SEND(SendArray, strlen(SendArray));
    
     
    delay_ms(500);
    ESP8266_Clear();
    
    return 0;

}

int ESP8266_MQTTSendReturn(bool temp)
{
    char SendArray[256]; 
					snprintf(SendArray, sizeof(SendArray),
							"AT+MQTTPUB=0,\"$sys/%s/%s/thing/property/post\","
							"\""   
							"{\\\"id\\\": \\\"123\\\"\\,\\\"params\\\":"
							"{\\\"button\\\":{\\\"value\\\":%s}}}"
							"\""   
							",0,0\r\n",
							PRODUCEKEY, DEVICENAME, temp
					);
	
     
    ESP8266_SEND(SendArray, strlen(SendArray));
    
     
    delay_ms(500);
    ESP8266_Clear();
    
    return 0;

}


int contains_substring(const u8 *main_string, const char *substring)
{
	if (strstr((const char *)main_string, substring) != NULL)
	{
		return 1;
	}
	else
	{
		return 0; 
	}
}

int ESP8266_Get(const char *res1, const char *res2)
{

	if (1 == contains_substring(ESP8266_BUF, res1))
	{
		if (1 == contains_substring(ESP8266_BUF, res2))
		{
			return 1; 
		}
	}
	return 0;
}

 
uint16_t button_st,button_at;
float temp_set;
int ESP8266_Get_state(void)
{

	if (1 == ESP8266_Get("button", "false"))
	{
		button_st++;
	}
	else if (1 == ESP8266_Get("button", "true"))
	{
		button_at++;
	}
	return 1;
}
 
int ESP8266_Get_data(const char *substring)
{
	  char *start_ptr, *end_ptr;
    char value_str[16];  
    float value = 0.0f;

     
    char pattern[32];
    snprintf(pattern, sizeof(pattern), "\"%s\":", substring);

     
    start_ptr = strstr((char *)ESP8266_BUF, pattern);
    if (start_ptr == NULL) return 0.0f;

     
    start_ptr += strlen(pattern);

     
    end_ptr = strpbrk(start_ptr, ",} \n");  
    if (end_ptr == NULL) return 0.0f;

     
    strncpy(value_str, start_ptr, end_ptr - start_ptr);
    value_str[end_ptr - start_ptr] = '\0';
    value = atof(value_str);

    return value;
}

void ESP8266_Get_information(void)
{
	ESP8266_Get_state();
	if(ESP8266_Get_data("temp_SET"))
	{
		temp_set = ESP8266_Get_data("temp_SET");
	}	
	ESP8266_Clear();
	
}










