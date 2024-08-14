/*
 * 			广州创龙电子科技有限公司
 *
 * Copyright 2015 Tronlong All rights reserved
 */

/*
 * 功能描述：
 *	   本例程演示空闲及唤醒的低功耗模式。将CPU设置为IDLE模式，配置GPIO0为低功耗
 *模式唤醒管脚，当GPIO0产生下降沿，触发XINT1中断将CPU从IDLE模式唤醒。为了便于观
 *察，CPU唤醒后将第一盏LED灯点亮。
 *
 */

#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

// Prototype statements for functions found within this file.
__interrupt void XINT_1_ISR(void);  	// ISR

void main()
{
// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the DSP2833x_SysCtrl.c file.
   InitSysCtrl();

// Step 2. Initialize GPIO:
// This example function is found in the DSP2833x_Gpio.c file and
// illustrates how to set the GPIO to it's default state.
// InitGpio();  // Skipped for this example

    EALLOW;
	GpioCtrlRegs.GPAPUD.all = 0;                    // Enable all Pull-ups
	GpioCtrlRegs.GPBPUD.all = 0;
	GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 0;		// Choose GPIO0 as the XINT1 pin.
	GpioCtrlRegs.GPADIR.all = 0xFFFFFFFE;	        // All pins are outputs except 0
	GpioCtrlRegs.GPBDIR.bit.GPIO50=1;
	GpioDataRegs.GPBCLEAR.bit.GPIO50=1;
	GpioDataRegs.GPADAT.all = 0x00000000;	        // All I/O pins are driven low
    EDIS;

    XIntruptRegs.XINT1CR.bit.ENABLE = 1; 	        // Enable XINT1 pin
	XIntruptRegs.XINT1CR.bit.POLARITY = 0;	        // Interrupt triggers on falling edge

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
   SCIPuts("Welcome to TL28335 LPM_IDLEWAKE Demo application.\r\n\r\n", -1);

// Interrupts that are used in this example are re-mapped to
// ISR functions found within this file.
   EALLOW;  // This is needed to write to EALLOW protected registers
   PieVectTable.XINT1 = &XINT_1_ISR;
   EDIS;

// Step 4. Initialize all the Device Peripherals:
// Not applicable for this example.

// Step 5. User specific code, enable interrupts:

// Enable CPU INT1 which is connected to WakeInt:
   IER |= M_INT1;

// Enable XINT1 in the PIE: Group 1 interrupt 4
   PieCtrlRegs.PIEIER1.bit.INTx4 = 1;
   PieCtrlRegs.PIEACK.bit.ACK1 = 1;

// Enable global Interrupts:
   EINT;   // Enable Global interrupt INTM

// Write the LPM code value
  EALLOW;
  if (SysCtrlRegs.PLLSTS.bit.MCLKSTS != 1) // Only enter Idle mode when PLL is not in limp mode.
  {
     SysCtrlRegs.LPMCR0.bit.LPM = 0x0000;  // LPM mode = Idle
  }
  EDIS;

  __asm(" IDLE");                            // Device waits in IDLE until XINT1 interrupts
  for(;;){}
}

__interrupt void XINT_1_ISR(void)
{
   GpioDataRegs.GPBSET.bit.GPIO50 = 1;	// GPIO50 is driven high upon exiting IDLE.
   PieCtrlRegs.PIEACK.bit.ACK1 = 1;
   EINT;
   SCIPuts("\r\n ============Test End===========.\r\n", -1);
   return;
}



