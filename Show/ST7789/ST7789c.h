#ifndef __ST7789C_H
#define __ST7789C_H

#include "main.h"


 
typedef struct  
{										    
	uint16_t width;			 
	uint16_t height;			 
	uint16_t id;				  //LCD ID
	uint8_t  dir;			   
	uint16_t	 wramcmd;		 
	uint16_t  setxcmd;		 
	uint16_t  setycmd;		 
  uint8_t   xoffset;    
  uint8_t	 yoffset;
}_lcd_dev; 	


 
extern _lcd_dev lcddev;	 
#define USE_HORIZONTAL  	 0  
  
 
#define LCD_W 240
#define LCD_H 240

 
 
 

 
#define WHITE       0xFFFF
#define BLACK      	0x0000	  
#define BLUE       	0x001F  
#define BRED        0XF81F
#define GRED 			 	0XFFE0
#define GBLUE			 	0X07FF
#define RED         0xF800
#define MAGENTA     0xF81F
#define GREEN       0x07E0
#define CYAN        0x7FFF
#define YELLOW      0xFFE0
#define BROWN 			0XBC40 				 
#define BRRED 			0XFC07 				 
#define GRAY  			0X8430 				 
#define GRAY0       0xEF7D 
#define GRAY1       0x8410      	 
#define GRAY2       0x4208 
 

#define DARKBLUE      	 0X01CF	 
#define LIGHTBLUE      	 0X7D7C	 
#define GRAYBLUE       	 0X5458  
 
 
#define LIGHTGREEN     	0X841F  
#define LIGHTGRAY     	0XEF5B  
#define LGRAY 			 		0XC618  

#define LGRAYBLUE      	0XA651  
#define LBBLUE          0X2B12  



#define COM 0
#define DAT 1



#define ST7789_RST_PORT                 GPIOC
#define ST7789_RST_PIN                	GPIO_PIN_14
#define ST7789_RST_CLK_ENABLE()       	do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)              

#define ST7789_DC_PORT              	 	GPIOC
#define ST7789_DC_PIN               	 	GPIO_PIN_13
#define ST7789_DC_CLK_ENABLE()      	 	do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)              

//#define ST7789_BK_PORT                	GPIOB
//#define ST7789_BK_PIN                 	GPIO_PIN_5
 


#define ST7789_RESET 		 HAL_GPIO_WritePin(ST7789_RST_PORT,ST7789_RST_PIN,GPIO_PIN_RESET);      //
#define ST7789_SET  	   HAL_GPIO_WritePin(ST7789_RST_PORT,ST7789_RST_PIN,GPIO_PIN_SET);

#define ST7789_REG_IN    HAL_GPIO_WritePin(ST7789_DC_PORT,ST7789_DC_PIN,GPIO_PIN_RESET);
#define ST7789_DATA_IN   HAL_GPIO_WritePin(ST7789_DC_PORT,ST7789_DC_PIN,GPIO_PIN_SET);


//#define ST7789_BK_ON  	HAL_GPIO_WritePin(ST7789_BK_PORT,ST7789_BK_PIN,GPIO_PIN_SET);
//#define ST7789_BK_OFF  	HAL_GPIO_WritePin(ST7789_BK_PORT,ST7789_BK_PIN,GPIO_PIN_RESET);




void LCD_ST7789_Init(void);
void LCD_Wr_Reg(uint8_t data);
void LCD_Wr_Data(uint8_t data);
void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue);
void LCD_WriteRAM_Prepare(void);
void Lcd_WriteData_16Bit(uint16_t Data);
void LCD_DrawPoint(uint16_t x,uint16_t y);
void LCD_Clear(uint16_t Color);
void LCD_SetWindows(uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd);
void LCD_direction(uint8_t direction);
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);
void LCD_Init(void);
void LCD_RESET(void);



void GUI_DrawPoint(uint16_t x,uint16_t y,uint16_t color);
void LCD_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color);
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void Draw_Circle(uint16_t x0,uint16_t y0,uint16_t fc,uint8_t r);
void Draw_Triangel(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
void Fill_Triangel(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
void LCD_ShowChar(uint16_t x,uint16_t y,uint16_t fc, uint16_t bc, uint8_t num,uint8_t size,uint8_t mode);
void LCD_ShowNum(uint16_t x,uint16_t y,u32 num,uint8_t len,uint8_t size);
void LCD_Show2Num(uint16_t x,uint16_t y,uint16_t num,uint8_t len,uint8_t size,uint8_t mode);
void LCD_ShowString(uint16_t x,uint16_t y,uint8_t size,uint8_t *p,uint8_t mode);
void GUI_DrawFont16(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *s,uint8_t mode);
void GUI_DrawFont24(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *s,uint8_t mode);
void GUI_DrawFont32(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *s,uint8_t mode);
void Show_Str(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *str,uint8_t size,uint8_t mode);
void Gui_Drawbmp16(uint16_t x,uint16_t y,const unsigned char *p);  
void gui_circle(int xc, int yc,uint16_t c,int r, int fill);
void Gui_StrCenter(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *str,uint8_t size,uint8_t mode);
void LCD_DrawFillRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void test_st7789(void);

/*************************TEST****************************/
void DrawTestPage(uint8_t *str);
void Display_ButtonUp(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
void menu_test(void);
void Test_Color(void);
void Test_FillRec(void);
void Test_Circle(void);
void Test_Triangle(void);
void English_Font_test(void);
void Chinese_Font_test(void);
void Pic_test(void);
void Load_Drow_Dialog(void);
void Touch_Test(void);
void main_test(void);
void Rotate_Test(void);

#endif



