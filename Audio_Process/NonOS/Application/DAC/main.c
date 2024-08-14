/*
 * 			���ݴ������ӿƼ����޹�˾
 *
 * Copyright 2015 Tronlong All rights reserved
 */

/*
 * ����������
 *     ������ͨ��SPI����DAоƬ(TLC5615)��DA�������Ϊ10-bit�������ѹ��Χ0~5v��
 * �����������ѹΪ3.3V��
 *
 */

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

void WriteDAC(unsigned int data);
void delay(unsigned int t);
void spi_xmit(Uint16 a);
void spi_fifo_init(void);
void spi_init(void);

void main(void)
{   
	/*��ʼ��ϵͳ*/
	InitSysCtrl();

	/*��ʼ��GPIO;*/
	InitSpiaGpio();

	///��ʼ��IO��
	
	EALLOW;
	// ����SN74HC138
	GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 0; // ����GPIO11ΪGPIO��
	GpioCtrlRegs.GPADIR.bit.GPIO11 = 1;      // ����GPIO17�������
	GpioCtrlRegs.GPAPUD.bit.GPIO11 = 0;      // ��ֹ���� GPIO17����
	GpioDataRegs.GPADAT.bit.GPIO11 = 0;      // ����͵�ƽ
	EDIS;
	
	/* ���ж� */
	DINT;
	IER = 0x0000;
	IFR = 0x0000;
	
	/* ��ʼ��PIE���ƼĴ��� */
	InitPieCtrl();
	
	/* ��ʼ��PIE������ */
	InitPieVectTable();	
	
	SCIStdioInit();
    SCIPuts("\r\n ============Test Start===========.\r\n", -1);
    SCIPuts("Welcome to TL28335 DAC(TLC5615) Demo application.\r\n\r\n", -1);

	///��ʼ��SPI
    spi_init();		  // ��ʼ��SPI

    SCIPuts("The DAC output voltage: 3.3v\r\n\r\n", -1);
    WriteDAC(0x2a3);  //���3.3v��ѹ������������� = �����ѹ*1024/5

    SCIPuts("\r\n ============Test End===========.\r\n", -1);
    while(1)
    {

	}
} 	

	
void WriteDAC(unsigned int data)
{
     data = data << 6;
     //ע���������Ч������12λ��SPI��ʼ����Ҳ�����˶���
 
    while(SpiaRegs.SPISTS.bit.BUFFULL_FLAG ==1);			//�ж�SPI�ķ��ͻ������Ƿ��ǿյ�,����0��д����
     
    SpiaRegs.SPITXBUF = data;	//�ѷ��͵�����д��SPI���ͻ�����

    while( SpiaRegs.SPISTS.bit.BUFFULL_FLAG==1);		//�����ͻ�������������־λʱ,��ʼ��������
    	 
    DELAY_US(20);

}

void delay(unsigned int t)
{
 
 	while(t>0)
    	t--;
}
//��ʼ��SPI����
void spi_init()
{    
	SpiaRegs.SPICCR.all =0x00;///�����ʼ״̬��������������������Բ��ֹ��12λ����ģʽ
	                            
	SpiaRegs.SPICTL.all =0x0006; // ʹ������ģʽ��������λ��ʹ���������ͣ���ֹ����
	                            //����жϣ���ֹSPI�жϣ�
                                   
	SpiaRegs.SPIBRR =0x0031;	//SPI������=37.5M/50	=0.75MHZ��							
    SpiaRegs.SPICCR.all =0x8b | 1<<6; //�˳���ʼ״̬���������½���������Բ��ֹ��12λ����ģʽ
    SpiaRegs.SPIPRI.bit.FREE = 1;  // ��������
}






//===========================================================================
// No more.
//===========================================================================
