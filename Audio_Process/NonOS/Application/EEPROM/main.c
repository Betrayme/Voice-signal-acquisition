/*
 * 			广州创龙电子科技有限公司
 *
 * Copyright 2015 Tronlong All rights reserved
 */

/*
 * 功能描述：
 *     本例程通过 IIC 向 EEPROM 的 0X0~0xf 地址写入 0x0~0xf 数据，读回并校验数据。
 * 读写数据通过串口b打印。
 *
 */

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

// Note: I2C Macros used in this example can be found in the 
// DSP280x_I2C_defines.h file
extern void writebyte(Uint16 addr,Uint16 data);
extern Uint16 readbyte(Uint16 addr);
extern void Eerom_Gpio_Init(void);


Uint16 addr = 0;
Uint16 RecvBuf[16]={0};
Uint16 TranBuf[16]={0,1,2,3,4,5,6,7,8,9,0xA,0xB,0xC,0xD,0xE,0xF};
Uint32  ErrorCount;
void main(void)
{

// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the DSP280x_SysCtrl.c file.
   InitSysCtrl();						//系统初始化程序


// Step 2. Initalize GPIO: 
// This example function is found in the DSP280x_Gpio.c file and
// illustrates how to set the GPIO to it's default state.
	Eerom_Gpio_Init();					//初始化SCL SDA IO口

// Step 3. Clear all interrupts and initialize PIE vector table:
// Disable CPU interrupts 
   DINT;   

// Initialize PIE control registers to their default state.
// The default state is all PIE interrupts disabled and flags
// are cleared.  
// This function is found in the DSP280x_PieCtrl.c file.
   InitPieCtrl();			

// Disable CPU interrupts and clear all CPU interrupt flags:
   IER = 0x0000;
   IFR = 0x0000;

// Initialize the PIE vector table with pointers to the shell Interrupt 
// Service Routines (ISR).  
// This will populate the entire table, even if the interrupt
// is not used in this example.  This is useful for debug purposes.
// The shell ISR routines are found in DSP280x_DefaultIsr.c.
// This function is found in DSP280x_PieVect.c.
   InitPieVectTable();

   SCIStdioInit();
   SCIPuts("\r\n ============Test Start===========.\r\n", -1);
   SCIPuts("Welcome to TL28335 EEPROM(AT24C02) Demo application.\r\n\r\n", -1);

	for(addr = 0;addr<=0xf;addr++)
	{
		SCIprintf("Write single byte to address %x, value is %x.\r\n",addr, TranBuf[addr]);
		writebyte(addr,TranBuf[addr]);					//写Eeprom
		DELAY_US(50000);  //延时500us
		RecvBuf[addr] = readbyte(addr);					//读Eeprom
		SCIprintf("Read single byte to address %x, value is %x.\r\n\r\n",addr, RecvBuf[addr]);
	}

	ErrorCount = 0;
	for(addr = 0;addr<=0xf;addr++)
	{
		if(TranBuf[addr] != RecvBuf[addr])
			ErrorCount++;
	}

	if(ErrorCount == 0)
	{
	  // OK，数据校验正确
	  SCIPuts("Verify successfully.\r\n", -1);
	}
	else
	{
	  // ERROR，
	  SCIPuts("Verify failed.\r\n", -1);
	}

	SCIPuts("\r\n ============Test End===========.\r\n", -1);

	while(1);
}   



//===========================================================================
// No more.
//===========================================================================
