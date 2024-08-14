
#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "LCD.h"



/****************端口宏定义*****************/
#define LCD_CS GpioDataRegs.GPADAT.bit.GPIO8
#define LCD_RS GpioDataRegs.GPBDAT.bit.GPIO34
#define LCD_WR GpioDataRegs.GPADAT.bit.GPIO9
#define LCD_RD GpioDataRegs.GPBDAT.bit.GPIO49
#define LCD_DB GpioDataRegs.GPCDAT.all

//定义LCD的尺寸
#define LCD_W 480
#define LCD_H 272

unsigned int  HDP=479;
unsigned int  HT=531;
unsigned int  HPS=43;
unsigned int  LPS=8;
unsigned int   HPW=10;

unsigned int  VDP=271;
unsigned int  VT=288;
unsigned int  VPS=12;
unsigned int  FPS=4;
unsigned int   VPW=10;

void Init_Port(void)
{
	EALLOW;
	// DB0~15
	GpioCtrlRegs.GPCMUX1.all = 0;            // GPIO64~79
	GpioCtrlRegs.GPCDIR.all = 0XFFFFFFFF;    // output
	GpioDataRegs.GPCCLEAR.all = 0XFFFFFFFF;
	// LCD_CS
	GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 0;  // GPIO8
	GpioCtrlRegs.GPADIR.bit.GPIO8 = 1;   // output
	GpioDataRegs.GPACLEAR.bit.GPIO8 = 1;
	// LCD_WR
	GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 0;  // GPIO9
	GpioCtrlRegs.GPADIR.bit.GPIO9 = 1;   // output
	GpioDataRegs.GPACLEAR.bit.GPIO9 = 1;
	// LCD_RS
	GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 0;  // GPIO34
	GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1;   // output
	GpioDataRegs.GPBCLEAR.bit.GPIO34 = 1;
	// LCD_RD
	GpioCtrlRegs.GPBMUX2.bit.GPIO49 = 0;  // GPIO49
	GpioCtrlRegs.GPBDIR.bit.GPIO49 = 1;   // output
	GpioDataRegs.GPBCLEAR.bit.GPIO49 = 1;
	EDIS;
}

void LCD_WR_Bus(unsigned int data)
{
	Uint32 temp = 0;
	unsigned int i;

	for(i=0;i<16;i++)
	{
		if(data & 0x01)
			temp |= 0x01;

		if(i == 15)
			break;

		temp = temp << 1;
		data >>= 1;
	}

	LCD_DB = temp;	//赋值给D0-D15对应的GPIO79-GPIO64引脚
}

//写寄存器地址函数
void LCD_WR_REG(unsigned int index)
{
	LCD_CS = 0;
	LCD_RS=0;
	LCD_WR_Bus(index);
	LCD_WR=0;
	LCD_WR=1;
	LCD_CS=1;
}

//写16位像素数据函数
void LCD_WR_DATA(unsigned int val)
{
	LCD_CS = 0;
	LCD_RS = 1;
	LCD_WR_Bus(val);
	LCD_WR=0;
	LCD_WR=1;
	LCD_CS=1;
}

//写寄存器数据函数
void LCD_WR_CMD(unsigned int index,unsigned int val)
{
	LCD_WR_REG(index);
	LCD_WR_DATA(val);
}

void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{
	LCD_WR_REG(0x002A);
	LCD_WR_DATA(x1 >>8);
	LCD_WR_DATA(x1 & 0x00ff);
	LCD_WR_DATA(x2 >> 8);
	LCD_WR_DATA(x2 & 0x00ff);
    LCD_WR_REG(0x002b);
	LCD_WR_DATA(y1 >> 8);
	LCD_WR_DATA(y1 & 0x00ff);
	LCD_WR_DATA(y2 >> 8);
	LCD_WR_DATA(y2 & 0x00ff);
	LCD_WR_REG(0x002c);
}



void Lcd_Init(void)
{

//调用一次这些函数，免得编译的时候提示警告
	LCD_CS=1;
	LCD_RD=1;
	LCD_WR=1;

	LCD_WR_REG(0x00E2);	//PLL multiplier, set PLL clock to 120M
	LCD_WR_DATA(0x0023);	    //N=0x36 for 6.5M, 0x23 for 10M crystal
	LCD_WR_DATA(0x0002);
	LCD_WR_DATA(0x0004);
	LCD_WR_REG(0x00E0);  // PLL enable
	LCD_WR_DATA(0x0001);
	DELAY_US(1000);
	LCD_WR_REG(0x00E0);
	LCD_WR_DATA(0x0003);
	DELAY_US(5000);
	LCD_WR_REG(0x0001);  // software reset
	DELAY_US(5000);
	LCD_WR_REG(0x00E6);	//PLL setting for PCLK, depends on resolution
	LCD_WR_DATA(0x0003);
	LCD_WR_DATA(0x00ff);
	LCD_WR_DATA(0x00ff);

	LCD_WR_REG(0x00B0);	//LCD SPECIFICATION
	LCD_WR_DATA(0x0020);//0x0027
	LCD_WR_DATA(0x0000);
	LCD_WR_DATA((HDP>>8)&0X00FF);  //Set HDP
	LCD_WR_DATA(HDP&0X00FF);
    LCD_WR_DATA((VDP>>8)&0X00FF);  //Set VDP
	LCD_WR_DATA(VDP&0X00FF);
    LCD_WR_DATA(0x0000);

	LCD_WR_REG(0x00B4);	//HSYNC
	LCD_WR_DATA((HT>>8)&0X00FF);  //Set HT
	LCD_WR_DATA(HT&0X00FF);
	LCD_WR_DATA((HPS>>8)&0X00FF);  //Set HPS
	LCD_WR_DATA(HPS&0X00FF);
	LCD_WR_DATA(HPW);			   //Set HPW
	LCD_WR_DATA((LPS>>8)&0X00FF);  //Set HPS
	LCD_WR_DATA(LPS&0X00FF);
	LCD_WR_DATA(0x0000);

	LCD_WR_REG(0x00B6);	//VSYNC
	LCD_WR_DATA((VT>>8)&0X00FF);   //Set VT
	LCD_WR_DATA(VT&0X00FF);
	LCD_WR_DATA((VPS>>8)&0X00FF);  //Set VPS
	LCD_WR_DATA(VPS&0X00FF);
	LCD_WR_DATA(VPW);			   //Set VPW
	LCD_WR_DATA((FPS>>8)&0X00FF);  //Set FPS
	LCD_WR_DATA(FPS&0X00FF);

	LCD_WR_REG(0x00BA);
	LCD_WR_DATA(0x000F);    //GPIO[3:0] out 1

	LCD_WR_REG(0x00B8);
	LCD_WR_DATA(0x0007);    //GPIO3=input, GPIO[2:0]=output
	LCD_WR_DATA(0x0001);    //GPIO0 normal

	LCD_WR_REG(0x0036); //rotation
	LCD_WR_DATA(0x0000);

	LCD_WR_REG(0x00F0); //pixel data interface
	LCD_WR_DATA(0x0003);

	DELAY_US(5000);

	LCD_WR_REG(0x0029); //display on

	LCD_Clear(WHITE);
}

void LCD_Clear(unsigned int Color)
{
	unsigned int i,j;
	Address_set(0,0,LCD_W-1,LCD_H-1);
	for(i=0;i<LCD_H;i++)
	{
		for (j=0;j<LCD_W;j++)
		{
			LCD_WR_DATA(Color);
		}
	}
}

//画点
//x:0~479
//y:0~271
//color:此点的颜色
void LCD_DrawPoint(unsigned int x,unsigned int y,unsigned int color)
{
	Address_set(x,y,x,y);//设置光标位置
	LCD_WR_DATA(color);
}

//显示Z_HIGH*Z_WHITH大小的汉字
void LCD_ShowHanZi(unsigned int x,unsigned int y,unsigned char *zi,
		unsigned char Z_WHITH,unsigned char Z_HIGH,unsigned int Color)
{
	 unsigned int x0,y0,i,j,temp;
	 x0 = x;
	 y0 = y;
	 for(y0=y; y<y0+Z_HIGH; y++)
	 {
	 	x = x0;
		for(i=0; i<(Z_WHITH/8); i++)
		{
			temp = zi[(y-y0)*(Z_WHITH/8) + i] ;
			for(j=0; j<8; j++)
			{
				if(temp&0x80)LCD_DrawPoint(x,y,Color);
	 			else
				{
//					LCD_DrawPoint(x,y,Color);
				}
				temp<<=1;
				x++;
			}
		}
	 }
}

void LCD_ShowHanZis(unsigned int x,unsigned int y,unsigned char *zi,
		unsigned char Z_WHITH,unsigned char Z_HIGH,char num, unsigned int Color)
{
	char i;
	for(i=0;i<num;i++)
	{
		LCD_ShowHanZi(x+Z_WHITH*i,y,&zi[i*Z_WHITH*Z_HIGH/8],Z_WHITH,Z_HIGH,Color);
	}
}


