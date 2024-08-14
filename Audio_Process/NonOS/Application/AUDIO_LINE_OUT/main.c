/*
 * 			广州创龙电子科技有限公司
 *
 * Copyright 2015 Tronlong All rights reserved
 */

/*
 * 功能描述：
 *     本例程会通过Line Out播放报警声.
 *
 */

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

void I2CA_Init(void);
Uint16 AIC23Write(int Address,int Data);
void Delay(int time);
void delay();
int DA_wptr,y;
Uint16 j=3;

void main(void)
{
	Uint16	temp,i;

   InitSysCtrl();

   InitMcbspaGpio();	//zq
   InitI2CGpio();

// Disable CPU interrupts
   DINT;

   InitPieCtrl();

// Disable CPU interrupts and clear all CPU interrupt flags:
   IER = 0x0000;
   IFR = 0x0000;

   InitPieVectTable();

   SCIStdioInit();
   SCIPuts("\r\n ============Test Start===========.\r\n", -1);
   SCIPuts("Welcome to TL28335 Audio Line Out Demo application.\r\n\r\n", -1);

   I2CA_Init();
   
   // Clear Counters
  // PassCount = 0;
  // FailCount = 0;
	
	AIC23Write(0x00,0x00);
	Delay(100);
	AIC23Write(0x02,0x00);
	Delay(100);
	AIC23Write(0x04,0x7f);
	Delay(100);
	AIC23Write(0x06,0x7f);
	Delay(100);
	AIC23Write(0x08,0x14);
	Delay(100);
	AIC23Write(0x0A,0x00);
	Delay(100);
	AIC23Write(0x0C,0x00);
	Delay(100);
	AIC23Write(0x0E,0x43);
	Delay(100);
	AIC23Write(0x10,0x23);
	Delay(100);
	AIC23Write(0x12,0x01);
	Delay(100);		//AIC23Init
	 
   InitMcbspa();          // Initalize the Mcbsp-A

//发出报警声
    while(1)
    {
       for(temp=30000;temp>0;temp-=100)
		{
      		for(i=0;i<2;i++)
      		{
         		y=5000;       
         		delay(temp); 
				McbspaRegs.DXR1.all = y;// 输出左声道数据
				McbspaRegs.DXR2.all = y;// 输出右声道数据
         		y=-5000;       
         		delay(temp);
				McbspaRegs.DXR1.all = y;// 输出左声道数据
				McbspaRegs.DXR2.all = y;// 输出右声道数据
      		}
		}
      }  
}   // end of main

void I2CA_Init(void)
{
   // Initialize I2C
   I2caRegs.I2CSAR = 0x001A;		// Slave address - EEPROM control code

   #if (CPU_FRQ_150MHZ)             // Default - For 150MHz SYSCLKOUT
        I2caRegs.I2CPSC.all = 14;   // Prescaler - need 7-12 Mhz on module clk (150/15 = 10MHz)
   #endif
   #if (CPU_FRQ_100MHZ)             // For 100 MHz SYSCLKOUT
     I2caRegs.I2CPSC.all = 9;	    // Prescaler - need 7-12 Mhz on module clk (100/10 = 10MHz)
   #endif

   I2caRegs.I2CCLKL = 100;			// NOTE: must be non zero
   I2caRegs.I2CCLKH = 100;			// NOTE: must be non zero
   I2caRegs.I2CIER.all = 0x24;		// Enable SCD & ARDY interrupts

//   I2caRegs.I2CMDR.all = 0x0020;	// Take I2C out of reset
   I2caRegs.I2CMDR.all = 0x0420;	// Take I2C out of reset		//zq
   									// Stop I2C when suspended

   I2caRegs.I2CFFTX.all = 0x6000;	// Enable FIFO mode and TXFIFO
   I2caRegs.I2CFFRX.all = 0x2040;	// Enable RXFIFO, clear RXFFINT,

   return;
}

Uint16 AIC23Write(int Address,int Data)
{
   if (I2caRegs.I2CMDR.bit.STP == 1)
   {
      return I2C_STP_NOT_READY_ERROR;
   }

   // Setup slave address
   I2caRegs.I2CSAR = 0x1A;

   // Check if bus busy
   if (I2caRegs.I2CSTR.bit.BB == 1)
   {
      return I2C_BUS_BUSY_ERROR;
   }

   // Setup number of bytes to send
   // MsgBuffer + Address
   I2caRegs.I2CCNT = 2;
   I2caRegs.I2CDXR = Address;
   I2caRegs.I2CDXR = Data;
   // Send start as master transmitter
   I2caRegs.I2CMDR.all = 0x6E20;
   return I2C_SUCCESS;
}

void Delay(int time)
{
	int i,j,k=0;
	for(i=0;i<time;i++)
		for(j=0;j<1024;j++)
			k++;
}

void delay(Uint32 k)
{
   while(k--);
}



//===========================================================================
// No more.
//===========================================================================
