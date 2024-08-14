/*
 * 			广州创龙电子科技有限公司
 *
 * Copyright 2015 Tronlong All rights reserved
 */

/*
 * 功能描述：
 *     本例程通过SPI驱动DA芯片(TLC5615)，DA输出精度为10-bit，输出电压范围0~5v，
 * 本例程输出电压为3.3V。
 *
 */

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

void WriteDAC(unsigned int data);
void delay(unsigned int t);
void spi_xmit(Uint16 a);
void spi_fifo_init(void);
void spi_init(void);

void main(void)
{   
	/*初始化系统*/
	InitSysCtrl();

	/*初始化GPIO;*/
	InitSpiaGpio();

	///初始化IO口
	
	EALLOW;
	// 配置SN74HC138
	GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 0; // 配置GPIO11为GPIO口
	GpioCtrlRegs.GPADIR.bit.GPIO11 = 1;      // 定义GPIO17输出引脚
	GpioCtrlRegs.GPAPUD.bit.GPIO11 = 0;      // 禁止上啦 GPIO17引脚
	GpioDataRegs.GPADAT.bit.GPIO11 = 0;      // 输出低电平
	EDIS;
	
	/* 关中断 */
	DINT;
	IER = 0x0000;
	IFR = 0x0000;
	
	/* 初始化PIE控制寄存器 */
	InitPieCtrl();
	
	/* 初始化PIE参数表 */
	InitPieVectTable();	
	
	SCIStdioInit();
    SCIPuts("\r\n ============Test Start===========.\r\n", -1);
    SCIPuts("Welcome to TL28335 DAC(TLC5615) Demo application.\r\n\r\n", -1);

	///初始化SPI
    spi_init();		  // 初始化SPI

    SCIPuts("The DAC output voltage: 3.3v\r\n\r\n", -1);
    WriteDAC(0x2a3);  //输出3.3v电压，函数传入参数 = 输出电压*1024/5

    SCIPuts("\r\n ============Test End===========.\r\n", -1);
    while(1)
    {

	}
} 	

	
void WriteDAC(unsigned int data)
{
     data = data << 6;
     //注意这里的有效数据是12位，SPI初始化中也进行了定义
 
    while(SpiaRegs.SPISTS.bit.BUFFULL_FLAG ==1);			//判断SPI的发送缓冲区是否是空的,等于0可写数据
     
    SpiaRegs.SPITXBUF = data;	//把发送的数据写如SPI发送缓冲区

    while( SpiaRegs.SPISTS.bit.BUFFULL_FLAG==1);		//当发送缓冲区出现满标志位时,开始琐存数据
    	 
    DELAY_US(20);

}

void delay(unsigned int t)
{
 
 	while(t>0)
    	t--;
}
//初始化SPI函数
void spi_init()
{    
	SpiaRegs.SPICCR.all =0x00;///进入初始状态，数据在上升沿输出，自测禁止，12位数据模式
	                            
	SpiaRegs.SPICTL.all =0x0006; // 使能主机模式，正常相位，使能主机发送，禁止接收
	                            //溢出中断，禁止SPI中断；
                                   
	SpiaRegs.SPIBRR =0x0031;	//SPI波特率=37.5M/50	=0.75MHZ；							
    SpiaRegs.SPICCR.all =0x8b | 1<<6; //退出初始状态；数据在下降沿输出，自测禁止，12位数据模式
    SpiaRegs.SPIPRI.bit.FREE = 1;  // 自由运行
}






//===========================================================================
// No more.
//===========================================================================
