/*
 * 			广州创龙电子科技有限公司
 *
 * Copyright 2015 Tronlong All rights reserved
 */

/*
 * 功能描述：
 *     使用RS485转串口模块连接开发板RS485和PC机,上位机设置串口波特率
 * 为9600,DSP通过串口C接收到字符后，会通过串口C打印出来。
 *
 */

#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

// Prototype statements for functions found within this file.
void scic_echoback_init(void);
void scic_fifo_init(void);
void scic_xmit(int a);
void scic_msg(char *msg);

// Global counts used in this example
Uint16 LoopCount;
Uint16 ErrorCount;

void main(void)
{

    Uint16 ReceivedChar;
    char *msg;

// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the DSP2833x_SysCtrl.c file.
   InitSysCtrl();

// Step 2. Initalize GPIO:
// This example function is found in the DSP2833x_Gpio.c file and
// illustrates how to set the GPIO to it's default state.
   // InitGpio(); Skipped for this example

// For this example, only init the pins for the SCI-A port.
// This function is found in the DSP2833x_Sci.c file.
   InitScicGpio();

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

   EALLOW;
// Step 5. User specific code:
   GpioCtrlRegs.GPBMUX2.bit.GPIO49 = 0;
   GpioCtrlRegs.GPBDIR.bit.GPIO49 = 1;

   //rs232使能
   GpioDataRegs.GPBSET.bit.GPIO49 = 1;
   EDIS;

    LoopCount = 0;
    ErrorCount = 0;

    scic_fifo_init();	   // Initialize the SCI FIFO
    scic_echoback_init();  // Initalize SCI for echoback

    msg = "\r\nWelcome to TL28335 SCIC RS485 Demo application.\n\0";
    scic_msg(msg);

    msg = "\r\nYou will enter a character, and the DSP will echo it back! \n\0";
    scic_msg(msg);
    ScicRegs.SCIFFRX.bit.RXFFOVRCLR=1;

	for(;;)
    {
       msg = "\r\nEnter a character: \0";
       scic_msg(msg);

       // Wait for inc character
       while(ScicRegs.SCIFFRX.bit.RXFFST !=1) { } // wait for XRDY =1 for empty state

       // Get character
       ReceivedChar = ScicRegs.SCIRXBUF.all;

       // Echo character back
       msg = "  You sent: \0";
       scic_msg(msg);
       scic_xmit(ReceivedChar);

       LoopCount++;
    }

}


// Test 1,SCIA  DLB, 8-bit word, baud rate 0x000F, default, 1 STOP bit, no parity
void scic_echoback_init()
{
    // Note: Clocks were turned on to the Scib peripheral
    // in the InitSysCtrl() function

 	ScicRegs.SCICCR.all =0x0007;   // 1 stop bit,  No loopback
                                   // No parity,8 char bits,
                                   // async mode, idle-line protocol
	ScicRegs.SCICTL1.all =0x0003;  // enable TX, RX, internal SCICLK,
                                   // Disable RX ERR, SLEEP, TXWAKE
	ScicRegs.SCICTL2.all =0x0003;
	ScicRegs.SCICTL2.bit.TXINTENA =1;
	ScicRegs.SCICTL2.bit.RXBKINTENA =1;
	#if (CPU_FRQ_150MHZ)
	      ScicRegs.SCIHBAUD    =0x0000;  // 9600 baud @LSPCLK = 37.5MHz.
	      ScicRegs.SCILBAUD    =0x0028;
	#endif
	#if (CPU_FRQ_100MHZ)
      ScicRegs.SCIHBAUD    =0x0001;  // 9600 baud @LSPCLK = 20MHz.
      ScicRegs.SCILBAUD    =0x0044;
	#endif
	ScicRegs.SCICTL1.all =0x0023;  // Relinquish SCI from Reset
}

// Transmit a character from the SCI
void scic_xmit(int a)
{
    while (ScicRegs.SCIFFTX.bit.TXFFST != 0) {}
    ScicRegs.SCITXBUF=a;

}

void scic_msg(char * msg)
{
    int i;
    i = 0;
    while(msg[i] != '\0')
    {
        scic_xmit(msg[i]);
        i++;
    }
}

// Initalize the SCI FIFO
void scic_fifo_init()
{
    ScicRegs.SCIFFTX.all=0xE040;
    ScicRegs.SCIFFRX.all=0x204f;
    ScicRegs.SCIFFCT.all=0x0;

}






//===========================================================================
// No more.
//===========================================================================

