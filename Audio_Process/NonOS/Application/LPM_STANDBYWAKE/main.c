/*
 * 			广州创龙电子科技有限公司
 *
 * Copyright 2015 Tronlong All rights reserved
 */

/*
 * 功能描述：
 *	   本例程演示待机及唤醒的低功耗模式。将CPU设置为STANDBY模式，配置GPIO0为低功耗
 *模式唤醒管脚，当GPIO0产生下降沿，将CPU从STANDBY模式唤醒。为了便于观察，CPU唤醒后
 *将第一盏LED灯状态翻转。
 *
 */

#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

// Prototype statements for functions found within this file.

__interrupt void WAKE_ISR(void);  	// ISR for WAKEINT

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
	GpioCtrlRegs.GPAPUD.all = 0;            // Enable all Pull-ups
	GpioCtrlRegs.GPBPUD.all = 0;
	GpioCtrlRegs.GPBDIR.bit.GPIO50 = 1;	    // GPIO50 set in the ISR to indicate device woken up.
    GpioIntRegs.GPIOLPMSEL.bit.GPIO0 = 1;	// Choose GPIO0 pin for wakeup
	EDIS;

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
   SCIPuts("Welcome to TL28335 LPM_STANDBYWAKE Demo application.\r\n\r\n", -1);

// Interrupts that are used in this example are re-mapped to
// ISR functions found within this file.
   EALLOW;  // This is needed to write to EALLOW protected registers
   PieVectTable.WAKEINT = &WAKE_ISR;
   EDIS;

// Step 4. Initialize all the Device Peripherals:
// Not applicable for this example.

// Step 5. User specific code, enable interrupts:

// Enable CPU INT1 which is connected to WakeInt:
   IER |= M_INT1;

// Enable WAKEINT in the PIE: Group 1 interrupt 8
   PieCtrlRegs.PIEIER1.bit.INTx8 = 1;
   PieCtrlRegs.PIEACK.bit.ACK1 = 1;
// Enable global Interrupts:
   EINT;   // Enable Global interrupt INTM

// Choose qualification cycles in LPMCR0 register
	SysCtrlRegs.LPMCR0.bit.QUALSTDBY = 0;	// The wakeup signal should be (2+QUALSTDBY) OSCCLKs wide.

// Write the LPM code value
    EALLOW;
	if (SysCtrlRegs.PLLSTS.bit.MCLKSTS != 1) // Only enter Standby mode when PLL is not in limp mode.
	{
      SysCtrlRegs.LPMCR0.bit.LPM = 0x0001;   // LPM mode = Standby
    }
    EDIS;

// Force device into STANDBY
	__asm(" IDLE");                          // Device waits in IDLE until falling edge on GPIO0/XNMI pin
	                                        // wakes device from Standby mode.
	for(;;){}								// Loop here after wake-up.
}

/* ----------------------------------------------- */
/* ISR for WAKEINT - Will be executed when         */
/* low pulse triggered on GPIO0 pin                */
/* ------------------------------------------------*/
__interrupt void WAKE_ISR(void)
{
   GpioDataRegs.GPBTOGGLE.bit.GPIO50 = 1;	// Toggle GPIO50 in the ISR - monitored with oscilloscope
   PieCtrlRegs.PIEACK.bit.ACK1 = 1;
   SCIPuts("\r\n ============Test End===========.\r\n", -1);
}


