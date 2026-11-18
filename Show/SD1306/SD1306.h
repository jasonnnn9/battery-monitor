#ifndef __SD1306_H
#define __SD1306_H

#include "main.h"
#define OLED_ADDRESS	0x78  
#define OLED_cmd			0x00 
#define OLED_dat			0x40 
 
#define IICSD1306S_GPIO_PORT            	   GPIOC
#define IICSD1306S_GPIO_SDA                  GPIO_PIN_13
#define IICSD1306S_GPIO_PORT            	   GPIOC
#define IICSD1306S_GPIO_SCL                  GPIO_PIN_14

 
#define NORMAL_DISPLAY  0
#define INVERSE_DISPLAY 1

 
#define FONT_6X8        1
#define FONT_8X16       2

 
void OLED_Init(void);
void WriteCmd(uint8_t I2C_Command);
void WriteDat(uint8_t I2C_Data);
void OLED_SetPos(uint8_t x, uint8_t y);
void OLED_Fill(uint8_t fill_Data);
void OLED_CLS(void);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t TextSize, uint8_t mode);
void OLED_ShowStr(uint8_t x, uint8_t y, uint8_t ch[], uint8_t TextSize, uint8_t mode);
void OLED_ShowCN(uint8_t x, uint8_t y, uint8_t N, uint8_t mode);
void OLED_DrawBMP(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t N);
void OLED_ShowCentigrade(uint8_t x, uint8_t y);
void OLED_ShowTH(uint8_t x, uint8_t y, uint8_t DATA);
void OLED_ShowFormatStr(uint8_t x, uint8_t y,  const char* format, ...);
void OLED_ShowNum(uint8_t x, uint8_t y, int num, uint8_t TextSize, uint8_t mode);
 
//extern const uint8_t F6x8[][6];
//extern const uint8_t F8X16[];
//extern const uint8_t F16x16[];
 

#endif /* __OLED_H */





