/*
 * 			广州创龙电子科技有限公司
 *
 * Copyright 2015 Tronlong All rights reserved
 */

/*
 * 功能描述：
 *     本例程通过PWM驱动蜂鸣器发声，通过按键可以调节PWM占空比，KEY1占空比增加,
 * KEY2占空比减小。
 *
 */
//###########################################################################

// DESCRIPTION:
//
//
//
////###########################################################################
// $TI Release: F2833x/F2823x Header Files and Peripheral Examples V140 $
// $Release Date: March  4, 2015 $
// $Copyright: Copyright (C) 2007-2015 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

// 使用前，声明本文件中的相关函数；
void InitEPwm6Example(void);
interrupt void epwm6_isr(void);
void Scan_Key(void);
void Manage_Up(void);
void Manage_Down(void);
void Motor_Drive(void);
void Init_Port(void);
void Buzzer_Drive(void);

/***************全局变量定义****************/
int32 temp=0;  //高电平时间

// 宏定义每个定时器周期寄存器的周期值；
#define EPWM6_TIMER_TBPRD  12500  // 周期值
#define EPWM6_MAX_CMPA     12500
#define EPWM6_MIN_CMPA       0
#define EPWM6_MAX_CMPB     12500
#define EPWM6_MIN_CMPB       0
/****************端口宏定义*****************/
#define Up_Key GpioDataRegs.GPADAT.bit.GPIO24
#define Down_Key GpioDataRegs.GPADAT.bit.GPIO25

void main(void)
{
// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the DSP2833x_SysCtrl.c file.
   InitSysCtrl();

// Step 2. Initalize GPIO:
// This example function is found in the DSP2833x_Gpio.c file and
// illustrates how to set the GPIO to it's default state.
// InitGpio();  // Skipped for this example

// For this case just init GPIO pins for ePWM1, ePWM2, ePWM3
// These functions are in the DSP2833x_EPwm.c file
   InitEPwm6Gpio();
   Init_Port();

// Step 3. Clear all interrupts and initialize PIE vector table:
// Disable CPU interrupts
   DINT;

// Initialize the PIE control registers to their default state.
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

   SCIStdioInit();
   SCIPuts("\r\n ============Test Start===========.\r\n", -1);
   SCIPuts("Welcome to TL28335 Buzzer Demo application.\r\n\r\n", -1);

// Interrupts that are used in this example are re-mapped to
// ISR functions found within this file.
   EALLOW;  // This is needed to write to EALLOW protected registers
   PieVectTable.EPWM6_INT = &epwm6_isr;
   EDIS;    // This is needed to disable write to EALLOW protected registers

// Step 4. Initialize all the Device Peripherals:
// This function is found in DSP2833x_InitPeripherals.c
// InitPeripherals();  // Not required for this example

// For this example, only initialize the ePWM
   EALLOW;
   SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
   EDIS;

   InitEPwm6Example();

   EALLOW;
   SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
   EDIS;

// Step 5. User specific code, enable interrupts:

// Enable CPU INT3 which is connected to EPWM1-6 INT:
   IER |= M_INT3;

// Enable EPWM INTn in the PIE: Group 3 interrupt 6
   PieCtrlRegs.PIEIER3.bit.INTx6 = 1;

// Enable global Interrupts and higher priority real-time debug events:
   EINT;   // Enable Global interrupt INTM
   ERTM;   // Enable Global realtime interrupt DBGM

// Step 6. IDLE loop. Just sit and loop forever (optional):
   for(;;)
   {
       asm("          NOP");
   }
}

interrupt void epwm6_isr(void)
{
	if((Up_Key==0)|(Down_Key==0))//扫描是否按键按下
	{
		Scan_Key();
		// 更新CMPA和CMPB比较寄存器的值
		Buzzer_Drive();
	}

   // 清除这个定时器的中断标志位
   EPwm6Regs.ETCLR.bit.INT = 1;

   // 清除PIE应答寄存器的第三位，以响应组3内的其他中断请求；
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}

void InitEPwm6Example()
{

  // 设置时间基准的时钟信号（TBCLK）
   EPwm6Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP; // 递增计数模式
   EPwm6Regs.TBPRD = EPWM6_TIMER_TBPRD;       // 设置定时器周期
   EPwm6Regs.TBCTL.bit.PHSEN = TB_DISABLE;    // 禁止相位加载
   EPwm6Regs.TBPHS.half.TBPHS = 0x0000;       // 时基相位寄存器的值赋值0
   EPwm6Regs.TBCTR = 0x0000;                   // 时基计数器清零
   EPwm6Regs.TBCTL.bit.HSPCLKDIV = 6/2;   // 设置时基时钟速率为系统时钟SYSCLKOUT/6=25MHZ;
   EPwm6Regs.TBCTL.bit.CLKDIV = TB_DIV1;//由时基时钟频率和时基周期可知PWM1频率=2KHZ；

   // 设置比较寄存器的阴影寄存器加载条件：时基计数到0
   EPwm6Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
   EPwm6Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
   EPwm6Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
   EPwm6Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

   // 设置比较寄存器的值
   EPwm6Regs.CMPA.half.CMPA = EPWM6_MIN_CMPA;     // 设置比较寄存器A的值
   EPwm6Regs.CMPB = EPWM6_MIN_CMPB;               // 设置比较寄存器B的值

   // 设置动作限定；首先默认为转动方向为正转，这时只有PWM1A输出占空比；
   EPwm6Regs.AQCTLA.bit.ZRO = AQ_SET;            // 计数到0时PWM6A输出高电平
   EPwm6Regs.AQCTLA.bit.CAU = AQ_CLEAR;          // 递增计数时，发生比较寄存器A匹配时清除PWM1A输出

   EPwm6Regs.AQCTLB.bit.ZRO = AQ_CLEAR;            // 计数到0时PWM6B输出低电平
   EPwm6Regs.AQCTLB.bit.CBU = AQ_CLEAR;          // 递增计数时，发生比较寄存器A匹配时清除PWM1B输出
   
   // 3次0匹配事件发生时产生一个中断请求；一次匹配是100us，一共300us产生一次中断；
   EPwm6Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // 选择0匹配事件中断
   EPwm6Regs.ETSEL.bit.INTEN = 1;                // 使能事件触发中断
   EPwm6Regs.ETPS.bit.INTPRD = ET_3RD;           // 3次事件产生中断请求


}
/*------------------------------------------*/
/*形式参数：void                            */
/*返回值:void				                */
/*函数描述:初始化电机控制端口               */
/*------------------------------------------*/ 
void Init_Port(void)
{
	EALLOW;   
// 用户根据需要配置
// 其他不需要的行注释掉；
    GpioCtrlRegs.GPAPUD.bit.GPIO24 = 1;    // 禁止GPIO24 引脚内部上拉
    GpioCtrlRegs.GPAPUD.bit.GPIO25 = 1;    // 禁止GPIO25 引脚内部上拉
    GpioCtrlRegs.GPAPUD.bit.GPIO26 = 1;    // 禁止GPIO26 引脚内部上拉
   
/* 通过I/O功能选择寄存器配置通用I/O功能*/
    GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 0;   // 配置GPIO24为通用I/O口
    GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 0;   // 配置GPIO25为通用I/O口
	GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 0;   // 配置GPIO26为通用I/O口

	//输入数据全部清零   
   GpioDataRegs.GPADAT.bit.GPIO24 = 0x0001;
   GpioDataRegs.GPADAT.bit.GPIO25 = 0x0001;
   GpioDataRegs.GPADAT.bit.GPIO26 = 0x0001;

/* 设置I/O口为输入引脚*/
    GpioCtrlRegs.GPADIR.bit.GPIO24 = 0;   // 配置GPIO24为输入引脚
    GpioCtrlRegs.GPADIR.bit.GPIO25 = 0;   // 配置GPIO25为输入引脚
    GpioCtrlRegs.GPADIR.bit.GPIO26 = 0;   // 配置GPIO26为输入引脚
// 每个输入口可以有不同的输入限定
   // a) 输入与系统时钟 SYSCLKOUT同步
   // b) 输入被指定的采样窗口限定
   // c) 输入异步 (仅对外设输入有效)
   GpioCtrlRegs.GPAQSEL2.all = 0x0000;    // GPIO16-GPIO31与系统时钟SYSCLKOUT 同步
    EDIS;
} 
/*------------------------------------------*/
/*形式参数：void		            		*/
/*返回值:void				    			*/
/*函数描述:扫描键值             	    	*/
/*------------------------------------------*/
void Scan_Key(void)      
{
	unsigned int i;
	for(i=0;i<50000;i++);    //键盘消抖动

	if(Up_Key==0)
		Manage_Up();
	else if(Down_Key==0)
		Manage_Down();

	while((Up_Key==0)|(Down_Key==0));
}

/*------------------------------------------*/
/*形式参数：void		            		*/
/*返回值:void				    			*/
/*函数描述:按键的处理程序    	    		*/
/*------------------------------------------*/
void Manage_Up(void) //加
{
	if(temp <= EPWM6_MAX_CMPA - 500)
		temp=temp+500;
}

void Manage_Down(void)//减
{
	if(temp >= EPWM6_MIN_CMPA + 500)
		temp=temp-500;
} 
/*------------------------------------------*/
/*形式参数：void		            		*/
/*返回值:void				    			*/
/*函数描述:    	    			*/
/*------------------------------------------*/
void Buzzer_Drive(void)
{
	EPwm6Regs.CMPA.half.CMPA = temp;//改变脉宽
	EPwm6Regs.CMPB = temp;//改变脉宽
}




//===========================================================================
// No more.
//===========================================================================
