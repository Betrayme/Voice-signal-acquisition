/*
 * 			广州创龙电子科技有限公司
 *
 * Copyright 2015 Tronlong All rights reserved
 */

/*
 * 功能描述：
 *     本例程首先向sram写入0x5555，读回并校验数据；再写入0xaaaa，读回并校验数据；
 * 写入递增数据，读回并校验数据。测试结果通过串口B打印。
 *
 */

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

// XZCS7    0x200000~0x24FFFF共327679K
Uint16    *ExRamStart = (Uint16 *)0x200000;

void main(void)
{
   Uint32	i;
   unsigned int ErrorCount;
// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the DSP2833x_SysCtrl.c file.
   InitSysCtrl();

// Step 2. Initalize GPIO:
// This example function is found in the DSP2833x_Gpio.c file and
// illustrates how to set the GPIO to it's default state.
// InitGpio();  // Skipped for this example
   InitXintf();
//   InitXintf16Gpio();	//zq

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

   SCIStdioInit();
   SCIPuts("\r\n ============Test Start===========.\r\n", -1);
   SCIPuts("Welcome to TL28335 SRAM Demo application.\r\n\r\n", -1);

// Step 4. Write Data to SRAM:
   SCIPuts("Write data 0x5555 to SRAM.\r\n", -1);
   for(i = 0; i < 0x4FFFF; i++)
   {
		*(ExRamStart + i) = 0x5555;
   }
   SCIPuts("Verify the data.\r\n", -1);

   ErrorCount = 0;
   for(i = 0; i < 0x4FFFF; i++)
   		if(*(ExRamStart + i) != 0x5555)
   			ErrorCount++;

   if(ErrorCount == 0)
	   SCIPuts("Test succeed.\r\n", -1);
   else
	   SCIprintf("Test failed with %u errors.\r\n", ErrorCount);
   SCIPuts("\r\n", -1);

//////////////////////////////////////////////////////////////////
   SCIPuts("Write data 0xAAAA to SRAM.\r\n", -1);
   for(i = 0; i < 0x4FFFF; i++)
   {
		*(ExRamStart + i) = 0xAAAA;
   }

   SCIPuts("Verify the data.\r\n", -1);

   ErrorCount = 0;
   for(i = 0; i < 0x4FFFF; i++)
		if(*(ExRamStart + i) != 0xAAAA)
			ErrorCount++;

   if(ErrorCount == 0)
	   SCIPuts("Test succeed.\r\n", -1);
   else
	   SCIprintf("Test failed with %u errors.\r\n", ErrorCount);
   SCIPuts("\r\n", -1);

//////////////////////////////////////////////////////////////////
	SCIPuts("Write increasing data to SRAM.\r\n", -1);
	for(i = 0; i< 0xFFFF; i++)
	{
		*(ExRamStart + i) = i;
	}

	SCIPuts("Verify the data.\r\n", -1);

	ErrorCount = 0;
	for(i = 0; i < 0x4FFFF; i++)
		if(*(ExRamStart + i) != i)
			ErrorCount++;

	if(ErrorCount == 0)
	   SCIPuts("Test succeed.\r\n", -1);
	else
	   SCIprintf("Test failed with %u errors.\r\n", ErrorCount);

	SCIPuts("\r\n ============Test End===========.\r\n", -1);

	while(1);
}

//===========================================================================
// No more.
//===========================================================================
