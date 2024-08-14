/*
 * 			���ݴ������ӿƼ����޹�˾
 *
 * Copyright 2016 Tronlong All rights reserved
 *
 *
 */

#ifndef __DHT11_H
#define __DHT11_H 

//IO��������
#define DHT11_IO_IN()  {EALLOW; GpioCtrlRegs.GPADIR.bit.GPIO0 = 0; EDIS; }   // input
#define DHT11_IO_OUT() {EALLOW; GpioCtrlRegs.GPADIR.bit.GPIO0 = 1; EDIS; }   // output

////IO��������											   
#define	DHT11_DQ_OUT0 GpioDataRegs.GPACLEAR.bit.GPIO0=1 //���ݶ˿�
#define	DHT11_DQ_OUT1 GpioDataRegs.GPASET.bit.GPIO0=1 //���ݶ˿�
#define	DHT11_DQ_IN   GpioDataRegs.GPADAT.bit.GPIO0   //���ݶ˿�

unsigned char DHT11_Init(void);//��ʼ��DHT11
unsigned char DHT11_Read_Data(unsigned char *temp,unsigned char *humi);//��ȡ��ʪ��
unsigned char DHT11_Read_Byte(void);//����һ���ֽ�
unsigned char DHT11_Read_Bit(void);//����һ��λ
unsigned char DHT11_Check(void);//����Ƿ����DHT11
void DHT11_Rst(void);//��λDHT11    
#endif















