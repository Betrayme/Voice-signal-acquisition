/*
 * 			���ݴ������ӿƼ����޹�˾
 *
 * Copyright 2015 Tronlong All rights reserved
 */

/*
 * ����������
 *     �������GPIO2ֱ��GPIO5��������ߵ�ƽ���򲽽������˳ʱ����תһ��ʱ�䣬
 * Ȼ��������GPIO5ֱ��GPIO2��������ߵ�ƽ,�򲽽������ʱ����תһ��ʱ�䡣
 *
 */

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

// �����õ����Ӻ���
void delay_loop(Uint32 a);
void Gpio_select(void);

unsigned char Step_table[]={0x0006,0x000a,0x0012,0x0022};
unsigned char Step_table1[]={0x0021,0x0011,0x0009,0x0005};

void main(void)
{
   unsigned int i,j;
// ���� 1. ��ʼ��ϵͳ����:
// ����PLL, WatchDog, ʹ������ʱ��
// ��������������Դ�DSP2833x_SysCtrl.c�ļ����ҵ�.
   InitSysCtrl();
   
// ���� 2. ��ʼ��ͨ�����������·������GPIO: 
// ���������DSP2833x_Gpio.cԴ�ļ��б�������
// �������ʹGPIO������Ĵ�����ʼ����Ĭ��״̬
// InitGpio();  // Skipped for this example
 
// For this example use the following configuration:
   Gpio_select();	  

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
 
   j=512; //�ı����ֵ���Կ���ת��������
   while(j--)
   {
      for(i=0;i<4;i++)
      {
         GpioDataRegs.GPADAT.all=Step_table[i]; //GPIO2-GPIO5��������ߵ�ƽ
         delay_loop(80000);
      }
   }
	
	
//��ת
	j=512;
	GpioDataRegs.GPADAT.all=0x0000;
	while(j--)
    {
      for(i=0;i<4;i++)
      {
         GpioDataRegs.GPADAT.all=Step_table1[i]; //GPIO5-GPIO2��������ߵ�ƽ
         delay_loop(80000);
      }
    }
	while(1);
} 	

//��ʱ����
void delay_loop(Uint32 a)
{
   while(a--);
}

//��ʼ��GPIO
void Gpio_select(void)
{
    EALLOW;
	GpioCtrlRegs.GPAMUX1.all = 0x00000000;  // All GPIO
	GpioCtrlRegs.GPAMUX2.all = 0x00000000;  // All GPIO
	GpioCtrlRegs.GPBMUX1.all = 0x00000000;  // All GPIO
    GpioCtrlRegs.GPADIR.all = 0xFFFFFFFF;   // All outputs
    GpioCtrlRegs.GPBDIR.all = 0x0000000F;   // All outputs

	 //GPIO0-GPIO31����͵�ƽ
   GpioDataRegs.GPADAT.all=0x00000000;
   EDIS;
}     
//===========================================================================
// No more.
//===========================================================================

