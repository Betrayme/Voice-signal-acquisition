/*
 * 			广州创龙电子科技有限公司
 *
 * Copyright 2016 Tronlong All rights reserved
 */

/*
 * 功能描述：
 *     本例程使用使用 GPIO26 驱动单总线温湿度传感器，读出的的温湿度值通过串口2打印。
 *
 */

#include "DSP2833x_Device.h"     // DSP2823x Headerfile Include File
#include "DSP2833x_examples.h"   // DSP2823x Examples Include File
#include "dht11.h"

void main(void)
{
	unsigned char temperature = 0;
	unsigned char humidity = 0;

	//系统初始化
	InitSysCtrl();

	MemCopy(&RamfuncsLoadStart,&RamfuncsLoadEnd,&RamfuncsRunStart);
	InitFlash();

	DINT;

	//初始化向量
	InitPieCtrl();

	IER = 0x0000;
	IFR = 0x0000;

	InitPieVectTable();

	SCIStdioInit();
	SCIPuts("\r\n ============Test Start===========.\r\n", -1);
	SCIPuts("Welcome to TL28335 DHT11 Demo application.\r\n\r\n", -1);

	//初始化温湿度传感器
  	DHT11_Init();

  	// 主循环
  	while(1)
  	{
  		// 延时
		DELAY_US(500000);

  		DHT11_Read_Data(&temperature,&humidity);		//读取温湿度值
  		SCIprintf("temperature=%d °C, humidity=%d %%\n",temperature,humidity);
  	}
} 	

