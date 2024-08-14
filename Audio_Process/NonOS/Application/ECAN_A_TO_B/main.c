/*
 * 			广州创龙电子科技有限公司
 *
 * Copyright 2015 Tronlong All rights reserved
 */

/*
 * 功能描述：
 *     本例程通过CANA和CANB做测试，通过CANA发送，CANB接收,通信结果通过串口2打印
 * 接线：CANA-L连接CANB-L	CANA-H连接CANB-H
 *
 */

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

#define TXCOUNT  100  // Transmission will take place (TXCOUNT) times..

// Globals for this example
long      i,j;
long 	  loopcount = 0;
volatile struct MBOX *Mailbox;
Uint32  ErrorCount;
Uint32  PassCount;
Uint32  MessageReceivedCount;

Uint32  TestMbox1 = 0;
Uint32  TestMbox2 = 0;
Uint32  TestMbox3 = 0;


void mailbox_check(int32 T1, int32 T2, int32 T3)
{
    if((T1 !=0x55555555 ) || ( T2 != 0xAAAAAAAA)|| ( T3 != 0x95555555))
    {
       ErrorCount++;
    }
    else
    {
       PassCount++;
    }
}


void main()
{

/* Create a shadow register structure for the CAN control registers. This is
 needed, since, only 32-bit access is allowed to these registers. 16-bit access
 to these registers could potentially corrupt the register contents. This is
 especially true while writing to a bit (or group of bits) among bits 16 - 31 */

   struct ECAN_REGS ECanaShadow;
   struct ECAN_REGS ECanbShadow;
// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the DSP2833x_SysCtrl.c file.
   InitSysCtrl();

// Step 2. Initalize GPIO:
// This example function is found in the DSP2833x_Gpio.c file and
// illustrates how to set the GPIO to it's default state.
// InitGpio();  // Skipped for this example

   // Just initalize eCAN pins for this example
   // This function is in DSP2833x_ECan.c
   InitECanGpio();

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

// Interrupts that are used in this example are re-mapped to
// ISR functions found within this file.

// No interrupts used in this example.

// Step 4. Initialize all the Device Peripherals:
// This function is found in DSP2833x_InitPeripherals.c
// InitPeripherals(); // Not required for this example

   SCIStdioInit();
   SCIPuts("\r\n ============Test Start===========.\r\n", -1);
   SCIPuts("Welcome to TL28335 ECAN-A To ECAN-B Demo application.\r\n\r\n", -1);

   // In this case just initalize eCAN-A and eCAN-B
   // This function is in DSP2833x_ECan.c
   InitECan();
   ErrorCount = 0;
   PassCount = 0;
    // Step 5. User specific code:

/* Write to the MSGID field  */

   ECanaMboxes.MBOX25.MSGID.all = 0x95555555; // Extended Identifier

   ECanbMboxes.MBOX25.MSGID.all = 0x95555555; // Extended Identifier
/* Configure Mailbox under test as a Transmit mailbox */

   ECanaShadow.CANMD.all = ECanaRegs.CANMD.all;
   ECanaShadow.CANMD.bit.MD25 = 0;
   ECanaRegs.CANMD.all = ECanaShadow.CANMD.all;

   ECanbShadow.CANMD.all = ECanbRegs.CANMD.all;
   ECanbShadow.CANMD.bit.MD25 = 1;
   ECanbRegs.CANMD.all = ECanbShadow.CANMD.all;

/* Enable Mailbox under test */

   ECanaShadow.CANME.all = ECanaRegs.CANME.all;
   ECanaShadow.CANME.bit.ME25 = 1;
   ECanaRegs.CANME.all = ECanaShadow.CANME.all;

   ECanbShadow.CANME.all = ECanbRegs.CANME.all;
   ECanbShadow.CANME.bit.ME25 = 1;
   ECanbRegs.CANME.all = ECanbShadow.CANME.all;

/* Write to DLC field in Master Control reg */

   ECanaMboxes.MBOX25.MSGCTRL.bit.DLC = 8;

/* Write to the mailbox RAM field */

   ECanaMboxes.MBOX25.MDL.all = 0x55555555;
   ECanaMboxes.MBOX25.MDH.all = 0xAAAAAAAA;

/* Begin transmitting */

   SCIPuts("Begin transmitting.\r\n", -1);
   for(i=0; i < TXCOUNT; i++)
   {
       ECanaShadow.CANTRS.all = 0;
       ECanaShadow.CANTRS.bit.TRS25 = 1;             // Set TRS for mailbox under test
       ECanaRegs.CANTRS.all = ECanaShadow.CANTRS.all;

       do
	   {
    	   ECanaShadow.CANTA.all = ECanaRegs.CANTA.all;
	   } while(ECanaShadow.CANTA.bit.TA25 == 0 );   // Wait for TA5 bit to be set..//如果线没有连接，如果线连接错误


       ECanaShadow.CANTA.all = 0;
       ECanaShadow.CANTA.bit.TA25 = 1;     	         // Clear TA5
       ECanaRegs.CANTA.all = ECanaShadow.CANTA.all;

       Mailbox = &ECanbMboxes.MBOX0 + 25;  //CANB读数据
       TestMbox1 = Mailbox->MDL.all; // = 0x (n is the MBX number)
       TestMbox2 = Mailbox->MDH.all; // = 0x (a constant)
       TestMbox3 = Mailbox->MSGID.all;// = 0x (n is the MBX number)
       mailbox_check(TestMbox1,TestMbox2,TestMbox3); // Checks the received data

       loopcount ++;
    }
	if(ErrorCount == 0)
	{
	  // OK，数据校验正确
	  SCIPuts("Verify successfully.\r\n", -1);
	}
	else
	{
	  // ERROR，
	  SCIPuts("Verify failed.\r\n", -1);
	}

	SCIPuts("\r\n ============Test End===========.\r\n", -1);

	while(1);
}


