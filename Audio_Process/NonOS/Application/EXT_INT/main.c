/*
 * 			广州创龙电子科技有限公司
 *
 * Copyright 2015 Tronlong All rights reserved
 */

/*
 * 功能描述：
 *     本例程使用外部中断检测KEY1和KEY2，当KEY1按下时LED1电平翻转，当KEY2按下时
 * LED2电平翻转。
 */

#include "DSP2833x_Device.h"     // Headerfile Include File
#include "DSP2833x_Examples.h"   // Examples Include File

// 中断服务子程序在主程序下面时，需要在主程序之前要先申明才能使用.
interrupt void xint1_isr(void);
interrupt void xint2_isr(void);

// 本例中用到的全局变量；
volatile Uint32 Xint1Count; //发生外部中断1的次数；
volatile Uint32 Xint2Count; //发生外部中断2的次数；
volatile Uint32 Xint2Loop; //给外部中断2的for循环变量

#define keyon1 GpioDataRegs.GPADAT.bit.GPIO24
#define keyon2 GpioDataRegs.GPADAT.bit.GPIO25

#define DELAY 35.700L

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

    EALLOW;
	GpioCtrlRegs.GPBMUX2.bit.GPIO50 = 0;  // GPIO50
	GpioCtrlRegs.GPBMUX2.bit.GPIO51 = 0;  // GPIO51
	GpioCtrlRegs.GPBMUX2.bit.GPIO50 = 0;  // GPIO52
	GpioCtrlRegs.GPBMUX2.bit.GPIO51 = 0;  // GPIO53

	GpioCtrlRegs.GPBDIR.bit.GPIO50 = 1;   // output
	GpioCtrlRegs.GPBDIR.bit.GPIO51 = 1;   // output
	GpioCtrlRegs.GPBDIR.bit.GPIO52 = 1;   // output
	GpioCtrlRegs.GPBDIR.bit.GPIO53 = 1;   // output

	// GPIO50 = 0; GPIO51 = 0; GPIO52 = 0; GPIO53 = 0;
	GpioDataRegs.GPBCLEAR.all  =0x003c0000;
	EDIS;

//  本例中用到的中断重新映射到本件的中断服务子程序； 
   EALLOW;	// 修改被保护的寄存器，修改前应添加EALLOW语句
   PieVectTable.XINT1 = &xint1_isr;
   PieVectTable.XINT2 = &xint2_isr;
   EDIS;   // EDIS的意思是不允许修改被保护的寄存器
	
// 步骤 4. 初始化片内外设:
// 这个函数可以在DSP280x_CpuTimers.c源文件中找到
// InitPeripherals(); // 本例中不需要
 
// 步骤 5. 用户特定的代码来允许需要的中断

// 清除计数器
   Xint1Count = 0; // 计数外部中断1（XINT1）
   Xint2Count = 0; // 计数外部中断2（XINT2） 

// 在外设中断扩展模块中断使能寄存器中允许XINT1和XINT2: （组1中断4 & 5）
// 使能CPU中断1（INT1）:
   PieCtrlRegs.PIECTRL.bit.ENPIE = 1;          // 使能外设中断扩展PIE模块
   PieCtrlRegs.PIEIER1.bit.INTx4 = 1;          // 使能PIE组1的INT4
   PieCtrlRegs.PIEIER1.bit.INTx5 = 1;          // 使能PIE组1的INT5   
   IER |= M_INT1;                              // 使能CPU中断1（INT1）
   EINT;                                       // 开全局中断


   EALLOW;

  GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 0;         // GPIO
  GpioCtrlRegs.GPADIR.bit.GPIO24 = 0;          // input
  GpioCtrlRegs.GPAQSEL2.bit.GPIO24 = 0;

  GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 0;         // GPIO
  GpioCtrlRegs.GPADIR.bit.GPIO25 = 0;          // input
  GpioCtrlRegs.GPAQSEL2.bit.GPIO25 = 0;

  EDIS;


// 通过GPIO外部中断选择寄存器，选择GPIO24为外部中断1，选择GPIO25为外部中断2
   EALLOW;
   GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 24;   // XINT1是GPIO24
   GpioIntRegs.GPIOXINT2SEL.bit.GPIOSEL = 25;   // XINT2是GPIO25
   EDIS;

// 配置外部中断1和2的中断控制寄存器
   XIntruptRegs.XINT1CR.bit.POLARITY = 0;      // 下降沿触发中断
   XIntruptRegs.XINT2CR.bit.POLARITY = 0;      // 下降沿触发中断
   
// 使能外部中断1和外部中断2
   XIntruptRegs.XINT1CR.bit.ENABLE = 1;        // 使能XINT1
   XIntruptRegs.XINT2CR.bit.ENABLE = 1;        // 使能XINT2
   
// 步骤6. 无限循环:	
   for(;;);
	
} 	

//步骤7.在这里插入中断服务子程序，如果需要使用中断服务子程序，必须在步骤5中重新定义中断
//向量表中对应地地址
interrupt void xint1_isr(void)
{
	Uint32 i;
	for(i=0;i<1000000;i++);    //键盘消抖动
	while(keyon1==0);

	GpioDataRegs.GPBTOGGLE.bit.GPIO50=1;

	Xint1Count++;
		
	// 应答寄存器的位1清0，以响应同组内其他中断；
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

interrupt void xint2_isr(void)
{
	Uint32 i;
	for(i=0;i<1000000;i++);    //键盘消抖动
	while(keyon2==0);

	GpioDataRegs.GPBTOGGLE.bit.GPIO51=1;

	Xint1Count++;
	
	// 应答寄存器的位1清0，以响应同组内其他中断；
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

}



//===========================================================================
// No more.
//===========================================================================



