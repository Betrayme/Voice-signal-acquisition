/*
 * 			广州创龙电子科技有限公司
 *
 * Copyright 2015 Tronlong All rights reserved
 */

/*
 * 功能描述：
 *     本例程使用增强型正交编码模块EQEP测量EPWM频率。
 *     被测信号EPWM1A输出，配置频率为5kHz，查看变量freq.freq_pr，freq.freq_fr
 *     可验证测量结果。
 *
 *     外部连接：GPIO0----GPIO50
 *
 */

#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "Example_freqcal.h"    // Example specific include file

void EPwmSetup(void);
__interrupt void prdTick(void);

FREQCAL freq=FREQCAL_DEFAULTS;

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

// Only init the GPIO for EQep1 and EPwm1 in this case
// This function is found in DSP2833x_EQep.c
   InitEQep1Gpio();
   InitEPwm1Gpio();

// Step 3. Clear all interrupts and initialize PIE vector table:
// Disable CPU interrupts
   DINT;

// Initialize the PIE control registers to their default state.
// The default state is all PIE interrupts disabled and flags
// are cleared.
// This function is found in the DSP2833x_PieCtrl.c file.
   InitPieCtrl();

   SCIStdioInit();
   SCIPuts("\r\n ============Test Start===========.\r\n", -1);
   SCIPuts("Welcome to TL28335 EQEP_FREQCAL Demo application.\r\n\r\n", -1);

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

// Interrupts that are used in this example are re-mapped to
// ISR functions found within this file.
   EALLOW;  // This is needed to write to EALLOW protected registers
   PieVectTable.EPWM1_INT= &prdTick;
   EDIS;    // This is needed to disable write to EALLOW protected registers

// Step 4. Initialize all the Device Peripherals:
// Example specific ePWM setup.  This function is found
// in Example_EPwmSetup.c
   EPwmSetup();

// Step 5. User specific code, enable interrupts:
// Enable CPU INT1 which is connected to CPU-Timer 0:
   IER |= M_INT3;

// Enable TINT0 in the PIE: Group 3 interrupt 1
   PieCtrlRegs.PIEIER3.bit.INTx1 = 1;

// Enable global Interrupts and higher priority real-time debug events:
   EINT;   // Enable Global interrupt INTM
   ERTM;   // Enable Global realtime interrupt DBGM

   freq.init(&freq);         // Initializes eQEP for frequency calculation in
                             // FREQCAL_Init(void)function in Example_EPwmSetup.c

   SCIPuts("\r\n ============Test End===========.\r\n", -1);
	for(;;)
	{
	}
}

__interrupt void prdTick(void) // Interrupts once per ePWM period
{
   freq.calc(&freq); // Checks for event and calculates frequency in FREQCAL_Calc(FREQCAL *p)
                     // function in Example_EPwmSetup.c
   // Acknowledge this interrupt to receive more interrupts from group 1
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
   EPwm1Regs.ETCLR.bit.INT=1;
}


//===========================================================================
// No more.
//===========================================================================

