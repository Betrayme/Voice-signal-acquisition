/*
 * 			广州创龙电子科技有限公司
 *
 * Copyright 2015 Tronlong All rights reserved
 */

/*
 * 功能描述：
 *     NOR Flash(SST39VF800)读写测试，向Flash写入0x400个字节在读回来，校验数据
 * 是否正确，结果通过串口b打印。
 *
 */

#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
/**************************************************************************/

#define 	SST_ID                   0x00BF         /* SST Manufacturer's ID code   */
#define 	SST_39VF400A             0x2780         /* SST39VF400/SST39VF400A device code */
#define 	SST_39VF800A             0x2781         /* SST39VF800/SST39VF800A device code */
#define		TimeOutErr				 1
#define		VerifyErr				 2
#define		WriteOK					 0
#define		EraseErr				 3
#define		EraseOK					 0			
#define		SectorSize				 0x800
#define		BlockSize				 0x8000
unsigned    int  *FlashStart = (unsigned  int *)0x100000;

#pragma DATA_SECTION(WriteBuf, "DMARAML7")
#pragma DATA_SECTION(ReadBuf, "DMARAML7")
Uint16 WriteBuf[0x400],ReadBuf[0x400];
/**************************************************************************/

void delay_loop(Uint32 x);
extern void InitXintf(void);

#define  execute_30us    delay_loop(1000)    
#define  execute_150ms    delay_loop(5000000)
/******************************************************************************************
* 函数名称    ：delay_loop
******************************************************************************************/
void delay_loop(Uint32 x)
{
    Uint32      i;
    for (i = 0; i < x; i++) {}
}

/**************************************************************************/
Uint16	ChipErase(void)
{
	Uint16	i,Data;
	Uint32	TimeOut;
	*(FlashStart + 0x5555) = 0xAAAA;
	*(FlashStart + 0x2AAA) = 0x5555;
	*(FlashStart + 0x5555) = 0x8080;
	*(FlashStart + 0x5555) = 0xAAAA;
	*(FlashStart + 0x2AAA) = 0x5555;
	*(FlashStart + 0x5555) = 0x1010;
	i = 0;
	TimeOut = 0;
	execute_150ms;
	while(i<5)
	{
		Data = *(FlashStart +  0x3FFFF);
		if	(Data == 0xFFFF)	i++;
		else	i=0;
		if ( ++TimeOut>0x1000000)	return (TimeOutErr);
	}
	for	(i=0;i<0x4000;i++)
	{
		Data = *(FlashStart + i);
		if (Data !=0xFFFF)	return (EraseErr);
	}

	return  (EraseOK);

}
		

Uint16	FlashWrite(Uint32   RomStartAddr,Uint16 *pWriteBuf,  Uint16	Length)
{
	Uint32	i,TimeOut;
	Uint16	Data1,Data2,j;
	for	(i=0;i<Length;i++)
	{
		*(FlashStart + 0x5555)= 0xAA;
		*(FlashStart + 0x2AAA)= 0x55;
		*(FlashStart + 0x5555) = 0xA0;
		*(FlashStart + RomStartAddr + i) = *(pWriteBuf + i);
		TimeOut = 0;
		j=0;
		execute_30us;
	}

	while(j<5)
	{
		Data1 = *(FlashStart + RomStartAddr + i);
		Data2 = *(FlashStart + RomStartAddr + i);
		if	(Data1 == Data2)	j++;
		else	j=0;
		if ( ++TimeOut>0x1000000)	return (TimeOutErr);
	}


	for (i=0;i<Length;i++)
	{
		Data1 = *(FlashStart + RomStartAddr +i);
		Data2 = *(pWriteBuf +i);
		if  (Data1 != Data2)	return (VerifyErr);
	}
	return  (WriteOK);
}

void	FlashRead(Uint32   RomStartAddr,Uint16 *pReadBuf,	Uint16	Length)
{
	Uint32	i;
	Uint16 Temp;
	for	(i=0;i<Length;i++)
	{
		Temp =  (*((volatile	unsigned  int  *)(FlashStart + RomStartAddr + i)));
		*(pReadBuf +i) = Temp;
	}
}

/**************************************************************************/
void main(void)
{
   Uint16 i,errors=0;
// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the DSP2833x_SysCtrl.c file.
   InitSysCtrl();

// Step 2. Clear all interrupts and initialize PIE vector table:
// Disable CPU interrupts
   DINT;

// Step 3. Initialize the PIE control registers to their default state.
// The default state is all PIE interrupts disabled and flags
// are cleared.
// This function is found in the DSP2833x_PieCtrl.c file.
   InitPieCtrl();

// Step 4.Disable CPU interrupts and clear all CPU interrupt flags:
   IER = 0x0000;
   IFR = 0x0000;
//Step 5. Initialize PieVectTable
   InitPieVectTable();

   SCIStdioInit();
   SCIPuts("\r\n ============Test Start===========.\r\n", -1);
   SCIPuts("Welcome to TL28335 Nor flash(SST39VF800) Demo application.\r\n\r\n", -1);

//Step 6. Initialize Xintf
   InitXintf();	
//Step 7. 初始化DSP内部Ram区    
   for	(i=0;i<0x400;i++)
   {
	   WriteBuf[i] = i;
	   ReadBuf[i] = 0;
   }
//Step 8. 清除DSP外扩FLASH（SST39VF800）内容
   SCIPuts("Nor flash(SST39VF800) chip erase.\r\n", -1);
   ChipErase(); 
   execute_150ms;   
//Step 9. 写数据到SST39VF800    
   SCIPuts("Filling the data to the flash:  0x0000 ~ 0x03FFF.\r\n", -1);
   FlashWrite(0,WriteBuf,0x400);

//Step 11. 将FLASH中的数据再读取到另一块Ram（RAML7的后半部分）中 
   SCIPuts("Reading back the data from the flash.\r\n", -1);
   FlashRead(0,ReadBuf,0x400);

   SCIPuts("Compare the readback data to the written data.\r\n", -1);
   for(i=0;i<0x400;i++)
   {
	   if(WriteBuf[i] != ReadBuf[i])
		   errors++;
   }

   if(errors)
	   SCIprintf("Nor flash testing failed with %d errors.\r\n", errors);
   else
	   SCIprintf("Nor flash testing successfully.\r\n", errors);

   SCIPuts("\r\n ============Test End===========.\r\n", -1);
   while(1);
}




