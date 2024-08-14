/*
 * 			广州创龙电子科技有限公司
 *
 * Copyright 2015 Tronlong All rights reserved
 */

/*
 * 功能描述：
 *     向sd卡创建test.txt,并写入 www.tronlong.com。
 *
 */

#include "DSP2833x_Device.h"     // DSP2823x Headerfile Include File
#include "DSP2833x_examples.h"   // DSP2823x Examples Include File
extern	char play_Udisc();

void main(void)
{
   char status;

// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the DSP2833x_SysCtrl.c file.
   InitSysCtrl();

// Step 2. Initialize GPIO:
// This example function is found in the DSP2833x_Gpio.c file and
// illustrates how to set the GPIO to it's default state.
// Setup only the GP I/O only for SPI-A functionality
// This function is found in DSP2833x_Spi.c
   InitSpiaGpio();

   EALLOW;
   // SN74HC138
   GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 0; // 配置GPIO11为GPIO口
   GpioCtrlRegs.GPADIR.bit.GPIO11 = 1;      // 定义GPIO17输出引脚
   GpioDataRegs.GPADAT.bit.GPIO11 = 0;      // 输出低电平

   // SN74HC138 作为SPI_CS1_SD
   GpioCtrlRegs.GPBMUX2.bit.GPIO57 = 0; // 配置GPIO11为GPIO口
   GpioCtrlRegs.GPBDIR.bit.GPIO57 = 1;      // 定义GPIO17输出引脚
   GpioDataRegs.GPBDAT.bit.GPIO57 = 0;      // 输出低电平
   EDIS;

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

   SCIStdioInit();
   SCIPuts("\r\n ============Test Start===========.\r\n", -1);
   SCIPuts("Welcome to TL28335 SD FAT32 Demo application.\r\n\r\n", -1);
	
// Step 4. Initialize all the Device Peripherals:
// This function is found in DSP2833x_InitPeripherals.c
// InitPeripherals(); // Not required for this example
   spi_initialization();					//Initialize SPI

// Step 5. User specific code:
// Interrupts are not used in this example. 
   sd_card_insertion();			//Check for card insertion
   sd_initialization();			//Initialize card

    sd_read_register(SEND_CSD);		//Read CSD register
	sd_read_register(READ_OCR);		//Read OCR register
	sd_read_register(SEND_CID);		//Read CID register

//	sd_write_block(0, write_buffer);	//Write to sector specified
//	sd_read_block(0, read_buffer);	//Read from sector specified
//  sd_erase_block(0, 1);				//Erase sector range specified
//	sd_read_block(0, read_buffer);	//Read from sector specified
   status = play_Udisc();

   if(status == TRUE)
	   SCIPuts("Test succeed.\r\n", -1);
   else
	   SCIPuts("Test failed.\r\n", -1);

   SCIPuts("\r\n ============Test End===========.\r\n", -1);
    for (;;);
} 	
