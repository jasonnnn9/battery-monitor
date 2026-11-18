#include "main.h"
#include "PCF8574.h"

IIC_HardWareTypedef iicpcf8574;
static uint8_t lcd_backlight_state = 1;   

 
static void PCF8574_Write(uint8_t data) {
    IIC_Write_Device_Byte(PCF8574_ADDR, 0, data, iicpcf8574);
}

 
static void LCD1602_Write4Bits(uint8_t data, uint8_t rs_mode)
{ 										 
    uint8_t value = (data << 4)|(rs_mode << LCD_RS) |(lcd_backlight_state << LCD_BL);
    PCF8574_Write(value | LCD_E);  // E=1
    delay_us(1);
    PCF8574_Write(value & ~LCD_E); // E=0
    delay_us(100);
}

 
static void LCD1602_Write(uint8_t data, uint8_t rs_mode) {
    LCD1602_Write4Bits(data >> 4, rs_mode);    
    LCD1602_Write4Bits(data & 0x0F, rs_mode);  
}

 
void LCD1602_Init(void) 
{
    
    delay_ms(50);            

     
	
	
	
    LCD1602_Write4Bits(0x03, 0);   
    delay_ms(5);
    LCD1602_Write4Bits(0x03, 0);
    delay_us(100);
    LCD1602_Write4Bits(0x03, 0);
    delay_us(100);
    LCD1602_Write4Bits(0x02, 0);   
    delay_us(100);

     
    LCD1602_Write(0x28, 0);   
    LCD1602_Write(0x0C, 0);   
    LCD1602_Write(0x06, 0);   
    LCD1602_Clear();
}

 
void LCD1602_Clear(void) {
    LCD1602_Write(0x01, 0);
    delay_ms(2);
}

 
void LCD1602_SetCursor(uint8_t row, uint8_t col) {
    uint8_t address = (row == 0) ? 0x80 : 0xC0;   
    LCD1602_Write(address + col, 0);
}

 
void LCD1602_WriteChar(char ch) {
    LCD1602_Write(ch, 1);  // RS=1
}

 
void LCD1602_WriteString(const char *str) {
    while (*str) LCD1602_WriteChar(*str++);
}

 
void LCD1602_SetBacklight(uint8_t state) {
    lcd_backlight_state = state ? 1 : 0;
    PCF8574_Write(lcd_backlight_state << LCD_BL);
}
