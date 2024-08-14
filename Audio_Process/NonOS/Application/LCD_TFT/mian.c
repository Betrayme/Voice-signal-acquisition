/*
 * 			广州创龙电子科技有限公司
 *
 * Copyright 2015 Tronlong All rights reserved
 */

/*
 * 功能描述：
 *     本例程使用GPIO驱动4.3寸TFT LCD(SSD1963),运行程序后会在屏幕上显示“广州
 * 创龙电子科技有限公司”
 *
 */

#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "LCD.h"
/**************************************************************************/

/**************************************************************************/
//32*32 "广州创龙电子科技有限公司"  512
unsigned char hanzi32x32[]={
		//广
		//超级字符/图形字模提取软件 V1.0
		//C51 Code,逐行从上到下取模,每行左为最高位
		//Creat By Super Led_Dot_Matrix Char V1.0
		0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x03,0x80,0x00,0x00,0x03,0x80,0x00,0x00,0x01,0xC0,0x00,0x00,0x01,0xC0,0x00,0x0F,
		0xFF,0xFF,0xF8,0x0F,0xFF,0xFF,0xF8,0x0F,0xFF,0xFF,0xF8,0x0E,0x00,0x00,0x00,0x0E,0x00,0x00,0x00,0x0E,0x00,0x00,0x00,0x0E,0x00,
		0x00,0x00,0x0E,0x00,0x00,0x00,0x0E,0x00,0x00,0x00,0x0E,0x00,0x00,0x00,0x0E,0x00,0x00,0x00,0x0E,0x00,0x00,0x00,0x0E,0x00,0x00,
		0x00,0x0E,0x00,0x00,0x00,0x0E,0x00,0x00,0x00,0x0C,0x00,0x00,0x00,0x1C,0x00,0x00,0x00,0x1C,0x00,0x00,0x00,0x1C,0x00,0x00,0x00,
		0x3C,0x00,0x00,0x00,0x38,0x00,0x00,0x00,0x78,0x00,0x00,0x00,0xF0,0x00,0x00,0x00,0x70,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,
		//州
		//超级字符/图形字模提取软件 V1.0
		//C51 Code,逐行从上到下取模,每行左为最高位
		//Creat By Super Led_Dot_Matrix Char V1.0
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xC0,0x00,0xE0,0x01,0xC0,0xC0,0xE0,0x01,0xC0,0xC0,0xE0,0x01,0xC0,0xC0,0x60,0x01,
		0x80,0xC0,0x60,0x01,0x80,0xC0,0x60,0x01,0x80,0xC0,0x60,0x01,0x80,0xC0,0x60,0x19,0x80,0xC0,0x60,0x19,0xB8,0xD8,0x60,0x19,0x98,
		0xDC,0x60,0x39,0x98,0xCC,0x60,0x39,0x9C,0xCE,0x60,0x39,0x8C,0xC6,0x60,0x39,0x8C,0xC7,0x60,0x71,0x8E,0xC3,0x60,0x71,0x80,0xC2,
		0x60,0x11,0x80,0xC0,0x60,0x01,0x80,0xC0,0x60,0x03,0x80,0xC0,0x60,0x03,0x80,0xC0,0x60,0x03,0x00,0xC0,0x60,0x07,0x00,0xC0,0x60,
		0x07,0x00,0xC0,0x60,0x0E,0x00,0xC0,0x60,0x1E,0x00,0xC0,0xE0,0x3C,0x01,0xC0,0xE0,0x38,0x00,0x00,0xE0,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,
		//创
		//超级字符/图形字模提取软件 V1.0
		//C51 Code,逐行从上到下取模,每行左为最高位
		//Creat By Super Led_Dot_Matrix Char V1.0
		0x00,0x00,0x00,0x00,0x00,0x40,0x00,0x00,0x00,0xF0,0x00,0x70,0x00,0xE0,0x00,0x70,0x01,0xF0,0x00,0x70,0x03,0xF8,0x00,0x70,0x03,
		0x9E,0x1C,0x70,0x07,0x0F,0x18,0x70,0x0E,0x07,0x98,0x70,0x1E,0x03,0xD8,0x70,0x3C,0x01,0x98,0x70,0x7F,0xFE,0x18,0x70,0xFF,0xFE,
		0x18,0x70,0x6F,0xFE,0x18,0x70,0x0E,0x0E,0x18,0x70,0x0E,0x0E,0x18,0x70,0x0E,0x0E,0x18,0x70,0x0E,0x0E,0x18,0x70,0x0E,0x0E,0x18,
		0x70,0x0E,0x0E,0x18,0x70,0x0E,0x7E,0x18,0x70,0x0E,0x7C,0x18,0x70,0x0E,0x38,0x18,0x70,0x0E,0x00,0x18,0x70,0x0E,0x01,0x80,0x70,
		0x0E,0x01,0xC0,0x70,0x0E,0x03,0x80,0x70,0x0F,0xFF,0x83,0xF0,0x07,0xFF,0x03,0xF0,0x03,0xFE,0x01,0xE0,0x00,0x00,0x01,0x00,0x00,
		0x00,0x00,0x00,
		//龙
		//超级字符/图形字模提取软件 V1.0
		//C51 Code,逐行从上到下取模,每行左为最高位
		//Creat By Super Led_Dot_Matrix Char V1.0
		0x00,0x00,0x00,0x00,0x00,0x0E,0x00,0x00,0x00,0x0E,0x00,0x00,0x00,0x0E,0x0C,0x00,0x00,0x0E,0x1E,0x00,0x00,0x0E,0x0F,0x00,0x00,
		0x0C,0x07,0x80,0x00,0x0C,0x02,0x00,0x00,0x0C,0x00,0x00,0x3F,0xFF,0xFF,0xF8,0x3F,0xFF,0xFF,0xF8,0x00,0x1C,0xC0,0x00,0x00,0x1C,
		0xC0,0x00,0x00,0x1C,0xC0,0x00,0x00,0x18,0xC0,0x80,0x00,0x38,0xC1,0xE0,0x00,0x38,0xC3,0xC0,0x00,0x38,0xC3,0x80,0x00,0x70,0xC7,
		0x00,0x00,0x70,0xC7,0x00,0x00,0xE0,0xCE,0x00,0x01,0xE0,0xDC,0x00,0x01,0xC0,0xFC,0x00,0x03,0xC0,0xF8,0x00,0x07,0x81,0xE0,0x00,
		0x0F,0x03,0xE0,0x18,0x1E,0x0F,0xC0,0x1C,0x7C,0x3F,0xE0,0x1C,0x78,0x7C,0xFF,0xF8,0x30,0x30,0xFF,0xF0,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,
		//电
		//超级字符/图形字模提取软件 V1.0
		//C51 Code,逐行从上到下取模,每行左为最高位
		//Creat By Super Led_Dot_Matrix Char V1.0
		0x00,0x00,0x00,0x00,0x00,0x0E,0x00,0x00,0x00,0x0E,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x06,0x00,0x00,0x3F,
		0xFF,0xFF,0x80,0x3F,0xFF,0xFF,0x80,0x3F,0xFF,0xFF,0x80,0x38,0x06,0x01,0x80,0x38,0x06,0x01,0x80,0x38,0x06,0x01,0x80,0x38,0x06,
		0x01,0x80,0x3F,0xFF,0xFF,0x80,0x3F,0xFF,0xFF,0x80,0x3F,0xFF,0xFF,0x80,0x38,0x06,0x01,0x80,0x38,0x06,0x01,0x80,0x38,0x06,0x01,
		0x80,0x38,0x06,0x01,0x80,0x3F,0xFF,0xFF,0x80,0x3F,0xFF,0xFF,0x80,0x3F,0xFF,0xFF,0x80,0x38,0x06,0x00,0x00,0x38,0x06,0x00,0x10,
		0x00,0x06,0x00,0x1C,0x00,0x06,0x00,0x1C,0x00,0x07,0x00,0x3C,0x00,0x07,0xFF,0xF8,0x00,0x07,0xFF,0xF0,0x00,0x01,0xFF,0xE0,0x00,
		0x00,0x00,0x00,
		//子
		//超级字符/图形字模提取软件 V1.0
		//C51 Code,逐行从上到下取模,每行左为最高位
		//Creat By Super Led_Dot_Matrix Char V1.0
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xFF,0xFF,0xC0,0x0F,0xFF,0xFF,0xC0,0x0F,0xFF,0xFF,0xC0,0x00,
		0x00,0x1F,0x80,0x00,0x00,0x3E,0x00,0x00,0x00,0xF8,0x00,0x00,0x03,0xE0,0x00,0x00,0x03,0xC0,0x00,0x00,0x03,0x00,0x00,0x00,0x03,
		0x00,0x00,0x00,0x03,0x00,0x00,0x7F,0xFF,0xFF,0xFC,0x7F,0xFF,0xFF,0xFC,0x7F,0xFF,0xFF,0x80,0x00,0x03,0x00,0x00,0x00,0x03,0x00,
		0x00,0x00,0x03,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x03,0x00,0x00,
		0x00,0x03,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,
		//科
		//超级字符/图形字模提取软件 V1.0
		//C51 Code,逐行从上到下取模,每行左为最高位
		//Creat By Super Led_Dot_Matrix Char V1.0
		0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x80,0x00,0x30,0x01,0xC0,0x07,0xF8,0x01,0xC0,0x7F,0xF8,0x41,0xC0,0x3F,0x80,0xE1,0xC0,0x31,
		0x80,0xF1,0xC0,0x01,0x80,0x79,0xC0,0x01,0x80,0x39,0xC0,0x01,0x80,0x1D,0xC0,0x01,0x80,0x19,0xC0,0x7F,0xFC,0x01,0xC0,0x7F,0xFC,
		0x81,0xC0,0x03,0x81,0xE1,0xC0,0x03,0x80,0xE1,0xC0,0x07,0xB0,0x71,0xC0,0x07,0xB8,0x3D,0xC0,0x0F,0x9C,0x19,0xC0,0x0F,0x8E,0x11,
		0xFC,0x1D,0x84,0x0F,0xFC,0x1D,0x81,0xFF,0xF8,0x39,0x87,0xFF,0xC0,0x79,0x87,0xE1,0xC0,0xF1,0x86,0x01,0xC0,0x61,0x80,0x01,0xC0,
		0x01,0x80,0x01,0xC0,0x01,0x80,0x01,0xC0,0x01,0x80,0x01,0xC0,0x01,0x80,0x01,0xC0,0x03,0x80,0x01,0xC0,0x03,0x80,0x01,0xC0,0x00,
		0x00,0x00,0x00,
		//技
		//超级字符/图形字模提取软件
		//C51 Code,逐行从上到下取模,每行左为最高位
		//Creat By Super Led_Dot_Matrix Char V1.0
		0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x07,0x00,0x1C,0x00,0x07,0x00,0x1C,0x00,0x07,0x00,0x1C,0x00,0x07,0x00,0x1C,0x00,0x07,
		0x0F,0xFF,0xF8,0x07,0x0F,0xFF,0xF8,0x7F,0xEF,0xFF,0xF8,0x7F,0xE0,0x1C,0x00,0x07,0x00,0x1C,0x00,0x07,0x00,0x1C,0x00,0x07,0x00,
		0x1C,0x00,0x07,0x07,0xFF,0xF0,0x07,0x67,0xFF,0xF0,0x07,0xE7,0xFF,0xE0,0x0F,0xC1,0x80,0xE0,0x3F,0x01,0x80,0xE0,0x7F,0x01,0xC1,
		0xC0,0x77,0x00,0xC1,0xC0,0x47,0x00,0xE3,0x80,0x07,0x00,0x63,0x80,0x07,0x00,0x77,0x00,0x07,0x00,0x3E,0x00,0x07,0x00,0x1C,0x00,
		0x07,0x00,0x7F,0x00,0x07,0x03,0xF7,0xC0,0x3F,0x0F,0xE3,0xF0,0x3F,0x3F,0x80,0xFC,0x3C,0x1C,0x00,0x38,0x10,0x10,0x00,0x08,0x00,
		0x00,0x00,0x00,
		//有
		//超级字符/图形字模提取软件 V1.0
		//C51 Code,逐行从上到下取模,每行左为最高位
		//Creat By Super Led_Dot_Matrix Char V1.0
		0x00,0x00,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x0E,0x00,0x00,0x3F,0xFF,0xFF,0xF8,0x3F,
		0xFF,0xFF,0xFC,0x3F,0xFF,0xFF,0xFC,0x00,0x1C,0x00,0x00,0x00,0x38,0x00,0x00,0x00,0x78,0x00,0x00,0x00,0x7F,0xFF,0xE0,0x00,0xFF,
		0xFF,0xE0,0x01,0xE0,0x00,0xE0,0x03,0xE0,0x00,0xE0,0x07,0xE0,0x00,0xE0,0x0F,0xFF,0xFF,0xE0,0x1E,0xFF,0xFF,0xE0,0x7C,0xFF,0xFF,
		0xE0,0x78,0xE0,0x00,0xE0,0x30,0xE0,0x00,0xE0,0x00,0xFF,0xFF,0xE0,0x00,0xFF,0xFF,0xE0,0x00,0xFF,0xFF,0xE0,0x00,0xE0,0x00,0xE0,
		0x00,0xE0,0x00,0xE0,0x00,0xE0,0x00,0xE0,0x00,0xE0,0x0F,0xE0,0x00,0xE0,0x07,0xC0,0x00,0xE0,0x07,0x80,0x00,0xE0,0x02,0x00,0x00,
		0x00,0x00,0x00,
		//限
		//超级字符/图形字模提取软件
		//C51 Code,逐行从上到下取模,每行左为最高位
		//Creat By Super Led_Dot_Matrix Char V1.0
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0xE3,0xFF,0xE0,0x3F,0xE3,0xFF,0xE0,0x3F,0xE3,0xFF,0xE0,0x38,0xE3,0x00,0x60,0x38,
		0xC3,0x00,0x60,0x38,0xC3,0x00,0x60,0x39,0xC3,0xFF,0xE0,0x39,0xC3,0xFF,0xE0,0x39,0x83,0x00,0x60,0x3B,0x83,0x00,0x60,0x3B,0x83,
		0x00,0x60,0x39,0x83,0xFF,0xE0,0x39,0xC3,0xFF,0xE0,0x38,0xC3,0x18,0x00,0x38,0x63,0x18,0x20,0x38,0x63,0x1C,0x30,0x38,0x63,0x0C,
		0xF0,0x38,0x63,0x0D,0xE0,0x38,0x63,0x0F,0x80,0x3B,0xE3,0x06,0x00,0x3B,0xC3,0x07,0x00,0x39,0x83,0x03,0x80,0x38,0x03,0x13,0xC0,
		0x38,0x03,0x31,0xE0,0x38,0x03,0xF0,0xF8,0x38,0x07,0xE0,0x7C,0x38,0x07,0x80,0x38,0x38,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,
		//公
		//超级字符/图形字模提取软件 V1.0
		//C51 Code,逐行从上到下取模,每行左为最高位
		//Creat By Super Led_Dot_Matrix Char V1.0
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x70,0x00,0x00,0x38,0x70,0x00,0x00,0x70,0x38,0x00,0x00,0x70,0x38,0x00,0x00,
		0xE0,0x1C,0x00,0x00,0xE0,0x1C,0x00,0x01,0xC0,0x0E,0x00,0x01,0xC0,0x0F,0x00,0x03,0x80,0x07,0x00,0x07,0x00,0x03,0x80,0x0F,0x00,
		0x03,0xC0,0x0E,0x06,0x01,0xE0,0x1C,0x0F,0x00,0xF0,0x38,0x0E,0x00,0x7C,0x78,0x1C,0x00,0x38,0x30,0x1C,0x00,0x10,0x00,0x38,0x00,
		0x00,0x00,0x38,0x08,0x00,0x00,0x70,0x1C,0x00,0x00,0x70,0x1E,0x00,0x00,0xE0,0x0E,0x00,0x01,0xC0,0x07,0x00,0x01,0xC0,0x0F,0x80,
		0x03,0x9F,0xFF,0x80,0x0F,0xFF,0xFF,0xC0,0x07,0xFF,0xC1,0xC0,0x07,0xE0,0x00,0xE0,0x00,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,
		//司
		//超级字符/图形字模提取软件
		//C51 Code,逐行从上到下取模,每行左为最高位
		//Creat By Super Led_Dot_Matrix Char V1.0
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0xFF,0xFF,0xF0,0x1F,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x70,0x00,
		0x00,0x00,0x70,0x00,0x00,0x00,0x70,0x7F,0xFF,0xFC,0x70,0x7F,0xFF,0xFC,0x70,0x7F,0xFF,0xFC,0x70,0x00,0x00,0x00,0x70,0x00,0x00,
		0x00,0x70,0x00,0x00,0x00,0x70,0x0F,0xFF,0xF0,0x70,0x0F,0xFF,0xF0,0x70,0x0F,0xFF,0xF0,0x70,0x0E,0x00,0x70,0x70,0x0E,0x00,0x70,
		0x70,0x0E,0x00,0x70,0x70,0x0E,0x00,0x70,0x70,0x0F,0xFF,0xF0,0x70,0x0F,0xFF,0xF0,0x70,0x0F,0xFF,0xF0,0x70,0x0E,0x00,0x70,0x70,
		0x0E,0x00,0x70,0x70,0x0E,0x00,0x00,0x70,0x00,0x00,0x0F,0xF0,0x00,0x00,0x07,0xF0,0x00,0x00,0x03,0xC0,0x00,0x00,0x02,0x00,0x00,
		0x00,0x00,0x00,

};

/**************************************************************************/
void main(void)
{
// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the DSP2833x_SysCtrl.c file.
   InitSysCtrl();

// Step 2. Clear all interrupts and initialize PIE vector table:
// Disable CPU interrupts
   DINT;

// Step 3. Initialize the PIE control registers to their default state.
// The default state is all PIE interrupts disabled and flags
// are cleared.
// This function is found in the DSP2833x_PieCtrl.c file.
   InitPieCtrl();

// Step 4.Disable CPU interrupts and clear all CPU interrupt flags:
   IER = 0x0000;
   IFR = 0x0000;
//Step 5. Initialize PieVectTable
   InitPieVectTable();

   SCIStdioInit();
   SCIPuts("\r\n ============Test Start===========.\r\n", -1);
   SCIPuts("Welcome to TL28335 LCD TFT(4.3 inch SSD1963) Demo application.\r\n\r\n", -1);

   Init_Port();
   Lcd_Init();

   LCD_ShowHanZis(48,100,&hanzi32x32[0],32,32,12,RED);

   SCIPuts("\r\n ============Test End===========.\r\n", -1);

   while(1);
}






