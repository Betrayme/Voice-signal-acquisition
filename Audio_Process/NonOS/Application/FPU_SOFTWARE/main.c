/*
 * 			广州创龙电子科技有限公司
 *
 * Copyright 2015 Tronlong All rights reserved
 */

/*
 * 功能描述：
 *     本例程配置软件浮点运算，计算两个y=mx+b的方程。
 *     所有变量均为32位浮点型，观察变量y1,y2可验证计算结果。
 *
 */

#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include <stdio.h>

float y1, y2;
float m1, m2;
float x1, x2;
float b1, b2;
char s1[100],s2[100];

void main(void)
{
// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the DSP2833x_SysCtrl.c file.
   InitSysCtrl();

// Step 2. Initialize GPIO:
// This example function is found in the DSP2833x_Gpio.c file and
// illustrates how to set the GPIO to it's default state.
// InitGpio();  // Skipped for this example

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
   SCIPuts("Welcome to TL28335 FPU_SOFTWARE Demo application.\r\n\r\n", -1);

// Interrupts that are used in this example are re-mapped to
// ISR functions found within this file.

// Step 5. User specific code, enable interrupts:

// Calculate two y=mx+b equations.
   y1 = 0;
   y2 = 0;
   m1 = .5;
   m2 = .6;
   x1 = 3.4;
   x2 = 7.3;
   b1 = 4.2;
   b2 = 8.9;

   y1 = m1*x1 + b1;
   y2 = m2*x2 + b2;

   SCIPuts("\r\n ============Test End===========.\r\n", -1);

   ESTOP0;  // This is a software breakpoint
}


//===========================================================================
// No more.
//===========================================================================

