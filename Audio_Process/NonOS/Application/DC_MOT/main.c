/*
 * 			���ݴ������ӿƼ����޹�˾
 *
 * Copyright 2015 Tronlong All rights reserved
 */

/*
 * ����������
 *     ������ͨ�� PWM ����ֱ�������ͨ���������ڵ���ٶȺͷ���KEY1���٣�
 * KEY2���٣�KEY3����
 *
 */

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

// ʹ��ǰ���������ļ��е���غ�����
void InitEPwm1Example(void);
interrupt void epwm1_isr(void);
void Scan_Key(void);
void Manage_Up(void);
void Manage_Down(void);
void Motor_Drive(void);
void Init_Port(void);


/***************ȫ�ֱ�������****************/
Uint16 temp=0;  //�ߵ�ƽʱ�� 
Uint16 Direction=0;//ת�ٷ���


// �궨��ÿ����ʱ�����ڼĴ���������ֵ��
#define EPWM1_TIMER_TBPRD  3750  // ����ֵ
#define EPWM1_MAX_CMPA     3700
#define EPWM1_MIN_CMPA       0
#define EPWM1_MAX_CMPB     3700
#define EPWM1_MIN_CMPB       0
/****************�˿ں궨��*****************/
#define Up_Key GpioDataRegs.GPADAT.bit.GPIO24
#define Down_Key GpioDataRegs.GPADAT.bit.GPIO25
#define Direction_Key GpioDataRegs.GPADAT.bit.GPIO26

void main(void)
{
// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the DSP2833x_SysCtrl.c file.
   InitSysCtrl();

// Step 2. Initalize GPIO:
// This example function is found in the DSP2833x_Gpio.c file and
// illustrates how to set the GPIO to it's default state.
// InitGpio();  // Skipped for this example

// For this case just init GPIO pins for ePWM1, ePWM2, ePWM3
// These functions are in the DSP2833x_EPwm.c file
   InitEPwm1Gpio();
   Init_Port();

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
   SCIPuts("Welcome to TL28335 DC MOTOR Demo application.\r\n\r\n", -1);

// Interrupts that are used in this example are re-mapped to
// ISR functions found within this file.
   EALLOW;  // This is needed to write to EALLOW protected registers
   PieVectTable.EPWM1_INT = &epwm1_isr;
   EDIS;    // This is needed to disable write to EALLOW protected registers

// Step 4. Initialize all the Device Peripherals:
// This function is found in DSP2833x_InitPeripherals.c
// InitPeripherals();  // Not required for this example

// For this example, only initialize the ePWM

   EALLOW;
   SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
   EDIS;

   InitEPwm1Example();

   EALLOW;
   SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
   EDIS;

// Step 5. User specific code, enable interrupts:

// Enable CPU INT3 which is connected to EPWM1-3 INT:
   IER |= M_INT3;

// Enable EPWM INTn in the PIE: Group 3 interrupt 1-3
   PieCtrlRegs.PIEIER3.bit.INTx1 = 1;

// Enable global Interrupts and higher priority real-time debug events:
   EINT;   // Enable Global interrupt INTM
   ERTM;   // Enable Global realtime interrupt DBGM

   SCIPuts("Press the KEY1 increase the speed.\r\n", -1);
   SCIPuts("Press the KEY2 reduce the speed.\r\n", -1);
   SCIPuts("Press the KEY3 change direction\r\n", -1);

// Step 6. IDLE loop. Just sit and loop forever (optional):
   for(;;)
   {
       asm("          NOP");
   }

}

interrupt void epwm1_isr(void)
{
     if((Up_Key==0)|(Down_Key==0)|(Direction_Key==0))//ɨ���Ƿ񰴼�����
   {
	  Scan_Key();	
   // ����CMPA��CMPB�ȽϼĴ�����ֵ
      Motor_Drive();
   }
   
  
   // ��������ʱ�����жϱ�־λ
   EPwm1Regs.ETCLR.bit.INT = 1;

   // ���PIEӦ��Ĵ����ĵ���λ������Ӧ��3�ڵ������ж�����
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}



void InitEPwm1Example()
{

  // ����ʱ���׼��ʱ���źţ�TBCLK��
   EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP; // ��������ģʽ
   EPwm1Regs.TBPRD = EPWM1_TIMER_TBPRD;       // ���ö�ʱ������
   EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;    // ��ֹ��λ����
   EPwm1Regs.TBPHS.half.TBPHS = 0x0000;       // ʱ����λ�Ĵ�����ֵ��ֵ0
   EPwm1Regs.TBCTR = 0x0000;                   // ʱ������������
   EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV2;   // ����ʱ��ʱ������Ϊϵͳʱ��SYSCLKOUT/4=37.5MHZ;
   EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV2;//��ʱ��ʱ��Ƶ�ʺ�ʱ�����ڿ�֪PWM1Ƶ��=10KHZ��

   // ���ñȽϼĴ�������Ӱ�Ĵ�������������ʱ��������0
   EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
   EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
   EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
   EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;


   // ���ñȽϼĴ�����ֵ
   EPwm1Regs.CMPA.half.CMPA = EPWM1_MIN_CMPA;     // ���ñȽϼĴ���A��ֵ
   EPwm1Regs.CMPB = EPWM1_MIN_CMPB;               // ���ñȽϼĴ���B��ֵ

   // ���ö����޶�������Ĭ��Ϊת������Ϊ��ת����ʱֻ��PWM1A���ռ�ձȣ�
   EPwm1Regs.AQCTLA.bit.ZRO = AQ_SET;            // ������0ʱPWM1A����ߵ�ƽ
   EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;          // ��������ʱ�������ȽϼĴ���Aƥ��ʱ���PWM1A���

   EPwm1Regs.AQCTLB.bit.ZRO = AQ_CLEAR;            // ������0ʱPWM1B����͵�ƽ
   EPwm1Regs.AQCTLB.bit.CBU = AQ_CLEAR;          // ��������ʱ�������ȽϼĴ���Aƥ��ʱ���PWM1B���
   
   // 3��0ƥ���¼�����ʱ����һ���ж�����һ��ƥ����100us��һ��300us����һ���жϣ�
   EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // ѡ��0ƥ���¼��ж�
   EPwm1Regs.ETSEL.bit.INTEN = 1;                // ʹ���¼������ж�
   EPwm1Regs.ETPS.bit.INTPRD = ET_3RD;           // 3���¼������ж�����  


}
/*------------------------------------------*/
/*��ʽ������void                            */
/*����ֵ:void				                */
/*��������:��ʼ��������ƶ˿�               */
/*------------------------------------------*/ 
void Init_Port(void)
{
	EALLOW;   
// �û�������Ҫ����
// ��������Ҫ����ע�͵���
    GpioCtrlRegs.GPAPUD.bit.GPIO24 = 1;    // ��ֹGPIO24 �����ڲ�����
    GpioCtrlRegs.GPAPUD.bit.GPIO25 = 1;    // ��ֹGPIO25 �����ڲ�����
    GpioCtrlRegs.GPAPUD.bit.GPIO26 = 1;    // ��ֹGPIO26 �����ڲ�����
   
/* ͨ��I/O����ѡ��Ĵ�������ͨ��I/O����*/
    GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 0;   // ����GPIO24Ϊͨ��I/O��
    GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 0;   // ����GPIO25Ϊͨ��I/O��
	GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 0;   // ����GPIO26Ϊͨ��I/O��

	//��������ȫ������   
   GpioDataRegs.GPADAT.bit.GPIO24 = 0x0001;
   GpioDataRegs.GPADAT.bit.GPIO25 = 0x0001;
   GpioDataRegs.GPADAT.bit.GPIO26 = 0x0001;

/* ����I/O��Ϊ��������*/
    GpioCtrlRegs.GPADIR.bit.GPIO24 = 0;   // ����GPIO24Ϊ��������
    GpioCtrlRegs.GPADIR.bit.GPIO25 = 0;   // ����GPIO25Ϊ��������
    GpioCtrlRegs.GPADIR.bit.GPIO26 = 0;   // ����GPIO26Ϊ��������
// ÿ������ڿ����в�ͬ�������޶�
   // a) ������ϵͳʱ�� SYSCLKOUTͬ��
   // b) ���뱻ָ���Ĳ��������޶�
   // c) �����첽 (��������������Ч)
   GpioCtrlRegs.GPAQSEL2.all = 0x0000;    // GPIO16-GPIO31��ϵͳʱ��SYSCLKOUT ͬ��
    EDIS;
} 
/*------------------------------------------*/
/*��ʽ������void		            		*/
/*����ֵ:void				    			*/
/*��������:ɨ���ֵ             	    	*/
/*------------------------------------------*/
void Scan_Key(void)      
{
	unsigned int i;
	for(i=0;i<50000;i++);    //����������
	if(Direction_Key==0)//ɨ�跽�򰴼��Ƿ���
	{
//��֤����EPWMA��EPWMB�໥�л�ͬʱ���0��ƽ��
	EPwm1Regs.CMPA.half.CMPA = 0;//�ı�����
	EPwm1Regs.CMPB = 0;//�ı�����
       if(Direction==0)
       {  
   // ���ö����޶�������Ĭ��Ϊת������Ϊ��ת����ʱֻ��PWM1B���ռ�ձȣ�
       EPwm1Regs.AQCTLA.bit.ZRO = AQ_CLEAR;            // ������0ʱPWM1A����͵�ƽ
       EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;          // ��������ʱ�������ȽϼĴ���Aƥ��ʱ���PWM1A���

       EPwm1Regs.AQCTLB.bit.ZRO = AQ_SET;            // ������0ʱPWM1B����ߵ�ƽ
       EPwm1Regs.AQCTLB.bit.CBU = AQ_CLEAR;          // ��������ʱ�������ȽϼĴ���Aƥ��ʱ���PWM1B���
	   Direction=1;
	   }
       else
	   {
        // ���ö����޶�������Ĭ��Ϊת������Ϊ��ת����ʱֻ��PWM1A���ռ�ձȣ�
       EPwm1Regs.AQCTLA.bit.ZRO = AQ_SET;            // ������0ʱPWM1A����ߵ�ƽ
       EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;          // ��������ʱ�������ȽϼĴ���Aƥ��ʱ���PWM1A���

       EPwm1Regs.AQCTLB.bit.ZRO = AQ_CLEAR;            // ������0ʱPWM1B����͵�ƽ
       EPwm1Regs.AQCTLB.bit.CBU = AQ_CLEAR;          // ��������ʱ�������ȽϼĴ���Aƥ��ʱ���PWM1B���
	   Direction=0;
	   }
    temp=0;
	}
	else
	{	
	if(Up_Key==0)
		Manage_Up();
	else if(Down_Key==0)
		Manage_Down();
	}			  
	while((Up_Key==0)|(Down_Key==0)|(Direction_Key==0));
}

/*------------------------------------------*/
/*��ʽ������void		            		*/
/*����ֵ:void				    			*/
/*��������:�����Ĵ������    	    		*/
/*------------------------------------------*/
void Manage_Up(void) //��
{
	if(temp!=3500)
		temp=temp+500;
}

void Manage_Down(void)//��
{
	if(temp!=0)
		temp=temp-500;
} 
/*------------------------------------------*/
/*��ʽ������void		            		*/
/*����ֵ:void				    			*/
/*��������:�����������    	    			*/
/*------------------------------------------*/
void Motor_Drive(void) 
{
	EPwm1Regs.CMPA.half.CMPA = temp;//�ı�����
	EPwm1Regs.CMPB = temp;//�ı�����
}




//===========================================================================
// No more.
//===========================================================================
