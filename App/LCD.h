#ifndef __LCD_H
#define __LCD_H	

#include "main.h" 

#define Fsize12      12
#define Fsize16      16
#define XLevelL		 0x00
#define XLevelH		 0x10
#define Max_Column	 128
#define Max_Row		 64
#define	Brightness	 0xFF 
#define X_WIDTH 	 128
#define Y_WIDTH 	 64	    						  

// D0 --> 3线SPI的SCLK
#define LCD_SCLK_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_10)//CLK
#define LCD_SCLK_Set() GPIO_SetBits(GPIOB,GPIO_Pin_10)

// D1 --> 3线SPI的SDIN
#define LCD_SDIN_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_11)//DIN
#define LCD_SDIN_Set() GPIO_SetBits(GPIOB,GPIO_Pin_11)

// RES -->  
#define LCD_RST_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_5)  //RES
#define LCD_RST_Set() GPIO_SetBits(GPIOB,GPIO_Pin_5)

// DC --> 数据/命令控制引脚，H:数据  L:命令
#define LCD_DC_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_1)   //DC
#define LCD_DC_Set() GPIO_SetBits(GPIOB,GPIO_Pin_1)

// CS --> 3线SPI的CS
#define LCD_CS_Clr()  GPIO_ResetBits(GPIOB,GPIO_Pin_0)  //CS
#define LCD_CS_Set()  GPIO_SetBits(GPIOB,GPIO_Pin_0)
 
#define LCD_CMD  0	//写命令
#define LCD_DATA 1	//写数据

//LCD控制用函数
void LCD_WR_Byte(uint8_t dat,uint8_t cmd);	    
void LCD_Display_On(void);
void LCD_Display_Off(void);
void LCD_Refresh_Gram(void);		   
							   		    
void LCD_Init(void);
void LCD_Clear(void);
void LCD_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
void LCD_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot);
void LCD_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode);
void LCD_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size);
void LCD_ShowString(uint8_t x,uint8_t y,const uint8_t *p);	
void LCD_Set_Pos(unsigned char x, unsigned char y);
void LCD_ShowCHinese(uint8_t x,uint8_t y,uint8_t no);
void LCD_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);

#endif  
	 



