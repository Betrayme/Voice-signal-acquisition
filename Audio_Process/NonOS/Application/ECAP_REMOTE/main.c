/*
 * 			广州创龙电子科技有限公司
 *
 * Copyright 2015 Tronlong All rights reserved
 */

/*
 * 功能描述：
 *     本例程通过ECAP捕获红外遥控发出的波形，判断遥控按下的按键，并且通过
 * 串口2打印按键信息通信结果通过串口2打印。
 *
 */

#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

#define  ECAP_CEVT1_INT                  0x0002u
#define  ECAP_CEVT2_INT                  0x0004u
#define  ECAP_CEVT3_INT                  0x0008u
#define  ECAP_CEVT4_INT                  0x0010u
#define  ECAP_CNTOVF_INT                 0x0020u
#define  ECAP_PRDEQ_INT                  0x0040u
#define  ECAP_CMPEQ_INT                  0x0080u

//红外遥控识别码(ID),每款遥控器的该值基本都不一样,但也有一样的.
//我们选用的遥控器识别码为0
#define REMOTE_ID 0

// Prototype statements for functions found within this file.
__interrupt void ecap5_isr(void);
void InitECapture(void);
void Fail(void);
unsigned char Remote_Scan(void);

//遥控器接收状态
//[7]:收到了引导码标志
//[6]:得到了一个按键的所有信息
//[5:2]:保留
//[1:0]:溢出计时器
unsigned char 	RmtSta=0;

unsigned long RmtRec=0;	//红外接收到的数据
unsigned char  RmtCnt=0;	//按键按下的次数

void main(void)
{
	char *str;
	unsigned char key=0;

	// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the DSP2833x_SysCtrl.c file.
   InitSysCtrl();

// Step 2. Initialize GPIO:
// This example function is found in the DSP2833x_Gpio.c file and
// illustrates how to set the GPIO to it's default state.
// InitGpio();  // Skipped for this example

   InitECap5Gpio();

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
   SCIPuts("Welcome to TL28335 ECAP Remote Demo application.\r\n\r\n", -1);

// Interrupts that are used in this example are re-mapped to
// ISR functions found within this file.
   EALLOW;  // This is needed to write to EALLOW protected registers
   PieVectTable.ECAP5_INT = &ecap5_isr;
   EDIS;    // This is needed to disable write to EALLOW protected registers

// Step 4. Initialize all the Device Peripherals:
// This function is found in DSP2833x_InitPeripherals.c
// InitPeripherals();  // Not required for this example
//   InitEPwmTimer();    // For this example, only initialize the ePWM Timers
   InitECapture();

// Step 5. User specific code, enable interrupts:

// Enable CPU INT4 which is connected to ECAP1-4 INT:
   IER |= M_INT4;

// Enable eCAP INTn in the PIE: Group 3 interrupt 1-6
   PieCtrlRegs.PIEIER4.bit.INTx5 = 1;

// Enable global Interrupts and higher priority real-time debug events:
   EINT;   // Enable Global interrupt INTM
   ERTM;   // Enable Global realtime interrupt DBGM

   ECap5Regs.ECCLR.all = 0xFFFF;              // Clear all CAP interrupt flags//

// Step 6. IDLE loop. Just sit and loop forever (optional):

   	// 主循环
   	for(;;)
   	{
		key = Remote_Scan();
		if(key)
		{
			switch(key)
			{
				case 162:	str="CH-";	break;
				case 98:	str="CH";	break;
				case 226:	str="CH+";	break;
				case 34:	str="|<<";	break;
				case 2:		str=">>|";	break;
				case 194:	str=">||";	break;
				case 224:	str="-";	break;
				case 168:	str="+";	break;
				case 144:	str="EQ";	break;
				case 104:	str="0";	break;
				case 152:	str="100+";	break;
				case 176:	str="200+";	break;
				case 48:	str="1";	break;
				case 24:	str="2";	break;
				case 122:	str="3";	break;
				case 16:	str="4";	break;
				case 56:	str="5";	break;
				case 90:	str="6";	break;
				case 66:	str="7";	break;
				case 74:	str="8";	break;
				case 82:	str="9";	break;
			}
			SCIprintf("KEYVAL: %s, KEYCNT: %d\n", str, RmtCnt);
		}
	}
}

void InitECapture()
{
//   ECap5Regs.ECEINT.all = 0x0000;             // Disable all capture interrupts
   ECap5Regs.ECCLR.all = 0xFFFF;              // Clear all CAP interrupt flags//
//   ECap5Regs.ECCTL1.bit.CAPLDEN = 0;          // Disable CAP1-CAP4 register loads
   ECap5Regs.ECCTL2.bit.TSCTRSTOP = 0;        // Make sure the counter is stopped

   // Configure peripheral registers
   ECap5Regs.ECCTL2.bit.CONT_ONESHT = 0;      // Continuoud mode//
   ECap5Regs.ECCTL2.bit.STOP_WRAP = 3;        // Continuoud at 4 events //
   ECap5Regs.ECCTL1.bit.CAP1POL = 1;          // Falling edge//
   ECap5Regs.ECCTL1.bit.CAP2POL = 1;          // Falling edge//
   ECap5Regs.ECCTL1.bit.CAP3POL = 1;          // Falling edge//
   ECap5Regs.ECCTL1.bit.CAP4POL = 1;          // Falling edge  //
   ECap5Regs.ECCTL1.bit.CTRRST1 = 1;          // Difference operation//
   ECap5Regs.ECCTL1.bit.CTRRST2 = 1;          // Difference operation//
   ECap5Regs.ECCTL1.bit.CTRRST3 = 1;          // Difference operation//
   ECap5Regs.ECCTL1.bit.CTRRST4 = 1;          // Difference operation//
   ECap5Regs.ECCTL2.bit.SYNCI_EN = 0;         // Disable sync in//
   ECap5Regs.ECCTL2.bit.SYNCO_SEL = 2;        // Disable sync out//
   ECap5Regs.ECCTL1.bit.CAPLDEN = 1;          // Enable capture units //
   ECap5Regs.ECCTL2.bit.CAP_APWM = 0;         // Capture Mode//
   ECap5Regs.TSCTR = 0xffffffff - 15000000;   // 100ms 溢出 //

//   ECap5Regs.ECCTL2.bit.REARM = 1;            // arm one-shot
   ECap5Regs.ECCTL1.bit.CAPLDEN = 1;          // Enable CAP1-CAP4 register loads //
   ECap5Regs.ECEINT.bit.CEVT4 = 1;            // 4 events = interrupt //
   ECap5Regs.ECEINT.bit.CEVT2 = 1;//
   ECap5Regs.ECEINT.bit.CEVT1 = 1;//
   ECap5Regs.ECEINT.bit.CEVT3 = 1;//
   ECap5Regs.ECEINT.bit.CTROVF = 1;//
   ECap5Regs.ECCLR.bit.INT = 1;//
   ECap5Regs.ECCTL2.bit.TSCTRSTOP = 1;        // Start Counter

}


__interrupt void ecap5_isr(void)
{
//   ECap1Regs.ECCTL2.bit.REARM = 1;
   unsigned long value;
   	unsigned long intFlag;

   	intFlag = ECap5Regs.ECFLG.all;
   	ECap5Regs.ECCLR.all = intFlag;

   	if(intFlag & ECAP_CEVT1_INT)
   	{
   		value = ECap5Regs.CAP1;
   	}
   	if(intFlag & ECAP_CEVT2_INT)
   	{
   		value = ECap5Regs.CAP2;
   	}
   	if(intFlag & ECAP_CEVT3_INT)
   	{
   		value = ECap5Regs.CAP3;
   	}
   	if(intFlag & ECAP_CEVT4_INT)
   	{
   		value = ECap5Regs.CAP4;
   	}
   	if(intFlag & ECAP_CNTOVF_INT)
   	{
   		ECap5Regs.TSCTR = 0xffffffff - 15000000;   // 100ms 溢出 //
   		if(RmtSta & 0X80)//接收到了引导码
   		{
   			RmtSta++;
   			if((RmtSta & 0x0f) >= 2)
   			{
   				RmtSta |= 1<<6; //得到了一个键的所有信息
   				RmtSta&=~(1<<7);//清空引导标识
   				RmtSta&=0XF0;	//清空计数器
   			}
   		}
   	}

   	ECap5Regs.ECCLR.bit.INT = 1;



   	value = value/150;   //将值转换为微秒us
   	if(RmtSta & 0X80)//接收到了引导码
   	{

   		if(value > 800 && value < 1400)			//1125为标准值,1125us
   		{
   			RmtRec <<= 1;	//左移一位.
   			RmtRec |= 0;	//接收到0
   		}
   		else if(value > 1800 && value < 2500)	// 2250为标准值,2250us
   		{
   			RmtRec <<= 1;	//左移一位.
   			RmtRec |= 1;	//接收到1
   		}
   		else if(value > 11000 && value < 12000)	//得到按键键值增加的信息 11500为标准值11.5ms
   		{
   			RmtCnt++; 		//按键次数增加1次
   			RmtSta &= 0XF0;	//清空计时器
   			ECap5Regs.TSCTR = 0xffffffff - 15000000;   // 100ms 溢出 //
   		}
   	}
   	else if(value > 13200 && value < 13700)		//13500为标准值13.5ms
   	{
   		RmtSta |= 1 << 7;	//标记成功接收到了引导码
   		RmtCnt = 0;		//清除按键次数计数器
   	}

   	// Acknowledge this interrupt to receive more interrupts from group 4
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP4;
}


/****************************************************************************/
/*                                                                          */
/*              红外遥控扫描                                                */
/*                                                                          */
/****************************************************************************/
unsigned char Remote_Scan(void)
{
	unsigned char sta = 0;
    unsigned char t1,t2;
	if(RmtSta & (1<<6))//得到一个按键的所有信息了
	{
	    t1 = RmtRec >> 24;			//得到地址码
	    t2 = (RmtRec >> 16) & 0xff;	//得到地址反码

	    t1 = t1 & 0xff;
	    t2 = (~t2) & 0xff;
 	    if(t1 == t2 && t1 == REMOTE_ID)//检验遥控识别码(ID)及地址
	    {
	        t1 = RmtRec >> 8;
	        t2 = RmtRec;

	        t1 = t1 & 0xff;
			t2 = (~t2) & 0xff;
	        if(t1 == t2) sta = t1;//键值正确
		}
		if((sta==0) || ((RmtSta&0X80)==0))//按键数据错误/遥控已经没有按下了
		{
		 	RmtSta &= ~(1 << 6);//清除接收到有效按键标识
		}
	}
    return sta;
}


void Fail()
{
    __asm("   ESTOP0");
}




//===========================================================================
// No more.
//===========================================================================
