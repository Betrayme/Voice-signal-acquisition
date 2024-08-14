/*
 * LCD.h
 *
 *  Created on: 2015-9-28
 *      Author: daogu
 */

#ifndef LCD_H_
#define LCD_H_

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000
#define BLUE         	 0x001F
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色
#define GRAYBLUE       	 0X5458 //灰蓝色

void Init_Port(void);
void LCD_WR_Bus(unsigned int data);
void LCD_WR_REG(unsigned int index);
void LCD_WR_DATA(unsigned int val);
void LCD_WR_CMD(unsigned int index,unsigned int val);
void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
void Lcd_Init(void);
void LCD_Clear(unsigned int Color);
void LCD_DrawPoint(unsigned int x,unsigned int y,unsigned int color);

void LCD_ShowHanZi(unsigned int x,unsigned int y,unsigned char *zi,
		unsigned char Z_WHITH,unsigned char Z_HIGH,unsigned int Color);
void LCD_ShowHanZis(unsigned int x,unsigned int y,unsigned char *zi,
		unsigned char Z_WHITH,unsigned char Z_HIGH,char num, unsigned int Color);

#endif /* LCD_H_ */
