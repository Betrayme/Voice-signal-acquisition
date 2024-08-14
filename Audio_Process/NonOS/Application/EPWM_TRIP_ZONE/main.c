/*
 * 			广州创龙电子科技有限公司
 *
 * Copyright 2015 Tronlong All rights reserved
 */

/*
 * 功能描述：
 *     故障区演示。
 *     GPIO0，GPIO1分别为EPWM1A，EPWM1B输出，
 *     GPIO2，GPIO3分别为EPWM2A，EPWM2B输出，
 *     GPIO12配置为TZ1,单次触发，
 *     GPIO13配置为TZ2，周期触发，
 *     初始化TZ1,TZ2为高电平，程序运行时将TZ1或TZ2拉低，相应EPWM做出相应动作
 *     当TZ拉低时，EPWMxA输出拉高，EPWMxB输出拉低
 *
 */

#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

// Prototype statements for functions found within this file.
void InitEPwm1Example(void);
void InitEPwm2Example(void);
__interrupt void epwm1_tzint_isr(void);
__interrupt void epwm2_tzint_isr(void);

// Global variables used in this example
Uint32  EPwm1TZIntCount;
Uint32  EPwm2TZIntCount;

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

// For this case just init GPIO pins for ePWM1, ePWM2, and TZ pins
   InitEPwm1Gpio();
   InitEPwm2Gpio();
   InitTzGpio();

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
   SCIPuts("Welcome to TL28335 EPWM_TRIP_ZONE Demo application.\r\n\r\n", -1);

// Interrupts that are used in this example are re-mapped to
// ISR functions found within this file.
   EALLOW;  // This is needed to write to EALLOW protected registers
   PieVectTable.EPWM1_TZINT = &epwm1_tzint_isr;
   PieVectTable.EPWM2_TZINT = &epwm2_tzint_isr;
   EDIS;    // This is needed to disable write to EALLOW protected registers

// Step 4. Initialize all the Device Peripherals:
// This function is found in DSP2833x_InitPeripherals.c
// InitPeripherals();  // Not required for this example
   EALLOW;
   SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
   EDIS;

   InitEPwm1Example();
   InitEPwm2Example();

   EALLOW;
   SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
   EDIS;

// Step 5. User specific code, enable interrupts
// Initialize counters:
   EPwm1TZIntCount = 0;
   EPwm2TZIntCount = 0;

// Enable CPU INT3 which is connected to EPWM1-3 INT:
   IER |= M_INT2;

// Enable EPWM INTn in the PIE: Group 2 interrupt 1-3
   PieCtrlRegs.PIEIER2.bit.INTx1 = 1;
   PieCtrlRegs.PIEIER2.bit.INTx2 = 1;

// Enable global Interrupts and higher priority real-time debug events:
   EINT;   // Enable Global interrupt INTM
   ERTM;   // Enable Global realtime interrupt DBGM

// Step 6. IDLE loop. Just sit and loop forever (optional):
   for(;;)
   {
       __asm("          NOP");
   }
}

__interrupt void epwm1_tzint_isr(void)
{
   EPwm1TZIntCount++;

// Leave these flags set so we only take this
// interrupt once
//
// EALLOW;
// EPwm1Regs.TZCLR.bit.OST = 1;
// EPwm1Regs.TZCLR.bit.INT = 1;
// EDIS;

   // Acknowledge this interrupt to receive more interrupts from group 2
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;
}

__interrupt void epwm2_tzint_isr(void)
{
   EPwm2TZIntCount++;

// Clear the flags - we will continue to take
// this interrupt until the TZ pin goes high
//
   EALLOW;
   EPwm2Regs.TZCLR.bit.CBC = 1;
   EPwm2Regs.TZCLR.bit.INT = 1;
   EDIS;

   // Acknowledge this interrupt to receive more interrupts from group 2
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;
}

void InitEPwm1Example()
{
   // Enable TZ1 and TZ2 as one shot trip sources
   EALLOW;
   EPwm1Regs.TZSEL.bit.OSHT1 = 1;
   EPwm1Regs.TZSEL.bit.OSHT2 = 1;

   // What do we want the TZ1 and TZ2 to do?
   EPwm1Regs.TZCTL.bit.TZA = TZ_FORCE_HI;
   EPwm1Regs.TZCTL.bit.TZB = TZ_FORCE_LO;

   // Enable TZ interrupt
   EPwm1Regs.TZEINT.bit.OST = 1;
   EDIS;

   EPwm1Regs.TBPRD = 6000;                         // Set timer period
   EPwm1Regs.TBPHS.half.TBPHS = 0x0000;            // Phase is 0
   EPwm1Regs.TBCTR = 0x0000;                       // Clear counter

   // Setup TBCLK
   EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up
   EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
   EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV4;       // Clock ratio to SYSCLKOUT
   EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV4;

   EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;    // Load registers every ZERO
   EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
   EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
   EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

   // Setup compare
   EPwm1Regs.CMPA.half.CMPA = 3000;


   // Set actions
   EPwm1Regs.AQCTLA.bit.CAU = AQ_SET;             // Set PWM1A on Zero
   EPwm1Regs.AQCTLA.bit.CAD = AQ_CLEAR;


   EPwm1Regs.AQCTLB.bit.CAU = AQ_CLEAR;          // Set PWM1A on Zero
   EPwm1Regs.AQCTLB.bit.CAD = AQ_SET;
}

void InitEPwm2Example()
{
   // Enable TZ1 and TZ2 as one cycle-by-cycle trip sources
   EALLOW;
   EPwm2Regs.TZSEL.bit.CBC1 = 1;
   EPwm2Regs.TZSEL.bit.CBC2 = 1;

   // What do we want the TZ1 and TZ2 to do?
   EPwm2Regs.TZCTL.bit.TZA = TZ_FORCE_HI;
   EPwm2Regs.TZCTL.bit.TZB = TZ_FORCE_LO;

   // Enable TZ interrupt
   EPwm2Regs.TZEINT.bit.CBC = 1;
   EDIS;

   EPwm2Regs.TBPRD = 6000;                        // Set timer period
   EPwm2Regs.TBPHS.half.TBPHS = 0x0000;           // Phase is 0
   EPwm2Regs.TBCTR = 0x0000;                      // Clear counter

   // Setup TBCLK
   EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up
   EPwm2Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
   EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV4;       // Clock ratio to SYSCLKOUT
   EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV4;          // Slow just to observe on the scope

   // Setup compare
   EPwm2Regs.CMPA.half.CMPA = 3000;

   // Set actions
   EPwm2Regs.AQCTLA.bit.CAU = AQ_SET;             // Set PWM2A on Zero
   EPwm2Regs.AQCTLA.bit.CAD = AQ_CLEAR;

   EPwm2Regs.AQCTLB.bit.CAU = AQ_CLEAR;           // Set PWM2A on Zero
   EPwm2Regs.AQCTLB.bit.CAD = AQ_SET;
}


//===========================================================================
// No more.
//===========================================================================
