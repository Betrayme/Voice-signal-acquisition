/*
 * 			���ݴ������ӿƼ����޹�˾
 *
 * Copyright 2015 Tronlong All rights reserved
 */
/*
 *     ����������
 *     DSP��FPGAͨ�Ų���(XINTF) ��FPGAд�����飬�ٶ�������
 *     ���������Ƿ�һ��
 */

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File
void  InitXintf(void);

#define EMIF_BUFFER_LENGTH 100

unsigned int ErrorCount;
unsigned int t_buffer[1000];
unsigned int r_buffer[1000];

// XZCS6
Uint16    *ExRamStart = (Uint16 *)0x100000;

void main(void)
{
   Uint32	i;

   //��ʼ��ϵͳ����
   InitSysCtrl();

   //��ʼ��XINTF�ܽ�
   InitXintf();

   EALLOW;

   //���á�ʹ�ܹܽ�
   //����Ƭѡ����ֹ���߳�ͻ
   GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 0;
   GpioCtrlRegs.GPADIR.bit.GPIO28 = 1;
   GpioDataRegs.GPASET.bit.GPIO28 = 1; //��ʹ��

   GpioCtrlRegs.GPBMUX1.bit.GPIO36 = 0;
   GpioCtrlRegs.GPBDIR.bit.GPIO36 = 1;
   GpioDataRegs.GPBSET.bit.GPIO36 = 1; //��ʹ��

   GpioCtrlRegs.GPBMUX1.bit.GPIO37 = 0;
   GpioCtrlRegs.GPBDIR.bit.GPIO37 = 1;
   GpioDataRegs.GPBSET.bit.GPIO37 = 1; //��ʹ��
   EDIS;

   DINT;

   //��ʼ���ж�����
   InitPieCtrl();

   //��ֹ�жϺ�����жϱ�־
   IER = 0x0000;
   IFR = 0x0000;

   //��ʼ�����ݻ���
   for(i = 0; i < EMIF_BUFFER_LENGTH; i++)
   {
       t_buffer[i] = i;
       r_buffer[i] = 0xDEAD;
   }

   SCIStdioInit();
   SCIPuts("DSP >> FPGA testing..\r\n\r\n", -1);

   SCIPuts("Write data to FPGA.\r\n", -1);

   for(i = 0; i< EMIF_BUFFER_LENGTH; i++)
   {
      *(ExRamStart + i) =i;
   }

   SCIPuts("Verify the data.\r\n", -1);

   ErrorCount = 0;

   for (i = 0; i < EMIF_BUFFER_LENGTH; i++)
   {
       r_buffer[i] = *(ExRamStart + i);
   }

   for(i = 0; i < EMIF_BUFFER_LENGTH; i++)
   {
       if( r_buffer[i] != t_buffer[i] )
       {
           ErrorCount++;
       }
   }

   if(ErrorCount == 0)
       SCIPuts("Test succeed.\r\n", -1);
   else
	   SCIprintf("Test failed with %u errors.\r\n", ErrorCount);

   SCIPuts("\r\n ============Test End===========.\r\n", -1);

   while(1);
}

//===========================================================================
// No more.
//===========================================================================
