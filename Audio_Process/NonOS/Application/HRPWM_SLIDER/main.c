/*
 * 			广州创龙电子科技有限公司
 *
 * Copyright 2015 Tronlong All rights reserved
 */

/*
 * 功能描述：
 *			本例程通过HRPWM滚动条来调整MEP，利用滚动条修改变量DutyFine，调整
 *			MEP控制寄存器的值来展示由于各自EPWM模块的HRPWM控制扩展功能而产生
 *			的边沿替代。
 *			HRPWM控制EPWM模块的EPWM1A,EPWM2A,EPWM3A,EPWM4A通道，HRPWM逻辑
 *			使其边沿更好。
 *			EPWM1A对应GPIO0		EPWM1B对应GPIO1
 *			EPWM2A对应GPIO2		EPWM2B对应GPIO3
 *			EPWM3A对应GPIO4		EPWM3B对应GPIO5
 *			EPWM4A对应GPIO6		EPWM4B对应GPIO7
 *
 *			在运行程序之前应加载Example_2833xHRPWM_slider.gel文件，可从Scripts
 *			菜单下调出滚动条，通过变量DutyFine可观察到滚动条的调整情况。
 */

#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "DSP2833x_EPwm_defines.h" 	// useful defines for initialization

// Declare your function prototypes here
void HRPWM1_Config(int);
void HRPWM2_Config(int);
void HRPWM3_Config(int);
void HRPWM4_Config(int);

// General System nets - Useful for debug
Uint16 i,j,	duty, DutyFine, n,update;

Uint32 temp;

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
// For this case, just init GPIO for ePWM1-ePWM4

// For this case just init GPIO pins for ePWM1, ePWM2, ePWM3, ePWM4
// These functions are in the DSP2833x_EPwm.c file
   InitEPwm1Gpio();
   InitEPwm2Gpio();
   InitEPwm3Gpio();
   InitEPwm4Gpio();

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
   SCIPuts("Welcome to TL28335 HRPWM_SLIDER Demo application.\r\n\r\n", -1);

// Step 4. Initialize all the Device Peripherals:
// This function is found in DSP2833x_InitPeripherals.c
// InitPeripherals();  // Not required for this example

// For this example, only initialize the ePWM
// Step 5. User specific code, enable interrupts:

   update =1;
   DutyFine =0;

   EALLOW;
   SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
   EDIS;

//  Some useful Period vs Frequency values
//  SYSCLKOUT =     150MHz         100 MHz
//  -----------------------------------------
//	Period	        Frequency      Frequency
//	1000			150 kHz		   100 KHz
//	800				187 kHz		   125 KHz
//	600				250 kHz		   167 KHz
//	500				300 kHz		   200 KHz
//	250				600 kHz		   400 KHz
//	200				750 kHz		   500 KHz
//	100				1.5 MHz		   1.0 MHz
//	50				3.0 MHz		   2.0 MHz
//	25				6.0 MHz		   4.0 MHz
//	20				7.5 MHz		   5.0 MHz
//	12				12.5 MHz	   8.33 MHz
//	10				15.0 MHz	   10.0 MHz
//	9				16.7 MHz	   11.1 MHz
//	8				18.8 MHz	   12.5 MHz
//	7				21.4 MHz	   14.3 MHz
//	6				25.0 MHz	   16.7 MHz
//	5				30.0 MHz	   20.0 MHz

//====================================================================
// ePWM and HRPWM register initialization
//====================================================================
   HRPWM1_Config(10);	    // ePWM1 target, 15 MHz PWM (SYSCLK=150MHz) or  10 MHz PWM (SYSCLK=100MHz)
   HRPWM2_Config(20);	    // ePWM2 target, 7.5 MHz PWM (SYSCLK=150MHz) or 5 MHz PWM (SYSCLK=100MHz)
   HRPWM3_Config(10);	    // ePWM3 target, 15 MHz PWM (SYSCLK=150MHz) or 10 MHz PWM (SYSCLK=100MHz)
   HRPWM4_Config(20);	    // ePWM4 target, 7.5 MHz PWM (SYSCLK=150MHz) or 5 MHz PWM (SYSCLK=100MHz)

   EALLOW;
   SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
   EDIS;

   while (update ==1)
	{
//        for(DutyFine =1; DutyFine <256 ;DutyFine ++)
        {

        // Example, write to the HRPWM extension of CMPA
        EPwm1Regs.CMPA.half.CMPAHR = DutyFine << 8;     // Left shift by 8 to write into MSB bits
        EPwm2Regs.CMPA.half.CMPAHR = DutyFine << 8;     // Left shift by 8 to write into MSB bits

        // Example, 32-bit write to CMPA:CMPAHR
        EPwm3Regs.CMPA.all = ((Uint32)EPwm3Regs.CMPA.half.CMPA << 16) + (DutyFine << 8);
        EPwm4Regs.CMPA.all = ((Uint32)EPwm4Regs.CMPA.half.CMPA << 16) + (DutyFine << 8);

//		for (i=0;i<10000;i++){}                         // Dummy delay between MEP changes
		}
	}
}


void HRPWM1_Config(period)
{
// ePWM1 register configuration with HRPWM
// ePWM1A toggle low/high with MEP control on Rising edge

	EPwm1Regs.TBCTL.bit.PRDLD = TB_IMMEDIATE;	        // set Immediate load
	EPwm1Regs.TBPRD = period - 1;		                // PWM frequency = 1 / period
	EPwm1Regs.CMPA.half.CMPA = period / 2;              // set duty 50% initially
    EPwm1Regs.CMPA.half.CMPAHR = (1 << 8);              // initialize HRPWM extension
	EPwm1Regs.CMPB = period / 2;	                    // set duty 50% initially
	EPwm1Regs.TBPHS.all = 0;
	EPwm1Regs.TBCTR = 0;

	EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;
	EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;		       // EPWM1 is the Master
	EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE;
	EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
	EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;

	EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
	EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
	EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;

	EPwm1Regs.AQCTLA.bit.ZRO = AQ_CLEAR;               // PWM toggle low/high
	EPwm1Regs.AQCTLA.bit.CAU = AQ_SET;
	EPwm1Regs.AQCTLB.bit.ZRO = AQ_CLEAR;
	EPwm1Regs.AQCTLB.bit.CBU = AQ_SET;

	EALLOW;
	EPwm1Regs.HRCNFG.all = 0x0;
	EPwm1Regs.HRCNFG.bit.EDGMODE = HR_REP;				//MEP control on Rising edge
	EPwm1Regs.HRCNFG.bit.CTLMODE = HR_CMP;
	EPwm1Regs.HRCNFG.bit.HRLOAD  = HR_CTR_ZERO;
	EDIS;
}

void HRPWM2_Config(period)
{
// ePWM2 register configuration with HRPWM
// ePWM2A toggle low/high with MEP control on Rising edge

	EPwm2Regs.TBCTL.bit.PRDLD = TB_IMMEDIATE;	        // set Immediate load
	EPwm2Regs.TBPRD = period - 1;		                // PWM frequency = 1 / period
	EPwm2Regs.CMPA.half.CMPA = period / 2;              // set duty 50% initially
    EPwm1Regs.CMPA.half.CMPAHR = (1 << 8);              // initialize HRPWM extension
	EPwm2Regs.CMPB = period / 2;	                    // set duty 50% initially
	EPwm2Regs.TBPHS.all = 0;
	EPwm2Regs.TBCTR = 0;

	EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;
	EPwm2Regs.TBCTL.bit.PHSEN = TB_DISABLE;		         // ePWM2 is the Master
	EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE;
	EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
	EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV1;

	EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
	EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
	EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;

	EPwm2Regs.AQCTLA.bit.ZRO = AQ_CLEAR;                  // PWM toggle low/high
	EPwm2Regs.AQCTLA.bit.CAU = AQ_SET;
	EPwm2Regs.AQCTLB.bit.ZRO = AQ_CLEAR;
	EPwm2Regs.AQCTLB.bit.CBU = AQ_SET;

	EALLOW;
	EPwm2Regs.HRCNFG.all = 0x0;
	EPwm2Regs.HRCNFG.bit.EDGMODE = HR_REP;                //MEP control on Rising edge
	EPwm2Regs.HRCNFG.bit.CTLMODE = HR_CMP;
	EPwm2Regs.HRCNFG.bit.HRLOAD  = HR_CTR_ZERO;

	EDIS;
}

void HRPWM3_Config(period)
{
// ePWM3 register configuration with HRPWM
// ePWM3A toggle high/low with MEP control on falling edge

	EPwm3Regs.TBCTL.bit.PRDLD = TB_IMMEDIATE;	        // set Immediate load
	EPwm3Regs.TBPRD = period - 1;		                // PWM frequency = 1 / period
	EPwm3Regs.CMPA.half.CMPA = period / 2;              // set duty 50% initially
	EPwm3Regs.CMPA.half.CMPAHR = (1 << 8);              // initialize HRPWM extension
	EPwm3Regs.CMPB = period / 2;	                    // set duty 50% initially
	EPwm3Regs.TBPHS.all = 0;
	EPwm3Regs.TBCTR = 0;

	EPwm3Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;
	EPwm3Regs.TBCTL.bit.PHSEN = TB_DISABLE;		        // ePWM3 is the Master
	EPwm3Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE;
	EPwm3Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
	EPwm3Regs.TBCTL.bit.CLKDIV = TB_DIV1;

	EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
	EPwm3Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
	EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm3Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;

	EPwm3Regs.AQCTLA.bit.ZRO = AQ_SET;                  // PWM toggle high/low
	EPwm3Regs.AQCTLA.bit.CAU = AQ_CLEAR;
	EPwm3Regs.AQCTLB.bit.ZRO = AQ_SET;
	EPwm3Regs.AQCTLB.bit.CBU = AQ_CLEAR;

	EALLOW;
	EPwm3Regs.HRCNFG.all = 0x0;
	EPwm3Regs.HRCNFG.bit.EDGMODE = HR_FEP;               //MEP control on falling edge
	EPwm3Regs.HRCNFG.bit.CTLMODE = HR_CMP;
	EPwm3Regs.HRCNFG.bit.HRLOAD  = HR_CTR_ZERO;
	EDIS;
}

void HRPWM4_Config(period)
{
// ePWM4 register configuration with HRPWM
// ePWM4A toggle high/low with MEP control on falling edge

	EPwm4Regs.TBCTL.bit.PRDLD = TB_IMMEDIATE;	        // set Immediate load
	EPwm4Regs.TBPRD = period - 1;		                // PWM frequency = 1 / period
	EPwm4Regs.CMPA.half.CMPA = period / 2;              // set duty 50% initially
	EPwm4Regs.CMPA.half.CMPAHR = (1 << 8);              // initialize HRPWM extension
	EPwm4Regs.CMPB = period / 2;	                    // set duty 50% initially
	EPwm4Regs.TBPHS.all = 0;
	EPwm4Regs.TBCTR = 0;

	EPwm4Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;
	EPwm4Regs.TBCTL.bit.PHSEN = TB_DISABLE;		        // ePWM4 is the Master
	EPwm4Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE;
	EPwm4Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
	EPwm4Regs.TBCTL.bit.CLKDIV = TB_DIV1;

	EPwm4Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
	EPwm4Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
	EPwm4Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm4Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;

	EPwm4Regs.AQCTLA.bit.ZRO = AQ_SET;                  // PWM toggle high/low
	EPwm4Regs.AQCTLA.bit.CAU = AQ_CLEAR;
	EPwm4Regs.AQCTLB.bit.ZRO = AQ_SET;
	EPwm4Regs.AQCTLB.bit.CBU = AQ_CLEAR;

	EALLOW;
	EPwm4Regs.HRCNFG.all = 0x0;
	EPwm4Regs.HRCNFG.bit.EDGMODE = HR_FEP;              //MEP control on falling edge
	EPwm4Regs.HRCNFG.bit.CTLMODE = HR_CMP;
	EPwm4Regs.HRCNFG.bit.HRLOAD  = HR_CTR_ZERO;
	EDIS;
}

//===========================================================================
// No more.
//===========================================================================
