/*
 * 			广州创龙电子科技有限公司
 *
 * Copyright 2015 Tronlong All rights reserved
 */

/*
 * 功能描述：
 *     程序控制GPIO2直到GPIO5依次输出高电平，则步进电机先顺时钟旋转一段时间，
 * 然后代码控制GPIO5直到GPIO2依次输出高电平,则步进电机逆时针旋转一段时间。
 *
 */

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

// 申明用到的子函数
void delay_loop(Uint32 a);
void Gpio_select(void);

unsigned char Step_table[]={0x0006,0x000a,0x0012,0x0022};
unsigned char Step_table1[]={0x0021,0x0011,0x0009,0x0005};

void main(void)
{
   unsigned int i,j;
// 步骤 1. 初始化系统控制:
// 设置PLL, WatchDog, 使能外设时钟
// 下面这个函数可以从DSP2833x_SysCtrl.c文件中找到.
   InitSysCtrl();
   
// 步骤 2. 初始化通用输入输出多路复用器GPIO: 
// 这个函数在DSP2833x_Gpio.c源文件中被定义了
// 这个函数使GPIO控制类寄存器初始化到默认状态
// InitGpio();  // Skipped for this example
 
// For this example use the following configuration:
   Gpio_select();	  

// Step 3. Clear all interrupts and initialize PIE vector table:
// Disable CPU interrupts 
   DINT;

// Initialize PIE control registers to their default state.
// The default state is all PIE interrupts disabled and flags
// are cleared.  
// This function is found in the DSP2833x_PieCtrl.c file.
   InitPieCtrl();

// Disable CPU interrupts and clear all CPU interrupt flags:
   IER = 0x0000;
   IFR = 0x0000;

// Initialize the PIE vector table with pointers to the shell Interrupt 
// Service Routines (ISR).  
// This will populate the entire table, even if the interrupt
// is not used in this example.  This is useful for debug purposes.
// The shell ISR routines are found in DSP2833x_DefaultIsr.c.
// This function is found in DSP2833x_PieVect.c.
   InitPieVectTable();

	
// Step 4. Initialize all the Device Peripherals:
// This function is found in DSP2833x_InitPeripherals.c
// InitPeripherals(); // Not required for this example
 
   j=512; //改变这个值可以控制转动周数。
   while(j--)
   {
      for(i=0;i<4;i++)
      {
         GpioDataRegs.GPADAT.all=Step_table[i]; //GPIO2-GPIO5依次输出高电平
         delay_loop(80000);
      }
   }
	
	
//反转
	j=512;
	GpioDataRegs.GPADAT.all=0x0000;
	while(j--)
    {
      for(i=0;i<4;i++)
      {
         GpioDataRegs.GPADAT.all=Step_table1[i]; //GPIO5-GPIO2依次输出高电平
         delay_loop(80000);
      }
    }
	while(1);
} 	

//延时函数
void delay_loop(Uint32 a)
{
   while(a--);
}

//初始化GPIO
void Gpio_select(void)
{
    EALLOW;
	GpioCtrlRegs.GPAMUX1.all = 0x00000000;  // All GPIO
	GpioCtrlRegs.GPAMUX2.all = 0x00000000;  // All GPIO
	GpioCtrlRegs.GPBMUX1.all = 0x00000000;  // All GPIO
    GpioCtrlRegs.GPADIR.all = 0xFFFFFFFF;   // All outputs
    GpioCtrlRegs.GPBDIR.all = 0x0000000F;   // All outputs

	 //GPIO0-GPIO31输出低电平
   GpioDataRegs.GPADAT.all=0x00000000;
   EDIS;
}     
//===========================================================================
// No more.
//===========================================================================

