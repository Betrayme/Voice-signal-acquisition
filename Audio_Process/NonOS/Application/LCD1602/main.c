/*
 * 			广州创龙电子科技有限公司
 *
 * Copyright 2015 Tronlong All rights reserved
 */

/*
 * 功能描述：
 *     本例程使用GPIO驱动LCD1602,运行程序后会在屏幕上显示“Welcome”
 * "www.tronlong.com"
 *
 */

#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

/**************************************************************************/


/****************端口宏定义*****************/
#define LCD_RS GpioDataRegs.GPBDAT.bit.GPIO34
#define LCD_RW GpioDataRegs.GPADAT.bit.GPIO8
#define LCD_EN GpioDataRegs.GPADAT.bit.GPIO9
#define LCD_DB GpioDataRegs.GPCDAT.all

void LCD_init(void);                          //初始化函数
void LCD_write_command(unsigned char command);        //写指令函数
void LCD_write_data(unsigned char dat);               //写数据函数
void show(char *cont);                       //显示子函数
void Init_Port(void);   //端口初始化


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
   SCIPuts("Welcome to TL28335 LCD1602 Demo application.\r\n\r\n", -1);

//Step 6. Initialize GPIO
    Init_Port();

	LCD_init();

	LCD_write_command(0x80);	//第一行数据指针地址
	show("    Welcome");

	LCD_write_command(0xc0);	//第二行数据指针的地址
	show("www.tronlong.com");

	SCIPuts("\r\n ============Test End===========.\r\n", -1);
    while(1)
    {

    }
}


void Init_Port(void)
{
	EALLOW;
	GpioCtrlRegs.GPCMUX1.all = 0;
	GpioCtrlRegs.GPCDIR.all = 0XFFFFFFFF;
	// RW
	GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 0;  // GPIO8
	GpioCtrlRegs.GPADIR.bit.GPIO8 = 1;   // output
	GpioDataRegs.GPACLEAR.bit.GPIO8 = 1;
	// EN
	GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 0;  // GPIO9
	GpioCtrlRegs.GPADIR.bit.GPIO9 = 1;   // output
	GpioDataRegs.GPACLEAR.bit.GPIO9 = 1;
	// RS
	GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 0;  // GPIO34
	GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1;   // output
	GpioDataRegs.GPBCLEAR.bit.GPIO34 = 1;
	EDIS;
}

/*--------------------------------------
;模块名称:show(char *cont)
;功    能:LCD1602显示子函数
;参数说明:cont为显示字符串指针参数
;-------------------------------------*/
void show(char *cont)
{
	while(*cont!=0)
	{
		LCD_write_data(*cont);
		DELAY_US(500);  //延时500us
		cont++;
	}
}

/*--------------------------------------
;模块名称:LCD_write_command();
;功    能:LCD1602写指令函数
;参数说明:command为写命令参数
;-------------------------------------*/
void LCD_write_command(unsigned char command)
{
	unsigned char i;
	Uint32 temp = 0;

	for(i=0;i<8;i++)
	{
		if(command & 0x01)
			temp |= 0x01;

		if(i == 7)
			break;

		temp = temp << 1;
		command >>= 1;
	}
	temp = temp << 8;

	LCD_EN=0;
	LCD_RS=0;         //指令
	LCD_RW=0;         //写入
	LCD_DB=temp;//赋值给D0-D7对应的GPIO79-GPIO72引脚
	DELAY_US(500);
	LCD_EN=1;          //允许
	DELAY_US(1000);  //
	LCD_EN=0;
}

/*--------------------------------------
;模块名称:LCD_write_data();
;功    能:LCD1602写数据函数
;参数说明:dat为写数据参数
;-------------------------------------*/
void LCD_write_data(unsigned char dat)
{
	unsigned char i;
	Uint32 temp = 0;

	for(i=0;i<8;i++)
	{
		if(dat & 0x01)
			temp |= 0x01;

		if(i == 7)
			break;

		temp = temp << 1;
		dat >>= 1;
	}

	temp = temp << 8;
	LCD_EN=0;
	LCD_RS=1;          //数据
	LCD_RW=0;          //写入
	LCD_DB=temp;//赋值给D0-D7对应的GPIO79-GPIO72引脚
	DELAY_US(500);
	LCD_EN=1;           //允许
	DELAY_US(1000);
	LCD_EN=0;
}
/*--------------------------------------
;模块名称:LCD_init();
;功    能:初始化LCD1602
;占用资源:--
;参数说明:--
;-------------------------------------*/
void LCD_init(void)
{
	DELAY_US(15000);//延迟15ms
	LCD_write_command(0x38);//设置8位格式，2行，5x7
	DELAY_US(5000);//延迟5ms
	LCD_write_command(0x38);//设置8位格式，2行，5x7
	DELAY_US(5000);
	LCD_write_command(0x38);//设置8位格式，2行，5x7
	LCD_write_command(0x38);//设置8位格式，2行，5x7
	LCD_write_command(0x08);//关显示，不显示光标，光标不闪烁；
	LCD_write_command(0x01);//清除屏幕显示：数据指针清零，所有显示清零；
	LCD_write_command(0x06);//设定输入方式，增量不移位
	LCD_write_command(0x0c);//整体显示，关光标，不闪烁
}


