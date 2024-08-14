/*
 * 			广州创龙电子科技有限公司
 *
 * Copyright 2015 Tronlong All rights reserved
 */

/*
 * 功能描述：
 *     本例程使用查询方式检测KEY1和KEY2，当KEY1按下时LED1亮，KEY1松开是LED1灭；
 * 当KEY2按下时LED2亮，KEY2松开是LED2灭；当KEY3按下时LED3亮，KEY3松开是LED3灭。
 *
 */

#include "DSP2833x_Device.h"     // DSP2823x Headerfile Include File
#include "DSP2833x_examples.h"   // DSP2823x Examples Include File

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
	// Configure GPIO60 and GPIO61 as a GPIO output pin
   EALLOW;
   // LED
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

// Configure GPIO58 and GPIO59 as a GPIO input pin  
   EALLOW;
   // KEY
   GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 0;         // GPIO
   GpioCtrlRegs.GPADIR.bit.GPIO24 = 0;          // input
   GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 0;         // GPIO
   GpioCtrlRegs.GPADIR.bit.GPIO25 = 0;          // input
   GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 0;         // GPIO
   GpioCtrlRegs.GPADIR.bit.GPIO26 = 0;          // input

   EDIS;
////////////////////////////////////////////////
  while (1)
  {
  	if (GpioDataRegs.GPADAT.bit.GPIO24==0)
   		GpioDataRegs.GPBDAT.bit.GPIO50=1;  //LED1
   	else 
   		GpioDataRegs.GPBDAT.bit.GPIO50=0;

  	if (GpioDataRegs.GPADAT.bit.GPIO25==0)
   		GpioDataRegs.GPBDAT.bit.GPIO51=1;  //LED2
   	else 
   		GpioDataRegs.GPBDAT.bit.GPIO51=0;

  	if (GpioDataRegs.GPADAT.bit.GPIO26==0)
   		GpioDataRegs.GPBDAT.bit.GPIO52=1;  //LED3
   	else 
   		GpioDataRegs.GPBDAT.bit.GPIO52=0;

	}
} 	

	
//===========================================================================
// No more.
//===========================================================================

