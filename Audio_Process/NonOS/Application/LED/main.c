/*
 * 			广州创龙电子科技有限公司
 *
 * Copyright 2015 Tronlong All rights reserved
 */

/*
 * 功能描述：
 *     底板的4个LED:LED1，LED2，LED3，LED4 和 核心板的2个LED:D2，D3 都做流水灯
 * 例程包含三种控制LED流水灯的方式，在编译前可以选择。第一种通过GPIO DATA寄存器
 * 控制，第二种通过 SET/CLEAR 寄存器控制，第三种通过 TOGGLE 寄存器控制。
 *
 */

#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

// Select the example to compile in.  Only one example should be set as 1
// the rest should be set as 0.
#define EXAMPLE1 1  // Use DATA registers to toggle I/O's
#define EXAMPLE2 0  // Use SET/CLEAR registers to toggle I/O's
#define EXAMPLE3 0  // Use TOGGLE registers to toggle I/O's

// Prototype statements for functions found within this file.
void delay_loop(void);
void Gpio_select(void);
void Gpio_example1(void);
void Gpio_example2(void);
void Gpio_example3(void);

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

// Step 5. User specific code:

#if EXAMPLE1

    // This example uses DATA registers to toggle I/O's
    Gpio_example1();

#endif  // - EXAMPLE1

#if EXAMPLE2

    // This example uses SET/CLEAR registers to toggle I/O's
    Gpio_example2();

#endif

#if EXAMPLE3

    // This example uses TOGGLE registers to toggle I/O's
    Gpio_example3();

#endif
}

void delay_loop()
{
    volatile long i;
    for (i = 0; i < 5000000; i++) {}
}

void Gpio_example1(void)
{
   // Example 1:
   // Toggle I/Os using DATA registers

   for(;;)
   {
	   // GPIO12 = 1; GPIO13 = 0;
	   GpioDataRegs.GPADAT.all = 0x00001000;
	   // GPIO50 = 1; GPIO51 = 0; GPIO52 = 0; GPIO53 = 0;
	   GpioDataRegs.GPBDAT.all = 0x00040000;
	   delay_loop();

	   // GPIO50 = 0; GPIO51 = 1; GPIO52 = 0; GPIO53 = 0;
	   GpioDataRegs.GPBDAT.all = 0x00080000;
	   delay_loop();

	   // GPIO12 = 0; GPIO13 = 1;
	   GpioDataRegs.GPADAT.all = 0x00002000;
	   // GPIO50 = 0; GPIO51 = 0; GPIO52 = 1; GPIO53 = 0;
	   GpioDataRegs.GPBDAT.all = 0x00100000;
	   delay_loop();

	   // GPIO50 = 0; GPIO51 = 0; GPIO52 = 0; GPIO53 = 1;
	   GpioDataRegs.GPBDAT.all = 0x00200000;
	   delay_loop();
    }
}

void Gpio_example2(void)
{
   // Example 2:
   // Toggle I/Os using SET/CLEAR registers
   for(;;)
   {
       GpioDataRegs.GPASET.bit.GPIO12 = 1;
	   GpioDataRegs.GPACLEAR.bit.GPIO13 = 1;

	   GpioDataRegs.GPBSET.bit.GPIO50 = 1;
	   GpioDataRegs.GPBCLEAR.bit.GPIO51 = 1;
	   GpioDataRegs.GPBCLEAR.bit.GPIO52 = 1;
	   GpioDataRegs.GPBCLEAR.bit.GPIO53 = 1;
	   delay_loop();

	   GpioDataRegs.GPBCLEAR.bit.GPIO50 = 1;
	   GpioDataRegs.GPBSET.bit.GPIO51 = 1;
	   GpioDataRegs.GPBCLEAR.bit.GPIO52 = 1;
	   GpioDataRegs.GPBCLEAR.bit.GPIO53 = 1;
	   delay_loop();

	   GpioDataRegs.GPASET.bit.GPIO13 = 1;
	   GpioDataRegs.GPACLEAR.bit.GPIO12 = 1;

	   GpioDataRegs.GPBCLEAR.bit.GPIO50 = 1;
	   GpioDataRegs.GPBCLEAR.bit.GPIO51 = 1;
	   GpioDataRegs.GPBSET.bit.GPIO52 = 1;
	   GpioDataRegs.GPBCLEAR.bit.GPIO53 = 1;
	   delay_loop();

	   GpioDataRegs.GPBCLEAR.bit.GPIO50 = 1;
	   GpioDataRegs.GPBCLEAR.bit.GPIO51 = 1;
	   GpioDataRegs.GPBCLEAR.bit.GPIO52 = 1;
	   GpioDataRegs.GPBSET.bit.GPIO53 = 1;
	   delay_loop();
    }
}

void Gpio_example3(void)
{
   // Example 2:
   // Toggle I/Os using TOGGLE registers

   // Set pins to a known state
	  // GPIO12 = 1; GPIO13 = 0;
      GpioDataRegs.GPASET.all    =0x00001000;
      GpioDataRegs.GPACLEAR.all  =0x00002000;

      // GPIO50 = 1; GPIO51 = 0; GPIO52 = 0; GPIO53 = 0;
      GpioDataRegs.GPBSET.all    =0x00040000;
      GpioDataRegs.GPBCLEAR.all  =0x00380000;

   // Use TOGGLE registers to flip the state of
   // the pins.
   // Any bit set to a 1 will flip state (toggle)
   // Any bit set to a 0 will not toggle.

   for(;;)
   {
	  delay_loop();
      GpioDataRegs.GPBTOGGLE.all =0x000C0000;

      delay_loop();
      GpioDataRegs.GPATOGGLE.all =0x00003000;
	  GpioDataRegs.GPBTOGGLE.all =0x00180000;

	  delay_loop();
	  GpioDataRegs.GPBTOGGLE.all =0x00300000;

	  delay_loop();
	  GpioDataRegs.GPATOGGLE.all =0x00003000;
	  GpioDataRegs.GPBTOGGLE.all =0x00240000;
    }
}

void Gpio_select(void)
{
	EALLOW;
	GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 0;  // GPIO12
	GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 0;  // GPIO13

	GpioCtrlRegs.GPADIR.bit.GPIO12 = 1;   // output
	GpioCtrlRegs.GPADIR.bit.GPIO13 = 1;   // output

	GpioCtrlRegs.GPBMUX2.bit.GPIO50 = 0;  // GPIO50
	GpioCtrlRegs.GPBMUX2.bit.GPIO51 = 0;  // GPIO51
	GpioCtrlRegs.GPBMUX2.bit.GPIO52 = 0;  // GPIO52
	GpioCtrlRegs.GPBMUX2.bit.GPIO53 = 0;  // GPIO53

    GpioCtrlRegs.GPBDIR.bit.GPIO50 = 1;   // output
    GpioCtrlRegs.GPBDIR.bit.GPIO51 = 1;   // output
    GpioCtrlRegs.GPBDIR.bit.GPIO52 = 1;   // output
    GpioCtrlRegs.GPBDIR.bit.GPIO53 = 1;   // output
    EDIS;
}
//===========================================================================
// No more.
//===========================================================================

