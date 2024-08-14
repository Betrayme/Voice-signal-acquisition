/*
 * 			广州创龙电子科技有限公司
 *
 * Copyright 2015 Tronlong All rights reserved
 */

/*
 * 功能描述：
 *		本例程通过中断方式实现SPI内部回环字符数据，发送的数据流由0依次增加。
 *		观察变量sdata、rdata可知数据回环情况。
 *
 *
 */

#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

// Prototype statements for functions found within this file.
// __interrupt void ISRTimer2(void);
__interrupt void spiTxFifoIsr(void);
__interrupt void spiRxFifoIsr(void);
void delay_loop(void);
void spi_fifo_init(void);
void error();

Uint16 sdata[8];     // Send data buffer
Uint16 rdata[8];     // Receive data buffer
Uint16 rdata_point;  // Keep track of where we are
                     // in the data stream to check received data

void main(void)
{
   Uint16 i;

// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the DSP2833x_SysCtrl.c file.
   InitSysCtrl();

// Step 2. Initialize GPIO:
// This example function is found in the DSP2833x_Gpio.c file and
// illustrates how to set the GPIO to it's default state.
// InitGpio();  // Skipped for this example
// Setup only the GP I/O only for SPI-A functionality
   InitSpiaGpio();

// Step 3. Initialize PIE vector table:
// Disable and clear all CPU interrupts
   DINT;
   IER = 0x0000;
   IFR = 0x0000;

// Initialize PIE control registers to their default state:
// This function is found in the DSP2833x_PieCtrl.c file.
   InitPieCtrl();

// Initialize the PIE vector table with pointers to the shell Interrupt
// Service Routines (ISR).
// This will populate the entire table, even if the interrupt
// is not used in this example.  This is useful for debug purposes.
// The shell ISR routines are found in DSP2833x_DefaultIsr.c.
// This function is found in DSP2833x_PieVect.c.
   InitPieVectTable();

   SCIStdioInit();
   SCIPuts("\r\n ============Test Start===========.\r\n", -1);
   SCIPuts("Welcome to TL28335 SPI_LOOPBACK_INTERRUPTS Demo application.\r\n\r\n", -1);

// Interrupts that are used in this example are re-mapped to
// ISR functions found within this file.
   EALLOW;	// This is needed to write to EALLOW protected registers
   PieVectTable.SPIRXINTA = &spiRxFifoIsr;
   PieVectTable.SPITXINTA = &spiTxFifoIsr;
   EDIS;   // This is needed to disable write to EALLOW protected registers

// Step 4. Initialize all the Device Peripherals:
// This function is found in DSP2833x_InitPeripherals.c
// InitPeripherals(); // Not required for this example
   spi_fifo_init();	  // Initialize the SPI only

// Step 5. User specific code, enable interrupts:

// Initialize the send data buffer
   for(i=0; i<8; i++)
   {
      sdata[i] = i;
   }
   rdata_point = 0;

// Enable interrupts required for this example
   PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   // Enable the PIE block
   PieCtrlRegs.PIEIER6.bit.INTx1=1;     // Enable PIE Group 6, INT 1
   PieCtrlRegs.PIEIER6.bit.INTx2=1;     // Enable PIE Group 6, INT 2
   IER=0x20;                            // Enable CPU INT6
   EINT;                                // Enable Global Interrupts

// Step 6. IDLE loop. Just sit and loop forever (optional):
	for(;;);
}

// Some Useful local functions
void delay_loop()
{
    long      i;
    for (i = 0; i < 1000000; i++) {}
}

void error(void)
{
    __asm("     ESTOP0");	 //Test failed!! Stop!
    for (;;);
}

void spi_fifo_init()
{
// Initialize SPI FIFO registers
   SpiaRegs.SPICCR.bit.SPISWRESET=0; // Reset SPI

   SpiaRegs.SPICCR.all=0x001F;       //16-bit character, Loopback mode
   SpiaRegs.SPICTL.all=0x0017;       //Interrupt enabled, Master/Slave XMIT enabled
   SpiaRegs.SPISTS.all=0x0000;
   SpiaRegs.SPIBRR=0x0063;           // Baud rate
   SpiaRegs.SPIFFTX.all=0xC028;      // Enable FIFO's, set TX FIFO level to 8
   SpiaRegs.SPIFFRX.all=0x0028;      // Set RX FIFO level to 8
   SpiaRegs.SPIFFCT.all=0x00;
   SpiaRegs.SPIPRI.all=0x0010;

   SpiaRegs.SPICCR.bit.SPISWRESET=1;  // Enable SPI

   SpiaRegs.SPIFFTX.bit.TXFIFO=1;
   SpiaRegs.SPIFFRX.bit.RXFIFORESET=1;
}

__interrupt void spiTxFifoIsr(void)
{
 	Uint16 i;
    for(i=0;i<8;i++)
    {
 	   SpiaRegs.SPITXBUF=sdata[i];      // Send data
    }

    for(i=0;i<8;i++)                    // Increment data for next cycle
    {
 	   sdata[i] = sdata[i]+ 1;
    }


    SpiaRegs.SPIFFTX.bit.TXFFINTCLR=1;  // Clear Interrupt flag
	PieCtrlRegs.PIEACK.all|=0x20;  		// Issue PIE ACK
}

__interrupt void spiRxFifoIsr(void)
{
    Uint16 i;
    for(i=0;i<8;i++)
    {
	    rdata[i]=SpiaRegs.SPIRXBUF;		// Read data
	}
	for(i=0;i<8;i++)                    // Check received data
	{
	    if(rdata[i] != rdata_point+i) error();
	}
	rdata_point++;
	SpiaRegs.SPIFFRX.bit.RXFFOVFCLR=1;  // Clear Overflow flag
	SpiaRegs.SPIFFRX.bit.RXFFINTCLR=1; 	// Clear Interrupt flag
	PieCtrlRegs.PIEACK.all|=0x20;       // Issue PIE ack
}



//===========================================================================
// No more.
//===========================================================================


