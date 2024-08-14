/*
 * 			���ݴ������ӿƼ����޹�˾
 *
 * Copyright 2015 Tronlong All rights reserved
 */

/*
 * ����������
 *     ������ʹ���ⲿ�жϼ��KEY1��KEY2����KEY1����ʱLED1��ƽ��ת����KEY2����ʱ
 * LED2��ƽ��ת��
 */

#include "DSP2833x_Device.h"     // Headerfile Include File
#include "DSP2833x_Examples.h"   // Examples Include File

// �жϷ����ӳ���������������ʱ����Ҫ��������֮ǰҪ����������ʹ��.
interrupt void xint1_isr(void);
interrupt void xint2_isr(void);

// �������õ���ȫ�ֱ�����
volatile Uint32 Xint1Count; //�����ⲿ�ж�1�Ĵ�����
volatile Uint32 Xint2Count; //�����ⲿ�ж�2�Ĵ�����
volatile Uint32 Xint2Loop; //���ⲿ�ж�2��forѭ������

#define keyon1 GpioDataRegs.GPADAT.bit.GPIO24
#define keyon2 GpioDataRegs.GPADAT.bit.GPIO25

#define DELAY 35.700L

void main(void)
{
// ���� 1. ��ʼ��ϵͳ����:
// ����PLL, WatchDog, ʹ������ʱ��
// ��������������Դ�DSP280x_SysCtrl.c�ļ����ҵ�.
   InitSysCtrl();
   
// ���� 2. ��ʼ��ͨ�����������·������GPIO: 
// ���������DSP280x_Gpio.cԴ�ļ��б�������
// �������ʹGPIO������Ĵ�����ʼ����Ĭ��״̬
// InitGpio();  // Skipped for this example  

// ���� 3. ��������ж�,��ʼ���ж�������:
// ��ֹCPUȫ���ж�
   DINT;

// ��ʼ��PIE���ƼĴ��������ǵ�Ĭ��״̬.
// ���Ĭ��״̬���ǽ�ֹPIE�жϼ��������PIE�жϱ�־ 
// �����������DSP280x_PieCtrl.cԴ�ļ���
   InitPieCtrl();

// ��ֹCPU�жϺ��������CPU�жϱ�־ 
   IER = 0x0000;
   IFR = 0x0000;

//��ʼ��PIE�ж���������ʹ��ָ���жϷ����ӳ���ISR��
// ��Щ�жϷ����ӳ��򱻷�����DSP280x_DefaultIsr.cԴ�ļ���
// �������������DSP280x_PieVect.cԴ�ļ�����.
   InitPieVectTable();

    EALLOW;
	GpioCtrlRegs.GPBMUX2.bit.GPIO50 = 0;  // GPIO50
	GpioCtrlRegs.GPBMUX2.bit.GPIO51 = 0;  // GPIO51
	GpioCtrlRegs.GPBMUX2.bit.GPIO50 = 0;  // GPIO52
	GpioCtrlRegs.GPBMUX2.bit.GPIO51 = 0;  // GPIO53

	GpioCtrlRegs.GPBDIR.bit.GPIO50 = 1;   // output
	GpioCtrlRegs.GPBDIR.bit.GPIO51 = 1;   // output
	GpioCtrlRegs.GPBDIR.bit.GPIO52 = 1;   // output
	GpioCtrlRegs.GPBDIR.bit.GPIO53 = 1;   // output

	// GPIO50 = 0; GPIO51 = 0; GPIO52 = 0; GPIO53 = 0;
	GpioDataRegs.GPBCLEAR.all  =0x003c0000;
	EDIS;

//  �������õ����ж�����ӳ�䵽�������жϷ����ӳ��� 
   EALLOW;	// �޸ı������ļĴ������޸�ǰӦ���EALLOW���
   PieVectTable.XINT1 = &xint1_isr;
   PieVectTable.XINT2 = &xint2_isr;
   EDIS;   // EDIS����˼�ǲ������޸ı������ļĴ���
	
// ���� 4. ��ʼ��Ƭ������:
// �������������DSP280x_CpuTimers.cԴ�ļ����ҵ�
// InitPeripherals(); // �����в���Ҫ
 
// ���� 5. �û��ض��Ĵ�����������Ҫ���ж�

// ���������
   Xint1Count = 0; // �����ⲿ�ж�1��XINT1��
   Xint2Count = 0; // �����ⲿ�ж�2��XINT2�� 

// �������ж���չģ���ж�ʹ�ܼĴ���������XINT1��XINT2: ����1�ж�4 & 5��
// ʹ��CPU�ж�1��INT1��:
   PieCtrlRegs.PIECTRL.bit.ENPIE = 1;          // ʹ�������ж���չPIEģ��
   PieCtrlRegs.PIEIER1.bit.INTx4 = 1;          // ʹ��PIE��1��INT4
   PieCtrlRegs.PIEIER1.bit.INTx5 = 1;          // ʹ��PIE��1��INT5   
   IER |= M_INT1;                              // ʹ��CPU�ж�1��INT1��
   EINT;                                       // ��ȫ���ж�


   EALLOW;

  GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 0;         // GPIO
  GpioCtrlRegs.GPADIR.bit.GPIO24 = 0;          // input
  GpioCtrlRegs.GPAQSEL2.bit.GPIO24 = 0;

  GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 0;         // GPIO
  GpioCtrlRegs.GPADIR.bit.GPIO25 = 0;          // input
  GpioCtrlRegs.GPAQSEL2.bit.GPIO25 = 0;

  EDIS;


// ͨ��GPIO�ⲿ�ж�ѡ��Ĵ�����ѡ��GPIO24Ϊ�ⲿ�ж�1��ѡ��GPIO25Ϊ�ⲿ�ж�2
   EALLOW;
   GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 24;   // XINT1��GPIO24
   GpioIntRegs.GPIOXINT2SEL.bit.GPIOSEL = 25;   // XINT2��GPIO25
   EDIS;

// �����ⲿ�ж�1��2���жϿ��ƼĴ���
   XIntruptRegs.XINT1CR.bit.POLARITY = 0;      // �½��ش����ж�
   XIntruptRegs.XINT2CR.bit.POLARITY = 0;      // �½��ش����ж�
   
// ʹ���ⲿ�ж�1���ⲿ�ж�2
   XIntruptRegs.XINT1CR.bit.ENABLE = 1;        // ʹ��XINT1
   XIntruptRegs.XINT2CR.bit.ENABLE = 1;        // ʹ��XINT2
   
// ����6. ����ѭ��:	
   for(;;);
	
} 	

//����7.����������жϷ����ӳ��������Ҫʹ���жϷ����ӳ��򣬱����ڲ���5�����¶����ж�
//�������ж�Ӧ�ص�ַ
interrupt void xint1_isr(void)
{
	Uint32 i;
	for(i=0;i<1000000;i++);    //����������
	while(keyon1==0);

	GpioDataRegs.GPBTOGGLE.bit.GPIO50=1;

	Xint1Count++;
		
	// Ӧ��Ĵ�����λ1��0������Ӧͬ���������жϣ�
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

interrupt void xint2_isr(void)
{
	Uint32 i;
	for(i=0;i<1000000;i++);    //����������
	while(keyon2==0);

	GpioDataRegs.GPBTOGGLE.bit.GPIO51=1;

	Xint1Count++;
	
	// Ӧ��Ĵ�����λ1��0������Ӧͬ���������жϣ�
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

}



//===========================================================================
// No more.
//===========================================================================



