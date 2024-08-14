/*
 * 			广州创龙电子科技有限公司
 *
 * Copyright 2015 Tronlong All rights reserved
 */

/*
 * 功能描述：
 *     本例程会一直采集通道 A0~A7 B0~B7 的AD数据，并且通过串口B打印。
 * 电压值计算公式：输入模拟电压 = 采样值 * 3 / 4096
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

// ADC start parameters
#if (CPU_FRQ_150MHZ)     // Default - 150 MHz SYSCLKOUT
  #define ADC_MODCLK 0x3 // HSPCLK = SYSCLKOUT/2*ADC_MODCLK2 = 150/(2*3)   = 25.0 MHz
#endif
#if (CPU_FRQ_100MHZ)
  #define ADC_MODCLK 0x2 // HSPCLK = SYSCLKOUT/2*ADC_MODCLK2 = 100/(2*2)   = 25.0 MHz
#endif
#define ADC_CKPS   0x1   // ADC module clock = HSPCLK/2*ADC_CKPS   = 25.0MHz/(1*2) = 12.5MHz
#define ADC_SHCLK  0xf   // S/H width in ADC module periods                        = 16 ADC clocks
#define BUF_SIZE   16  // Sample buffer size

// Global variable for this example
Uint16 SampleTable[BUF_SIZE];

void delay_loop();

main()
{
   Uint16 i;

// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the DSP2833x_SysCtrl.c file.
   InitSysCtrl();

// Specific clock setting for this example:
   EALLOW;
   SysCtrlRegs.HISPCP.all = ADC_MODCLK;	// HSPCLK = SYSCLKOUT/ADC_MODCLK
   EDIS;

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
   SCIPuts("Welcome to TL28335 ADC Sequencer Mode Demo application.\r\n\r\n", -1);

// Step 4. Initialize all the Device Peripherals:
// This function is found in DSP2833x_InitPeripherals.c
// InitPeripherals(); // Not required for this example
   InitAdc();  // For this example, init the ADC

// Specific ADC setup for this example:
   AdcRegs.ADCTRL1.bit.ACQ_PS = ADC_SHCLK;
   AdcRegs.ADCTRL3.bit.ADCCLKPS = ADC_CKPS;
   AdcRegs.ADCTRL1.bit.SEQ_CASC = 1;        // 1  Cascaded mode
   AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0x0;
   AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 0x1;
   AdcRegs.ADCCHSELSEQ1.bit.CONV02 = 0x2;
   AdcRegs.ADCCHSELSEQ1.bit.CONV03 = 0x3;

   AdcRegs.ADCCHSELSEQ2.bit.CONV04 = 0x4;
   AdcRegs.ADCCHSELSEQ2.bit.CONV05 = 0x5;
   AdcRegs.ADCCHSELSEQ2.bit.CONV06 = 0x6;
   AdcRegs.ADCCHSELSEQ2.bit.CONV07 = 0x7;

   AdcRegs.ADCCHSELSEQ3.bit.CONV08 = 0x8;
   AdcRegs.ADCCHSELSEQ3.bit.CONV09 = 0x9;
   AdcRegs.ADCCHSELSEQ3.bit.CONV10 = 0xa;
   AdcRegs.ADCCHSELSEQ3.bit.CONV11 = 0xb;

   AdcRegs.ADCCHSELSEQ4.bit.CONV12 = 0xc;
   AdcRegs.ADCCHSELSEQ4.bit.CONV13 = 0xd;
   AdcRegs.ADCCHSELSEQ4.bit.CONV14 = 0xe;
   AdcRegs.ADCCHSELSEQ4.bit.CONV15 = 0xf;

   AdcRegs.ADCMAXCONV.bit.MAX_CONV1 = 0xf;  // convert and store in 8 results registers

   AdcRegs.ADCTRL1.bit.CONT_RUN = 1;       // Setup continuous run

// Clear SampleTable
   for (i=0; i<BUF_SIZE; i++)
   {
     SampleTable[i] = 0;
   }

   // Start SEQ1
   AdcRegs.ADCTRL2.all = 0x2000;

   // Take ADC data and log the in SampleTable array
   for(;;)
   {
        while (AdcRegs.ADCST.bit.INT_SEQ1== 0) {} // Wait for interrupt
        AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;

        SampleTable[0] =((AdcRegs.ADCRESULT0>>4) );
        SampleTable[1] =((AdcRegs.ADCRESULT1>>4) );
        SampleTable[2] =((AdcRegs.ADCRESULT2>>4) );
        SampleTable[3] =((AdcRegs.ADCRESULT3>>4) );
        SampleTable[4] =((AdcRegs.ADCRESULT4>>4) );
        SampleTable[5] =((AdcRegs.ADCRESULT5>>4) );
        SampleTable[6] =((AdcRegs.ADCRESULT6>>4) );
        SampleTable[7] =((AdcRegs.ADCRESULT7>>4) );

        SampleTable[8] =((AdcRegs.ADCRESULT8>>4) );
		SampleTable[9] =((AdcRegs.ADCRESULT9>>4) );
		SampleTable[10] =((AdcRegs.ADCRESULT10>>4) );
		SampleTable[11] =((AdcRegs.ADCRESULT11>>4) );
		SampleTable[12] =((AdcRegs.ADCRESULT12>>4) );
		SampleTable[13] =((AdcRegs.ADCRESULT13>>4) );
		SampleTable[14] =((AdcRegs.ADCRESULT14>>4) );
		SampleTable[15] =((AdcRegs.ADCRESULT15>>4) );

		for(i=0;i<8;i++)
		{
			SCIprintf("A%d = %d \n",i,SampleTable[i]);
		}
		for(i=0;i<8;i++)
		{
			SCIprintf("B%d = %d \n",i,SampleTable[i+8]);
		}
		SCIprintf("\n");
        delay_loop();
   }
}

void delay_loop()
{
    volatile long i;
    for (i = 0; i < 5000000; i++) {}
}

//===========================================================================
// No more.
//===========================================================================

