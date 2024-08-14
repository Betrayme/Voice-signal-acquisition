/*
 * 			广州创龙电子科技有限公司
 *
 * Copyright 2016 Tronlong All rights reserved
 *
 *
 */

#ifndef __DHT11_H
#define __DHT11_H 

//IO方向设置
#define DHT11_IO_IN()  {EALLOW; GpioCtrlRegs.GPADIR.bit.GPIO0 = 0; EDIS; }   // input
#define DHT11_IO_OUT() {EALLOW; GpioCtrlRegs.GPADIR.bit.GPIO0 = 1; EDIS; }   // output

////IO操作函数											   
#define	DHT11_DQ_OUT0 GpioDataRegs.GPACLEAR.bit.GPIO0=1 //数据端口
#define	DHT11_DQ_OUT1 GpioDataRegs.GPASET.bit.GPIO0=1 //数据端口
#define	DHT11_DQ_IN   GpioDataRegs.GPADAT.bit.GPIO0   //数据端口

unsigned char DHT11_Init(void);//初始化DHT11
unsigned char DHT11_Read_Data(unsigned char *temp,unsigned char *humi);//读取温湿度
unsigned char DHT11_Read_Byte(void);//读出一个字节
unsigned char DHT11_Read_Bit(void);//读出一个位
unsigned char DHT11_Check(void);//检测是否存在DHT11
void DHT11_Rst(void);//复位DHT11    
#endif















