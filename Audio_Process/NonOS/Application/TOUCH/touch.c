/*
 * 			���ݴ������ӿƼ����޹�˾
 *
 * Copyright 2015 Tronlong All rights reserved
 */

#include "DSP2833x_Device.h"     // Headerfile Include File
#include "DSP2833x_Examples.h"   // Examples Include File
#include "touch.h"

#define TOUCH_AD_LEFT     86
#define TOUCH_AD_RIGHT    1968 /*A/Dת�������ص����ֵ*/
#define TOUCH_AD_TOP      180
#define TOUCH_AD_BOTTOM   1940

#define LCD_HEIGHT        272

float xscale;
float yscale;
float xoffset;
float yoffset;

char touch_flag = 0;

//��ʼ��SPI����
void spi_init()
{
	SpiaRegs.SPICCR.all =0x0007;	    //��λ, ���������������, 8 bits ����ģʽ
	SpiaRegs.SPICTL.all =0x000E;        //ʹ������ģʽ, ʱ����ʱ���ʱ������,
                                        //ʹ����������, ��ֹSPI�ж�.
	SpiaRegs.SPIBRR =0x0063;			//SPI������=37.5M/100	=0.375MHZ��
    SpiaRegs.SPICCR.all =0x0087;		//�˳���ʼ״̬��
    SpiaRegs.SPIPRI.bit.FREE = 1; 		//��������
}

// SPI ����һ���ֽ�
Uint16 spi_xmit_byte(Uint16 byte)
{
     byte = byte << 8;
     //ע���������Ч������8λ��SPI��ʼ����Ҳ�����˶���

    SpiaRegs.SPITXBUF = byte;							//�ѷ��͵�����д��SPI���ͻ�����
    while( SpiaRegs.SPISTS.bit.BUFFULL_FLAG==1);		//�����ͻ�������������־λʱ,��ʼ��������
    DELAY_US(20);

    return (SpiaRegs.SPIRXBUF << 8);
}

// ʹ�� SPI Ƭѡ
void spi_enable(void)
{
	GpioDataRegs.GPADAT.bit.GPIO11 = 1;      // ����ߵ�ƽ
	GpioDataRegs.GPBDAT.bit.GPIO57 = 0;      // ����͵�ƽ
}

// ��ֹ SPI Ƭѡ
void spi_disable(void)
{
	GpioDataRegs.GPADAT.bit.GPIO11 = 1;      // ����ߵ�ƽ
	GpioDataRegs.GPBDAT.bit.GPIO57 = 1;      // ����ߵ�ƽ
}

// ��������ʼ��
void touch_setup(void)
{
	//  �������õ����ж�����ӳ�䵽�������жϷ����ӳ���
	EALLOW;	// �޸ı������ļĴ������޸�ǰӦ���EALLOW���
	PieVectTable.XINT1 = &touch_isr;
	EDIS;   // EDIS����˼�ǲ������޸ı������ļĴ���

	// �������ж���չģ���ж�ʹ�ܼĴ���������XINT1: ����1�ж�4��
	// ʹ��CPU�ж�1��INT1��:
	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;          // ʹ�������ж���չPIEģ��
	PieCtrlRegs.PIEIER1.bit.INTx4 = 1;          // ʹ��PIE��1��INT4
	IER |= M_INT1;                              // ʹ��CPU�ж�1��INT1��
	EINT;                                       // ��ȫ���ж�

	// ����GPIO13Ϊ����,��Ϊ�ж� GPIO12Ϊ����BUSY
	EALLOW;
	GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 0;         // GPIO
	GpioCtrlRegs.GPADIR.bit.GPIO13 = 0;          // input
	GpioCtrlRegs.GPAQSEL1.bit.GPIO13 = 0;

	GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 0;         // GPIO
	GpioCtrlRegs.GPADIR.bit.GPIO12 = 0;          // input
	GpioCtrlRegs.GPAQSEL1.bit.GPIO12 = 0;
	EDIS;

	// ͨ��GPIO�ⲿ�ж�ѡ��Ĵ�����ѡ��GPIO13Ϊ�ⲿ�ж�1
	EALLOW;
	GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 13;   // XINT1��GPIO13
	EDIS;

	// �����ⲿ�ж�1���жϿ��ƼĴ���
	XIntruptRegs.XINT1CR.bit.POLARITY = 0;      // �½��ش����ж�

	// ʹ���ⲿ�ж�1
	XIntruptRegs.XINT1CR.bit.ENABLE = 1;        // ʹ��XINT1

	// ��ʼ��GPIO;
	InitSpiaGpio();

	spi_init();

	EALLOW;
	// ����SN74HC138
	GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 0; // ����GPIO11ΪGPIO��
	GpioCtrlRegs.GPADIR.bit.GPIO11 = 1;      // ����GPIO11�������
	GpioCtrlRegs.GPAPUD.bit.GPIO11 = 0;      // ��ֹ���� GPIO17����
	GpioDataRegs.GPADAT.bit.GPIO11 = 1;      // ����ߵ�ƽ

	// SN74HC138 ��ΪSPI_CS2_TOUCH
	GpioCtrlRegs.GPBMUX2.bit.GPIO57 = 0; // ����GPIO11ΪGPIO��
	GpioCtrlRegs.GPBDIR.bit.GPIO57 = 1;      // ����GPIO17�������
	GpioDataRegs.GPBDAT.bit.GPIO57 = 1;      // ����ߵ�ƽ
	EDIS;

	touch_init_xy();
}

// ����������ж�
interrupt void touch_isr(void)
{
	GpioDataRegs.GPBTOGGLE.bit.GPIO50=1;

	if (GpioDataRegs.GPADAT.bit.GPIO12==1)
	{
		touch_flag = 1;
	}

	// Ӧ��Ĵ�����λ1��0������Ӧͬ���������жϣ�
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

// ��ʼ������������
void touch_init_xy(void)
{
	xscale=(float)480.0 / (TOUCH_AD_RIGHT - TOUCH_AD_LEFT);//�õ�xscale
	xoffset=(float)(480.0 - xscale * TOUCH_AD_RIGHT);//�õ�xoff

	yscale=(float)272.0 / (TOUCH_AD_TOP - TOUCH_AD_BOTTOM);//�õ�yscale
	yoffset=(float)(272.0 - yscale * TOUCH_AD_TOP);//�õ�yoff
}

// �������
Uint16 TouchDetect(void)
{
    if(touch_flag == 0)
    	return 0;
    else if(touch_flag == 1)
    {
    	touch_flag = 0;
    	return 1;
    }
	return 0;
}

// ��ȡ AD ֵ
int Read_touch_ad(void)
{
	int result=0,re1,re2;

	re1 = spi_xmit_byte(0);
	re2 = spi_xmit_byte(0);

	result = (re1<<4) + (re2>>4);

	return result;
}

// ����ģʽ��ȡ x ����� y ����� AD ֵ
// mode = 0 ---- x����
// mode = 1 ---- y����
void tsc2046ReadAxis(char mode, int*p1)//, char*p2)
{
    unsigned int result1=0;
    unsigned char count=0,t,t1;
    unsigned int databuffer[30],temp;

    switch(mode)
    {
    case 0:
    	spi_enable();
    	do{
			spi_xmit_byte(0x93);
			DELAY_US(20);
			databuffer[count] = Read_touch_ad();
			count++;
    	}while(count<10);//(PEN==0)&&
    	spi_disable();

    	if(count == 10)
    	{
    		 do                                            //������X��������
			{
				t1=0;
				for(t=0;t<count-1;t++)
				{
					if(databuffer[t]>databuffer[t+1])//��������
					{
						temp=databuffer[t+1];
						databuffer[t+1]=databuffer[t];
						databuffer[t]=temp;
						t1=1;
					}
				}
			}while(t1);
    	}

    	break;
    case 1:
    	spi_enable();
		do{
			spi_xmit_byte(0xd3);
			DELAY_US(20);
			databuffer[count] = Read_touch_ad();
			count++;
		}while(count<10);//(PEN==0)&&
		spi_disable();

		if(count == 10)
		{
			 do                                            //������X��������
			{
				t1=0;
				for(t=0;t<count-1;t++)
				{
					if(databuffer[t]>databuffer[t+1])//��������
					{
						temp=databuffer[t+1];
						databuffer[t+1]=databuffer[t];
						databuffer[t]=temp;
						t1=1;
					}
				}
			}while(t1);
		}

		break;
    }

    for(count=3;count<8;count++)
    {
    	result1 = result1 + databuffer[count];
    }
    *p1 = result1/5;
}

// ��ȡ X Y ����ֵ
void TouchCoOrdGet(int *pX, int *pY)
{
    unsigned short xpos, ypos;
    char x = 1, y =0;
    int rx1 = 0;
    int ry1 = 0;
    unsigned int xDpos = 0;
    int yDpos = 0;

    /* read x-cordinates */
    tsc2046ReadAxis(x, &rx1);

    /* read y cordinates */
    tsc2046ReadAxis(y, &ry1);

    spi_enable();
    spi_xmit_byte(0x90);
    spi_disable();

    //while(!PEN);  //�ȴ��ͷ�

    xpos = rx1;

    ypos = ry1;

    xDpos = xpos * xscale + xoffset;
    yDpos = ypos * yscale + yoffset;

    yDpos = LCD_HEIGHT - yDpos;

    if(yDpos < 0)
    {
        yDpos = 0;
    }

    /* put touch screen in stand by mode.So that next
     * can be detected
     */
    *pX =(int)xDpos;
    *pY = yDpos;
}


