#include "main.h"
#include "lcd1602b.h"

IIC_HardWareTypedef iicpcf8574;
static uint8_t LCD_data = 0x08;   

 
static void LCD_Enable(void)
{
    LCD_data |= (1 << LCD_E_PIN);   // E=1
    PCF8574_WriteOneByte(LCD_data);
    delay_us(1);
    LCD_data &= ~(1 << LCD_E_PIN);  // E=0
    PCF8574_WriteOneByte(LCD_data);
    delay_us(1);
}

void LCD_Write_Com(uint8_t cmd) 
{
    delay_us(16);
     
    LCD_data &= ~((1 << LCD_RS_PIN) | (1 << LCD_RW_PIN));
    PCF8574_WriteOneByte(LCD_data);
    delay_us(2);
    
     
    LCD_data = (LCD_data & 0x0F) | (cmd & 0xF0);
    PCF8574_WriteOneByte(LCD_data);
    LCD_Enable();
    
     
    cmd <<= 4;
    LCD_data = (LCD_data & 0x0F) | (cmd & 0xF0);
    PCF8574_WriteOneByte(LCD_data);
    delay_us(2);
    LCD_Enable();
}

void LCD_Write_Data(uint8_t dat) 
{
    delay_us(16);
     
    LCD_data = (LCD_data | (1 << LCD_RS_PIN)) & ~(1 << LCD_RW_PIN);
    PCF8574_WriteOneByte(LCD_data);
    delay_us(2);
    
     
    LCD_data = (LCD_data & 0x0F) | (dat & 0xF0);
    PCF8574_WriteOneByte(LCD_data);
    delay_us(2);
    LCD_Enable();
    
     
    dat <<= 4;
    LCD_data = (LCD_data & 0x0F) | (dat & 0xF0);
    PCF8574_WriteOneByte(LCD_data);
    delay_us(2);
    LCD_Enable();
}

 
void LCD1602_Init(void) 
{        
   PCF8574_Init(PCF8574_ADDR);  
//    delay_ms(100);
    
    PCF8574_WriteOneByte(0x08);   
    
    LCD_Write_Com(0x0F);   
   // delay_us(40);
    
    LCD_Write_Com(0x28);   
    PCF8574_WriteOneByte(0x0C);   
    PCF8574_WriteOneByte(0x08);   
    
    LCD_Write_Com(0x28);   
    LCD_Write_Com(0x01);   
   // delay_ms(2);
    
    LCD_Write_Com(0x0C);   
    LCD_Write_Com(0x06);   
   // delay_ms(10);
}

void LCD_Write_String(uint8_t x, uint8_t y, uint8_t *str) 
{
    if(x == 0) x = 1;   
    
     
    if(y == 0 || y == 1) LCD_Write_Com(0x80 + x - 1);
    else if(y == 2) LCD_Write_Com(0xC0 + x - 1);
    else if(y == 3) LCD_Write_Com(0x94 + x - 1);
    else if(y == 4) LCD_Write_Com(0xD4 + x - 1);
    
     
    while(*str) {
        LCD_Write_Data(*str++);
    }
}

void LCD_Move_Cursor(uint8_t x, uint8_t y, uint8_t blink) 
{
    LCD_Write_String(x, y, "");   
    LCD_Write_Com(blink ? 0x0F : 0x0C);   
}

void LCD1602_Clear(void)
{
    LCD_Write_Com(0x01);
    delay_ms(2);
}

 
uint8_t PCF8574_Init(uint8_t addr)
{
    uint8_t temp = 1;   
    
    I2C_Start(iicpcf8574);
    I2C_SendByte(iicpcf8574,addr);
    temp = I2C_WaitAck(iicpcf8574);
    I2C_Stop(iicpcf8574);
    PCF8574_WriteOneByte(0xFF);   

    
    return temp;
}

uint8_t PCF8574_ReadOneByte(void)
{
    uint8_t temp = 0;
    I2C_Start(iicpcf8574);
    I2C_SendByte(iicpcf8574,PCF8574_ADDR | 0x01);   
    I2C_WaitAck(iicpcf8574);
    temp = I2C_ReceiveByte(iicpcf8574);
    I2C_Stop(iicpcf8574);
    return temp;
}

void PCF8574_WriteOneByte(uint8_t DataToWrite)
{
    I2C_Start(iicpcf8574);
    I2C_SendByte(iicpcf8574,PCF8574_ADDR);   
    I2C_WaitAck(iicpcf8574);
    I2C_SendByte(iicpcf8574,DataToWrite);
    I2C_WaitAck(iicpcf8574);
    I2C_Stop(iicpcf8574);
    HAL_Delay(1);   
}

void PCF8574_WriteBit(uint8_t bit, uint8_t sta)
{
    uint8_t data = PCF8574_ReadOneByte();
    data = sta ? (data | (1 << bit)) : (data & ~(1 << bit));
    PCF8574_WriteOneByte(data);
}

uint8_t PCF8574_ReadBit(uint8_t bit)
{
    return (PCF8574_ReadOneByte() & (1 << bit)) ? 1 : 0;
}
