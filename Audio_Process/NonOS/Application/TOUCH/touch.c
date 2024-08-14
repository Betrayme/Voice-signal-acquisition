/*
 * 			广州创龙电子科技有限公司
 *
 * Copyright 2015 Tronlong All rights reserved
 */

#include "DSP2833x_Device.h"     // Headerfile Include File
#include "DSP2833x_Examples.h"   // Examples Include File
#include "touch.h"

#define TOUCH_AD_LEFT     86
#define TOUCH_AD_RIGHT    1968 /*A/D转换器返回的最大值*/
#define TOUCH_AD_TOP      180
#define TOUCH_AD_BOTTOM   1940

#define LCD_HEIGHT        272

float xscale;
float yscale;
float xoffset;
float yoffset;

char touch_flag = 0;

//初始化SPI函数
void spi_init()
{
	SpiaRegs.SPICCR.all =0x0007;	    //复位, 数据在上升沿输出, 8 bits 数据模式
	SpiaRegs.SPICTL.all =0x000E;        //使能主机模式, 时钟延时半个时钟周期,
                                        //使能主机发送, 禁止SPI中断.
	SpiaRegs.SPIBRR =0x0063;			//SPI波特率=37.5M/100	=0.375MHZ；
    SpiaRegs.SPICCR.all =0x0087;		//退出初始状态；
    SpiaRegs.SPIPRI.bit.FREE = 1; 		//自由运行
}

// SPI 传输一个字节
Uint16 spi_xmit_byte(Uint16 byte)
{
     byte = byte << 8;
     //注意这里的有效数据是8位，SPI初始化中也进行了定义

    SpiaRegs.SPITXBUF = byte;							//把发送的数据写如SPI发送缓冲区
    while( SpiaRegs.SPISTS.bit.BUFFULL_FLAG==1);		//当发送缓冲区出现满标志位时,开始琐存数据
    DELAY_US(20);

    return (SpiaRegs.SPIRXBUF << 8);
}

// 使能 SPI 片选
void spi_enable(void)
{
	GpioDataRegs.GPADAT.bit.GPIO11 = 1;      // 输出高电平
	GpioDataRegs.GPBDAT.bit.GPIO57 = 0;      // 输出低电平
}

// 禁止 SPI 片选
void spi_disable(void)
{
	GpioDataRegs.GPADAT.bit.GPIO11 = 1;      // 输出高电平
	GpioDataRegs.GPBDAT.bit.GPIO57 = 1;      // 输出高电平
}

// 触摸屏初始化
void touch_setup(void)
{
	//  本例中用到的中断重新映射到本件的中断服务子程序；
	EALLOW;	// 修改被保护的寄存器，修改前应添加EALLOW语句
	PieVectTable.XINT1 = &touch_isr;
	EDIS;   // EDIS的意思是不允许修改被保护的寄存器

	// 在外设中断扩展模块中断使能寄存器中允许XINT1: （组1中断4）
	// 使能CPU中断1（INT1）:
	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;          // 使能外设中断扩展PIE模块
	PieCtrlRegs.PIEIER1.bit.INTx4 = 1;          // 使能PIE组1的INT4
	IER |= M_INT1;                              // 使能CPU中断1（INT1）
	EINT;                                       // 开全局中断

	// 设置GPIO13为输入,作为中断 GPIO12为输入BUSY
	EALLOW;
	GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 0;         // GPIO
	GpioCtrlRegs.GPADIR.bit.GPIO13 = 0;          // input
	GpioCtrlRegs.GPAQSEL1.bit.GPIO13 = 0;

	GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 0;         // GPIO
	GpioCtrlRegs.GPADIR.bit.GPIO12 = 0;          // input
	GpioCtrlRegs.GPAQSEL1.bit.GPIO12 = 0;
	EDIS;

	// 通过GPIO外部中断选择寄存器，选择GPIO13为外部中断1
	EALLOW;
	GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 13;   // XINT1是GPIO13
	EDIS;

	// 配置外部中断1的中断控制寄存器
	XIntruptRegs.XINT1CR.bit.POLARITY = 0;      // 下降沿触发中断

	// 使能外部中断1
	XIntruptRegs.XINT1CR.bit.ENABLE = 1;        // 使能XINT1

	// 初始化GPIO;
	InitSpiaGpio();

	spi_init();

	EALLOW;
	// 配置SN74HC138
	GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 0; // 配置GPIO11为GPIO口
	GpioCtrlRegs.GPADIR.bit.GPIO11 = 1;      // 定义GPIO11输出引脚
	GpioCtrlRegs.GPAPUD.bit.GPIO11 = 0;      // 禁止上啦 GPIO17引脚
	GpioDataRegs.GPADAT.bit.GPIO11 = 1;      // 输出高电平

	// SN74HC138 作为SPI_CS2_TOUCH
	GpioCtrlRegs.GPBMUX2.bit.GPIO57 = 0; // 配置GPIO11为GPIO口
	GpioCtrlRegs.GPBDIR.bit.GPIO57 = 1;      // 定义GPIO17输出引脚
	GpioDataRegs.GPBDAT.bit.GPIO57 = 1;      // 输出高电平
	EDIS;

	touch_init_xy();
}

// 触摸屏输出中断
interrupt void touch_isr(void)
{
	GpioDataRegs.GPBTOGGLE.bit.GPIO50=1;

	if (GpioDataRegs.GPADAT.bit.GPIO12==1)
	{
		touch_flag = 1;
	}

	// 应答寄存器的位1清0，以响应同组内其他中断；
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

// 初始化触摸屏坐标
void touch_init_xy(void)
{
	xscale=(float)480.0 / (TOUCH_AD_RIGHT - TOUCH_AD_LEFT);//得到xscale
	xoffset=(float)(480.0 - xscale * TOUCH_AD_RIGHT);//得到xoff

	yscale=(float)272.0 / (TOUCH_AD_TOP - TOUCH_AD_BOTTOM);//得到yscale
	yoffset=(float)(272.0 - yscale * TOUCH_AD_TOP);//得到yoff
}

// 触摸检测
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

// 读取 AD 值
int Read_touch_ad(void)
{
	int result=0,re1,re2;

	re1 = spi_xmit_byte(0);
	re2 = spi_xmit_byte(0);

	result = (re1<<4) + (re2>>4);

	return result;
}

// 根据模式获取 x 坐标和 y 坐标的 AD 值
// mode = 0 ---- x坐标
// mode = 1 ---- y坐标
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
    		 do                                            //将数据X升序排列
			{
				t1=0;
				for(t=0;t<count-1;t++)
				{
					if(databuffer[t]>databuffer[t+1])//升序排列
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
			 do                                            //将数据X升序排列
			{
				t1=0;
				for(t=0;t<count-1;t++)
				{
					if(databuffer[t]>databuffer[t+1])//升序排列
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

// 获取 X Y 坐标值
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

    //while(!PEN);  //等待释放

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


