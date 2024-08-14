/*
 * 			广州创龙电子科技有限公司
 *
 * Copyright 2015 Tronlong All rights reserved
 */

/*
 * 功能描述：
 *
 * 用i2c设置日期，07年12月3日，15点10分30秒，会在串口打印实时时间
 *
 */

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

void   I2CA_Init(void);
Uint16 I2CA_WriteData(struct I2CMSG *msg);
Uint16 I2CA_ReadData(struct I2CMSG *msg);
void   WriteData(struct I2CMSG *msg,Uint16 *MsgBuffer,Uint16 MemoryAdd,Uint16 NumOfBytes);
interrupt void i2c_int1a_isr(void);
void pass(void);
void fail(void);

//I2C通信相关参数
#define I2C_SLAVE_ADDR       0x6f
#define I2C_NUMBYTES          1
#define I2C_RNUMBYTES         8
#define I2C_RTC_HIGH_ADDR  0x00
#define I2C_RTC_LOW_ADDR   0x00

//宏定义时间
Uint16	YEAR = 0x2007;
Uint16	MONTH = 0x12;
Uint16	DAY = 0x03;
Uint16	WEEK = 0x01;
Uint16	HOUR = 0x15;
Uint16	MINUTE = 0x10;
Uint16	SECOND = 0x80;

//寄存器地址
#define	DW		0x0003
#define	YR		0x0006
#define	MO		0x0005
#define	DT		0x0004
#define	HR		0x0002
#define	MN		0x0001
#define	SC		0x0000

struct I2CMSG I2cMsgOut1={I2C_MSGSTAT_SEND_WITHSTOP,
                          I2C_SLAVE_ADDR,
                          I2C_NUMBYTES,
                          I2C_RTC_HIGH_ADDR,
                          I2C_RTC_LOW_ADDR};

struct I2CMSG I2cMsgIn1={ I2C_MSGSTAT_SEND_NOSTOP,
                          I2C_SLAVE_ADDR,
                          I2C_RNUMBYTES,
                          I2C_RTC_HIGH_ADDR,
                          I2C_RTC_LOW_ADDR};

struct I2CMSG *CurrentMsgPtr;				// Used in interrupts
Uint16 PassCount;
Uint16 FailCount;

void main(void)
{

   Uint16 i;

   CurrentMsgPtr = &I2cMsgOut1;

   InitSysCtrl();

   InitI2CGpio();

   DINT;

   InitPieCtrl();

   IER = 0x0000;
   IFR = 0x0000;

   InitPieVectTable();

   EALLOW;
   PieVectTable.I2CINT1A = &i2c_int1a_isr;
   EDIS;

   I2CA_Init();

   SCIStdioInit();
   SCIPuts("\r\n Test Start \r\n", -1);
   SCIPuts("Welcome to TL28335 RTC Demo application.\r\n\r\n", -1);

// Enable I2C interrupt 1 in the PIE: Group 8 interrupt 1
   PieCtrlRegs.PIEIER8.bit.INTx1 = 1;

// Enable CPU INT8 which is connected to PIE group 8
   IER |= M_INT8;
   EINT;

   i = SECOND;
   WriteData(&I2cMsgOut1,&i,0,1);

   i = YEAR;
   WriteData(&I2cMsgOut1,&i,YR,1);
   i = MINUTE;
   WriteData(&I2cMsgOut1,&i,MN,1);
   i = HOUR;
   WriteData(&I2cMsgOut1,&i,HR,1);
   i = WEEK;
   WriteData(&I2cMsgOut1,&i,DW,1);
   i = SECOND;
   WriteData(&I2cMsgOut1,&i,SC,1);
   i = MONTH;
   WriteData(&I2cMsgOut1,&i,MO,1);
   i =DAY;
   WriteData(&I2cMsgOut1,&i,DT,1);

   SCIPuts("printf the time\r\n\0 ", -1);

   for(;;)
   {

      // Check outgoing message status. Bypass read section if status is
      // not inactive.
	   I2cMsgOut1.MsgStatus = I2C_MSGSTAT_INACTIVE;
      if (I2cMsgOut1.MsgStatus == I2C_MSGSTAT_INACTIVE)
      {
         // Check incoming message status.
         if(I2cMsgIn1.MsgStatus == I2C_MSGSTAT_SEND_NOSTOP)
         {
            // RTC address setup portion
            while(I2CA_ReadData(&I2cMsgIn1) != I2C_SUCCESS)
            {
               // Maybe setup an attempt counter to break an infinite while
               // loop. The RTC will send back a NACK while it is performing
               // a write operation. Even though the write communique is
               // complete at this point, the RTC could still be busy
               // programming the data. Therefore, multiple attempts are
               // necessary.
            }
            // Update current message pointer and message status
            CurrentMsgPtr = &I2cMsgIn1;
            I2cMsgIn1.MsgStatus = I2C_MSGSTAT_SEND_NOSTOP_BUSY;
         }

         // Once message has progressed past setting up the internal address
         // of the RTC, send a restart to read the data bytes from the
         // RTC. Complete the communique with a stop bit. MsgStatus is
         // updated in the interrupt service routine.
         else if(I2cMsgIn1.MsgStatus == I2C_MSGSTAT_RESTART)
         {
            DELAY_US(1000000);

			YEAR = I2cMsgIn1.MsgBuffer[6];
			MONTH = I2cMsgIn1.MsgBuffer[5];
			DAY = I2cMsgIn1.MsgBuffer[4];
			WEEK = I2cMsgIn1.MsgBuffer[3];
			HOUR = I2cMsgIn1.MsgBuffer[2];
			MINUTE = I2cMsgIn1.MsgBuffer[1];
			SECOND = I2cMsgIn1.MsgBuffer[0];

			SCIprintf("\r %x-%x-%x    %x:%x%x:%x%x                          ",YEAR,MONTH,DAY,
			          HOUR,(MINUTE&0x70)>>4,MINUTE&0xf,(SECOND&0x70)>>4,SECOND&0x0f);

            // Read data portion
            while(I2CA_ReadData(&I2cMsgIn1) != I2C_SUCCESS)
            {
               // Maybe setup an attempt counter to break an infinite while
               // loop.
            }
            // Update current message pointer and message status
            CurrentMsgPtr = &I2cMsgIn1;
            I2cMsgIn1.MsgStatus = I2C_MSGSTAT_READ_BUSY;
         }
      }  // end of read section
   }   // end of for(;;)
}   // end of main

void I2CA_Init(void)
{
   // Initialize I2C
   I2caRegs.I2CMDR.all = 0x0000;	// Take I2C reset
   									// Stop I2C when suspended

   I2caRegs.I2CFFTX.all = 0x0000;	// Disable FIFO mode and TXFIFO
   I2caRegs.I2CFFRX.all = 0x0040;	// Disable RXFIFO, clear RXFFINT,

   #if (CPU_FRQ_150MHZ)             // Default - For 150MHz SYSCLKOUT
        I2caRegs.I2CPSC.all = 14;   // Prescaler - need 7-12 Mhz on module clk (150/15 = 10MHz)
   #endif
   #if (CPU_FRQ_100MHZ)             // For 100 MHz SYSCLKOUT
     I2caRegs.I2CPSC.all = 9;	    // Prescaler - need 7-12 Mhz on module clk (100/10 = 10MHz)
   #endif

   I2caRegs.I2CCLKL = 10;			// NOTE: must be non zero
   I2caRegs.I2CCLKH = 5;			// NOTE: must be non zero
   I2caRegs.I2CIER.all = 0x24;		// Enable SCD & ARDY interrupts

   I2caRegs.I2CMDR.all = 0x0020;	// Take I2C out of reset
   									// Stop I2C when suspended

   I2caRegs.I2CFFTX.all = 0x6000;	// Enable FIFO mode and TXFIFO
   I2caRegs.I2CFFRX.all = 0x2040;	// Enable RXFIFO, clear RXFFINT,

   return;
}


Uint16 I2CA_WriteData(struct I2CMSG *msg)
{
   Uint16 i;

   // Wait until the STP bit is cleared from any previous master communication.
   // Clearing of this bit by the module is delayed until after the SCD bit is
   // set. If this bit is not checked prior to initiating a new message, the
   // I2C could get confused.
   if (I2caRegs.I2CMDR.bit.STP == 1)
   {
      return I2C_STP_NOT_READY_ERROR;
   }

   // Setup slave address
   I2caRegs.I2CSAR = msg->SlaveAddress;

   // Check if bus busy
   if (I2caRegs.I2CSTR.bit.BB == 1)
   {
      return I2C_BUS_BUSY_ERROR;
   }

   // Setup number of bytes to send
   // MsgBuffer + Address
   I2caRegs.I2CCNT = msg->NumOfBytes+1;

   // Setup data to send
   I2caRegs.I2CDXR = msg->MemoryHighAddr;
//   I2caRegs.I2CDXR = msg->MemoryLowAddr;
// for (i=0; i<msg->NumOfBytes-2; i++)
   for (i=0; i<msg->NumOfBytes; i++)

   {
      I2caRegs.I2CDXR = *(msg->MsgBuffer+i);
   }

   // Send start as master transmitter
   I2caRegs.I2CMDR.all = 0x6E20;

   return I2C_SUCCESS;
}


Uint16 I2CA_ReadData(struct I2CMSG *msg)
{
   // Wait until the STP bit is cleared from any previous master communication.
   // Clearing of this bit by the module is delayed until after the SCD bit is
   // set. If this bit is not checked prior to initiating a new message, the
   // I2C could get confused.
   if (I2caRegs.I2CMDR.bit.STP == 1)
   {
      return I2C_STP_NOT_READY_ERROR;
   }

   I2caRegs.I2CSAR = msg->SlaveAddress;

   if(msg->MsgStatus == I2C_MSGSTAT_SEND_NOSTOP)
   {
      // Check if bus busy
      if (I2caRegs.I2CSTR.bit.BB == 1)
      {
         return I2C_BUS_BUSY_ERROR;
      }
      I2caRegs.I2CCNT = 1;
      I2caRegs.I2CDXR = msg->MemoryHighAddr;
      I2caRegs.I2CMDR.all = 0x2620;			// Send data to setup RTC address
   }
   else if(msg->MsgStatus == I2C_MSGSTAT_RESTART)
   {
      I2caRegs.I2CCNT = msg->NumOfBytes;	// Setup how many bytes to expect
      I2caRegs.I2CMDR.all = 0x2C20;			// Send restart as master receiver
   }

   return I2C_SUCCESS;
}

interrupt void i2c_int1a_isr(void)     // I2C-A
{
   Uint16 IntSource, i;

   // Read interrupt source
   IntSource = I2caRegs.I2CISRC.all;

   // Interrupt source = stop condition detected
   if(IntSource == I2C_SCD_ISRC)
   {
      // If completed message was writing data, reset msg to inactive state
      if (CurrentMsgPtr->MsgStatus == I2C_MSGSTAT_WRITE_BUSY)
      {
         CurrentMsgPtr->MsgStatus = I2C_MSGSTAT_INACTIVE;
      }
      else
      {
         // If a message receives a NACK during the address setup portion of the
         // RTC read, the code further below included in the register access ready
         // interrupt source code will generate a stop condition. After the stop
         // condition is received (here), set the message status to try again.
         // User may want to limit the number of retries before generating an error.
         if(CurrentMsgPtr->MsgStatus == I2C_MSGSTAT_SEND_NOSTOP_BUSY)
         {
            CurrentMsgPtr->MsgStatus = I2C_MSGSTAT_SEND_NOSTOP;
         }
         // If completed message was reading RTC data, reset msg to inactive state
         // and read data from FIFO.
         else if (CurrentMsgPtr->MsgStatus == I2C_MSGSTAT_READ_BUSY)
         {
            CurrentMsgPtr->MsgStatus = I2C_MSGSTAT_SEND_NOSTOP;//I2C_MSGSTAT_INACTIVE;
            for(i=0; i < CurrentMsgPtr->NumOfBytes; i++)
            {
              CurrentMsgPtr->MsgBuffer[i] = I2caRegs.I2CDRR;
            }
		 }
      }
   }  // end of stop condition detected

   // Interrupt source = Register Access Ready
   // This interrupt is used to determine when the RTC address setup portion of the
   // read data communication is complete. Since no stop bit is commanded, this flag
   // tells us when the message has been sent instead of the SCD flag. If a NACK is
   // received, clear the NACK bit and command a stop. Otherwise, move on to the read
   // data portion of the communication.
   else if(IntSource == I2C_ARDY_ISRC)
   {
      if(I2caRegs.I2CSTR.bit.NACK == 1)
      {
         I2caRegs.I2CMDR.bit.STP = 1;
         I2caRegs.I2CSTR.all = I2C_CLR_NACK_BIT;
      }
      else if(CurrentMsgPtr->MsgStatus == I2C_MSGSTAT_SEND_NOSTOP_BUSY)
      {
         CurrentMsgPtr->MsgStatus = I2C_MSGSTAT_RESTART;
      }
   }  // end of register access ready

   else
   {
      // Generate some error due to invalid interrupt source
      asm("   ESTOP0");
   }

   // Enable future I2C (PIE Group 8) interrupts
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;
}

void pass()
{
    asm("   ESTOP0");
    for(;;);
}

void fail()
{
    asm("   ESTOP0");
    for(;;);
}

void WriteData(struct I2CMSG *msg,Uint16 *MsgBuffer,Uint16 MemoryAdd,Uint16 NumOfBytes)
{
	Uint16 i,Error;
	for(i = 0; i < NumOfBytes; i++)
	{
		msg->MsgBuffer[i] = MsgBuffer[i];		
	}

	msg->MemoryHighAddr = MemoryAdd & 0xff;
//	msg->MemoryHighAddr = MemoryAdd >> 8;
	msg->MemoryLowAddr = MemoryAdd & 0xff;
	msg->NumOfBytes = NumOfBytes;
	Error = I2CA_WriteData(&I2cMsgOut1);

	if (Error == I2C_SUCCESS)
	{
		CurrentMsgPtr = &I2cMsgOut1;
		I2cMsgOut1.MsgStatus = I2C_MSGSTAT_WRITE_BUSY;
	}
	while(I2cMsgOut1.MsgStatus != I2C_MSGSTAT_INACTIVE);
	DELAY_US(1000);
}

//===========================================================================
// No more.
//===========================================================================
