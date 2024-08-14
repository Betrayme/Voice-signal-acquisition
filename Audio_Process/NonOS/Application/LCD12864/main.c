/*
 * 			广州创龙电子科技有限公司
 *
 * Copyright 2015 Tronlong All rights reserved
 */

/*
 * 功能描述：
 *     本例程使用GPIO驱动LCD12864,运行程序后会在屏幕上显示“广州
 * 创龙电子”"www.tronlong.com"
 *
 */

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

/***************全局变量定义****************/

/****************端口宏定义*****************/
#define LCD_RS GpioDataRegs.GPBDAT.bit.GPIO34
#define LCD_RW GpioDataRegs.GPADAT.bit.GPIO8
#define LCD_EN GpioDataRegs.GPADAT.bit.GPIO9
#define LCD_DB GpioDataRegs.GPCDAT.all

/*****汉字地址表*****/
unsigned char addr_tab[]={      //便于根据汉字坐标求出地址 
		0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,//第一行汉字位置 
		0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,//第二行汉字位置 
		0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,//第三行汉字位置 
		0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,//第四行汉字位置 
};

// 使用前，声明本文件中的相关函数
void Init_Port(void);
void LCD_init(void);                          //初始化函数
void LCD_write_command(unsigned char command);        //写指令函数
void LCD_write_data(unsigned char dat);               //写数据函数
void Disp_character(int x, int y,char *s);

void main(void)
{
// 步骤 1. 初始化系统控制:
// 设置PLL, WatchDog, 使能外设时钟
// 下面这个函数可以从DSP280x_SysCtrl.c文件中找到.
   InitSysCtrl();

// 步骤 2. 初始化通用输入输出多路复用器GPIO:
// 这个函数在DSP280x_Gpio.c源文件中被定义了
// 这个函数使GPIO控制类寄存器初始化到默认状态
// InitGpio();  // Skipped for this example  

//本例仅初始化液晶对应的GPIO引脚；
//这些函数在DSP280x_EPwm.c文件里面



// 步骤 3. 清除所有中断,初始化中断向量表:
// 禁止CPU全局中断
   DINT;

// 初始化PIE控制寄存器到他们的默认状态.
// 这个默认状态就是禁止PIE中断及清除所有PIE中断标志
// 这个函数放在DSP280x_PieCtrl.c源文件里
   InitPieCtrl();
   
// 禁止CPU中断和清除所有CPU中断标志
   IER = 0x0000;
   IFR = 0x0000;

//初始化PIE中断向量表，并使其指向中断服务子程序（ISR）
// 这些中断服务子程序被放在了DSP280x_DefaultIsr.c源文件中
// 这个函数放在了DSP280x_PieVect.c源文件里面.
   InitPieVectTable();

   SCIStdioInit();
   SCIPuts("\r\n ============Test Start===========.\r\n", -1);
   SCIPuts("Welcome to TL28335 LCD12864 Demo application.\r\n\r\n", -1);

// 步骤 4.初始化片内外设:
// InitPeripherals();  // Not required for this example


   
// 步骤 5. 以下是用户编写的指定代码, 使能相关中断:
   Init_Port();

// 步骤 6. 循环函数，显示字符串:
     LCD_init();

	 LCD_write_command(0x80);	//第一行数据指针地址
	 LCD_write_data(0xb9); //广
     LCD_write_data(0xe3);
     LCD_write_data(0xd6); //州
     LCD_write_data(0xdd);
     LCD_write_data(0xb4); //创
     LCD_write_data(0xb4);
     LCD_write_data(0xc1); //龙
     LCD_write_data(0xfa);
     LCD_write_data(0xb5); //电
     LCD_write_data(0xe7);
     LCD_write_data(0xd7); //子
     LCD_write_data(0xd3);
     LCD_write_data(0xbf); //科
     LCD_write_data(0xc6);
     LCD_write_data(0xbc); //技
     LCD_write_data(0xbc);

	 Disp_character(2,0,"www.tronlong.com");
	 DELAY_US(100);

	 SCIPuts("\r\n ============Test End===========.\r\n", -1);
      while(1);
} 

/*------------------------------------------*/
/*形式参数：void                            */
/*返回值:void				                */
/*函数描述:初始化gpio端口               */
/*------------------------------------------*/ 
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

	LCD_RS=0;         //指令
	LCD_RW=0;          //写入
	LCD_EN=1;          //允许
	LCD_DB=temp;//赋值给D0-D7对应的GPIO79-GPIO72引脚
	DELAY_US(10000);
	LCD_EN=0;
	DELAY_US(100);

}

/*-------------------------------------- 
;模块名称:LCD_write_data();
;功    能:LCD1602写数据函数
;占用资源: P2.0--RS(LCD_RS),P2.1--RW(LCD_RW),P2.2--E(LCD_E).
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

	LCD_RS=1;         //指令
	LCD_RW=0;          //写入
	LCD_EN=1;          //允许
	LCD_DB=temp;//赋值给D0-D7对应的GPIO79-GPIO72引脚
	DELAY_US(10000);
	LCD_EN=0;
	DELAY_US(100);
}
/*-------------------------------------- 
;模块名称:LCD_init();
;功    能:初始化LCD1602
;占用资源:--
;参数说明:--
;-------------------------------------*/ 
void LCD_init(void)
{
	DELAY_US(100000);//延迟100ms
	LCD_write_command(0x30);//功能设置
	DELAY_US(200);//延迟200us
	LCD_write_command(0x30);//功能设置
	DELAY_US(200);//延迟200us
	LCD_write_command(0x0c);//显示开关控制
	DELAY_US(200);//延迟200us
	LCD_write_command(0x01);//显示清除
	DELAY_US(12000);//延迟12ms
	LCD_write_command(0x06);//显示清除
	DELAY_US(500);//延迟500us
}

void Disp_character(int x, int y, char *s)
{
	LCD_write_command(addr_tab[8*x+y]);   //写地址

	DELAY_US(200);

	while(*s > 0)
	{
		LCD_write_data(*s);   //写数据
		DELAY_US(200);
		s++;
	}
}



//===========================================================================
// No more.
//===========================================================================
