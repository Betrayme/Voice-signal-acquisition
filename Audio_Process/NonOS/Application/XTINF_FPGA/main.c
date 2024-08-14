/*
 * 			广州创龙电子科技有限公司
 *
 * Copyright 2015 Tronlong All rights reserved
 */
/*
 *     功能描述：
 *     DSP与FPGA通信测试(XINTF) 向FPGA写入数组，再读回来。
 *     检验数据是否一致
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

   //初始化系统控制
   InitSysCtrl();

   //初始化XINTF管脚
   InitXintf();

   EALLOW;

   //配置、使能管脚
   //禁用片选，防止总线冲突
   GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 0;
   GpioCtrlRegs.GPADIR.bit.GPIO28 = 1;
   GpioDataRegs.GPASET.bit.GPIO28 = 1; //不使能

   GpioCtrlRegs.GPBMUX1.bit.GPIO36 = 0;
   GpioCtrlRegs.GPBDIR.bit.GPIO36 = 1;
   GpioDataRegs.GPBSET.bit.GPIO36 = 1; //不使能

   GpioCtrlRegs.GPBMUX1.bit.GPIO37 = 0;
   GpioCtrlRegs.GPBDIR.bit.GPIO37 = 1;
   GpioDataRegs.GPBSET.bit.GPIO37 = 1; //不使能
   EDIS;

   DINT;

   //初始化中断向量
   InitPieCtrl();

   //禁止中断和清楚中断标志
   IER = 0x0000;
   IFR = 0x0000;

   //初始化数据缓冲
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
