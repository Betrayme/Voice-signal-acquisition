/*
 * 			广州创龙电子科技有限公司
 *
 * Copyright 2015 Tronlong All rights reserved
 */

/*
 * 功能描述：
 *     ecap配置为apwm输出。
 *     ecap1配置为GPIO24输出，
 *     ecap2配置为GPIO7输出，
 *     ecap3配置为GPIO9输出，
 *     ecap4配置为GPIO11输出，
 *     若输入时钟为30M，XCLKOUT输出频率为150M，则
 *     ecap1输出频率在7.5Hz与15Hz之间变化，
 *     ecap2输出频率为7.5Hz，
 *     ecap3输出频率为1.5Hz，
 *     ecap4输出频率为30Hz，
 *     若输入时钟为20M，XCLKOUT输出频率为100M，则
 *     ecap1输出频率在5Hz与10Hz之间变化，
 *     ecap2输出频率为5Hz，
 *     ecap3输出频率为1Hz，
 *     ecap4输出频率为20Hz.
 *
 */

#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

// Global variables
Uint16 direction = 0;

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

// Initialize the GPIO pins for eCAP.
// This function is found in the DSP2833x_ECap.c file
   InitECapGpio();

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
   SCIPuts("Welcome to TL28335 ECAP_EPWM Demo application.\r\n\r\n", -1);

// Interrupts that are used in this example are re-mapped to
// ISR functions found within this file.
// No interrupts used for this example.

// Step 4. Initialize all the Device Peripherals:
// This function is found in DSP2833x_InitPeripherals.c
// InitPeripherals(); // Not required for this example

// Step 5. User specific code
   // Setup APWM mode on CAP1, set period and compare registers
   ECap1Regs.ECCTL2.bit.CAP_APWM = 1;	// Enable APWM mode
   ECap1Regs.CAP1 = 0x01312D00;			// Set Period value
   ECap1Regs.CAP2 = 0x00989680;			// Set Compare value
   ECap1Regs.ECCLR.all = 0x0FF;			// Clear pending interrupts
   ECap1Regs.ECEINT.bit.CTR_EQ_CMP = 1; // enable Compare Equal Int

   // Setup APWM mode on CAP2, set period and compare registers
   ECap2Regs.ECCTL2.bit.CAP_APWM = 1;	// Enable APWM mode
   ECap2Regs.CAP1 = 0x01312D00;			// Set Period value
   ECap2Regs.CAP2 = 0x00989680;			// Set Compare value
   ECap2Regs.ECCLR.all = 0x0FF;			// Clear pending interrupts
   ECap1Regs.ECEINT.bit.CTR_EQ_CMP = 1; // enable Compare Equal Int

   // Setup APWM mode on CAP3, set period and compare registers
   ECap3Regs.ECCTL2.bit.CAP_APWM = 1;	// Enable APWM mode
   ECap3Regs.CAP1 = 0x05F5E100;			// Set Period value
   ECap3Regs.CAP2 = 0x02FAF080;			// Set Compare value
   ECap3Regs.ECCLR.all = 0x0FF;			// Clear pending interrupts
   ECap1Regs.ECEINT.bit.CTR_EQ_CMP = 1; // enable Compare Equal Int

   // Setup APWM mode on CAP4, set period and compare registers
   ECap4Regs.ECCTL2.bit.CAP_APWM = 1;	// Enable APWM mode
   ECap4Regs.CAP1 = 0x00001388;			// Set Period value
   ECap4Regs.CAP2 = 0x000009C4;			// Set Compare value
   ECap4Regs.ECCLR.all = 0x0FF;			// Clear pending interrupts
   ECap1Regs.ECEINT.bit.CTR_EQ_CMP = 1; // enable Compare Equal Int

   // Start counters
   ECap1Regs.ECCTL2.bit.TSCTRSTOP = 1;
   ECap2Regs.ECCTL2.bit.TSCTRSTOP = 1;
   ECap3Regs.ECCTL2.bit.TSCTRSTOP = 1;
   ECap4Regs.ECCTL2.bit.TSCTRSTOP = 1;

   for(;;)
   {
      // set next duty cycle to 50%
      ECap1Regs.CAP4 = ECap1Regs.CAP1 >> 1;

      // vary freq between 7.5 Hz and 15 Hz (for 150MHz SYSCLKOUT) 5 Hz and 10 Hz (for 100 MHz SYSCLKOUT)
      if(ECap1Regs.CAP1 >= 0x01312D00)
      {
         direction = 0;
      } else if (ECap1Regs.CAP1 <= 0x00989680)
      {
         direction = 1;
      }

      if(direction == 0)
      {
         ECap1Regs.CAP3 = ECap1Regs.CAP1 - 500000;
      } else
      {
         ECap1Regs.CAP3 = ECap1Regs.CAP1 + 500000;
      }
   }
}


//===========================================================================
// No more.
//===========================================================================
