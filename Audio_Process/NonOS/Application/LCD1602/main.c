/*
 * 			���ݴ������ӿƼ����޹�˾
 *
 * Copyright 2015 Tronlong All rights reserved
 */

/*
 * ����������
 *     ������ʹ��GPIO����LCD1602,���г���������Ļ����ʾ��Welcome��
 * "www.tronlong.com"
 *
 */

#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

/**************************************************************************/


/****************�˿ں궨��*****************/
#define LCD_RS GpioDataRegs.GPBDAT.bit.GPIO34
#define LCD_RW GpioDataRegs.GPADAT.bit.GPIO8
#define LCD_EN GpioDataRegs.GPADAT.bit.GPIO9
#define LCD_DB GpioDataRegs.GPCDAT.all

void LCD_init(void);                          //��ʼ������
void LCD_write_command(unsigned char command);        //дָ���
void LCD_write_data(unsigned char dat);               //д���ݺ���
void show(char *cont);                       //��ʾ�Ӻ���
void Init_Port(void);   //�˿ڳ�ʼ��


/**************************************************************************/
void main(void)
{
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
   SCIPuts("Welcome to TL28335 LCD1602 Demo application.\r\n\r\n", -1);

//Step 6. Initialize GPIO
    Init_Port();

	LCD_init();

	LCD_write_command(0x80);	//��һ������ָ���ַ
	show("    Welcome");

	LCD_write_command(0xc0);	//�ڶ�������ָ��ĵ�ַ
	show("www.tronlong.com");

	SCIPuts("\r\n ============Test End===========.\r\n", -1);
    while(1)
    {

    }
}


void Init_Port(void)
{
	EALLOW;
	GpioCtrlRegs.GPCMUX1.all = 0;
	GpioCtrlRegs.GPCDIR.all = 0XFFFFFFFF;
	// RW
	GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 0;  // GPIO8
	GpioCtrlRegs.GPADIR.bit.GPIO8 = 1;   // output
	GpioDataRegs.GPACLEAR.bit.GPIO8 = 1;
	// EN
	GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 0;  // GPIO9
	GpioCtrlRegs.GPADIR.bit.GPIO9 = 1;   // output
	GpioDataRegs.GPACLEAR.bit.GPIO9 = 1;
	// RS
	GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 0;  // GPIO34
	GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1;   // output
	GpioDataRegs.GPBCLEAR.bit.GPIO34 = 1;
	EDIS;
}

/*--------------------------------------
;ģ������:show(char *cont)
;��    ��:LCD1602��ʾ�Ӻ���
;����˵��:contΪ��ʾ�ַ���ָ�����
;-------------------------------------*/
void show(char *cont)
{
	while(*cont!=0)
	{
		LCD_write_data(*cont);
		DELAY_US(500);  //��ʱ500us
		cont++;
	}
}

/*--------------------------------------
;ģ������:LCD_write_command();
;��    ��:LCD1602дָ���
;����˵��:commandΪд�������
;-------------------------------------*/
void LCD_write_command(unsigned char command)
{
	unsigned char i;
	Uint32 temp = 0;

	for(i=0;i<8;i++)
	{
		if(command & 0x01)
			temp |= 0x01;

		if(i == 7)
			break;

		temp = temp << 1;
		command >>= 1;
	}
	temp = temp << 8;

	LCD_EN=0;
	LCD_RS=0;         //ָ��
	LCD_RW=0;         //д��
	LCD_DB=temp;//��ֵ��D0-D7��Ӧ��GPIO79-GPIO72����
	DELAY_US(500);
	LCD_EN=1;          //����
	DELAY_US(1000);  //
	LCD_EN=0;
}

/*--------------------------------------
;ģ������:LCD_write_data();
;��    ��:LCD1602д���ݺ���
;����˵��:datΪд���ݲ���
;-------------------------------------*/
void LCD_write_data(unsigned char dat)
{
	unsigned char i;
	Uint32 temp = 0;

	for(i=0;i<8;i++)
	{
		if(dat & 0x01)
			temp |= 0x01;

		if(i == 7)
			break;

		temp = temp << 1;
		dat >>= 1;
	}

	temp = temp << 8;
	LCD_EN=0;
	LCD_RS=1;          //����
	LCD_RW=0;          //д��
	LCD_DB=temp;//��ֵ��D0-D7��Ӧ��GPIO79-GPIO72����
	DELAY_US(500);
	LCD_EN=1;           //����
	DELAY_US(1000);
	LCD_EN=0;
}
/*--------------------------------------
;ģ������:LCD_init();
;��    ��:��ʼ��LCD1602
;ռ����Դ:--
;����˵��:--
;-------------------------------------*/
void LCD_init(void)
{
	DELAY_US(15000);//�ӳ�15ms
	LCD_write_command(0x38);//����8λ��ʽ��2�У�5x7
	DELAY_US(5000);//�ӳ�5ms
	LCD_write_command(0x38);//����8λ��ʽ��2�У�5x7
	DELAY_US(5000);
	LCD_write_command(0x38);//����8λ��ʽ��2�У�5x7
	LCD_write_command(0x38);//����8λ��ʽ��2�У�5x7
	LCD_write_command(0x08);//����ʾ������ʾ��꣬��겻��˸��
	LCD_write_command(0x01);//�����Ļ��ʾ������ָ�����㣬������ʾ���㣻
	LCD_write_command(0x06);//�趨���뷽ʽ����������λ
	LCD_write_command(0x0c);//������ʾ���ع�꣬����˸
}


