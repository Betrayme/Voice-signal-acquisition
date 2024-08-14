/*
 * 			���ݴ������ӿƼ����޹�˾
 *
 * Copyright 2016 Tronlong All rights reserved
 */

/*
 * ����������
 *     ������ʹ��ʹ�� GPIO26 ������������ʪ�ȴ������������ĵ���ʪ��ֵͨ������2��ӡ��
 *
 */

#include "DSP2833x_Device.h"     // DSP2823x Headerfile Include File
#include "DSP2833x_examples.h"   // DSP2823x Examples Include File
#include "dht11.h"

void main(void)
{
	unsigned char temperature = 0;
	unsigned char humidity = 0;

	//ϵͳ��ʼ��
	InitSysCtrl();

	MemCopy(&RamfuncsLoadStart,&RamfuncsLoadEnd,&RamfuncsRunStart);
	InitFlash();

	DINT;

	//��ʼ������
	InitPieCtrl();

	IER = 0x0000;
	IFR = 0x0000;

	InitPieVectTable();

	SCIStdioInit();
	SCIPuts("\r\n ============Test Start===========.\r\n", -1);
	SCIPuts("Welcome to TL28335 DHT11 Demo application.\r\n\r\n", -1);

	//��ʼ����ʪ�ȴ�����
  	DHT11_Init();

  	// ��ѭ��
  	while(1)
  	{
  		// ��ʱ
		DELAY_US(500000);

  		DHT11_Read_Data(&temperature,&humidity);		//��ȡ��ʪ��ֵ
  		SCIprintf("temperature=%d ��C, humidity=%d %%\n",temperature,humidity);
  	}
} 	

