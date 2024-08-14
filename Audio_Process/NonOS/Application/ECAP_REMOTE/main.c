/*
 * 			���ݴ������ӿƼ����޹�˾
 *
 * Copyright 2015 Tronlong All rights reserved
 */

/*
 * ����������
 *     ������ͨ��ECAP�������ң�ط����Ĳ��Σ��ж�ң�ذ��µİ���������ͨ��
 * ����2��ӡ������Ϣͨ�Ž��ͨ������2��ӡ��
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

//����ң��ʶ����(ID),ÿ��ң�����ĸ�ֵ��������һ��,��Ҳ��һ����.
//����ѡ�õ�ң����ʶ����Ϊ0
#define REMOTE_ID 0

// Prototype statements for functions found within this file.
__interrupt void ecap5_isr(void);
void InitECapture(void);
void Fail(void);
unsigned char Remote_Scan(void);

//ң��������״̬
//[7]:�յ����������־
//[6]:�õ���һ��������������Ϣ
//[5:2]:����
//[1:0]:�����ʱ��
unsigned char 	RmtSta=0;

unsigned long RmtRec=0;	//������յ�������
unsigned char  RmtCnt=0;	//�������µĴ���

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

   	// ��ѭ��
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
   ECap5Regs.TSCTR = 0xffffffff - 15000000;   // 100ms ��� //

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
   		ECap5Regs.TSCTR = 0xffffffff - 15000000;   // 100ms ��� //
   		if(RmtSta & 0X80)//���յ���������
   		{
   			RmtSta++;
   			if((RmtSta & 0x0f) >= 2)
   			{
   				RmtSta |= 1<<6; //�õ���һ������������Ϣ
   				RmtSta&=~(1<<7);//���������ʶ
   				RmtSta&=0XF0;	//��ռ�����
   			}
   		}
   	}

   	ECap5Regs.ECCLR.bit.INT = 1;



   	value = value/150;   //��ֵת��Ϊ΢��us
   	if(RmtSta & 0X80)//���յ���������
   	{

   		if(value > 800 && value < 1400)			//1125Ϊ��׼ֵ,1125us
   		{
   			RmtRec <<= 1;	//����һλ.
   			RmtRec |= 0;	//���յ�0
   		}
   		else if(value > 1800 && value < 2500)	// 2250Ϊ��׼ֵ,2250us
   		{
   			RmtRec <<= 1;	//����һλ.
   			RmtRec |= 1;	//���յ�1
   		}
   		else if(value > 11000 && value < 12000)	//�õ�������ֵ���ӵ���Ϣ 11500Ϊ��׼ֵ11.5ms
   		{
   			RmtCnt++; 		//������������1��
   			RmtSta &= 0XF0;	//��ռ�ʱ��
   			ECap5Regs.TSCTR = 0xffffffff - 15000000;   // 100ms ��� //
   		}
   	}
   	else if(value > 13200 && value < 13700)		//13500Ϊ��׼ֵ13.5ms
   	{
   		RmtSta |= 1 << 7;	//��ǳɹ����յ���������
   		RmtCnt = 0;		//�����������������
   	}

   	// Acknowledge this interrupt to receive more interrupts from group 4
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP4;
}


/****************************************************************************/
/*                                                                          */
/*              ����ң��ɨ��                                                */
/*                                                                          */
/****************************************************************************/
unsigned char Remote_Scan(void)
{
	unsigned char sta = 0;
    unsigned char t1,t2;
	if(RmtSta & (1<<6))//�õ�һ��������������Ϣ��
	{
	    t1 = RmtRec >> 24;			//�õ���ַ��
	    t2 = (RmtRec >> 16) & 0xff;	//�õ���ַ����

	    t1 = t1 & 0xff;
	    t2 = (~t2) & 0xff;
 	    if(t1 == t2 && t1 == REMOTE_ID)//����ң��ʶ����(ID)����ַ
	    {
	        t1 = RmtRec >> 8;
	        t2 = RmtRec;

	        t1 = t1 & 0xff;
			t2 = (~t2) & 0xff;
	        if(t1 == t2) sta = t1;//��ֵ��ȷ
		}
		if((sta==0) || ((RmtSta&0X80)==0))//�������ݴ���/ң���Ѿ�û�а�����
		{
		 	RmtSta &= ~(1 << 6);//������յ���Ч������ʶ
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
