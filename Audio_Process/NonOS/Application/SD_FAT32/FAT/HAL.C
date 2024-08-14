#include	"DSP2833x_Device.h"



unsigned short WordSwap(unsigned short input)
{
	return(((input&0x00FF)<<8)|((input&0xFF00)>>8));
}

void DelayMs(unsigned int nFactor)
{
	unsigned char i;
	unsigned int j;

	for(i=0; i<nFactor; i++)
		{
		for(j=0;j<1000;j++)	j=j;		          
		}
}

unsigned long SwapINT32(unsigned long dData)
{
    dData = (dData&0xff)<<24|(dData&0xff00)<<8|(dData&0xff000000)>>24|(dData&0xff0000)>>8;
	return dData;
}

unsigned int SwapINT16(unsigned short dData)
{
    dData = (dData&0xff00)>>8|(dData&0x00ff)<<8;
	return dData;
}

unsigned int LSwapINT16(unsigned short dData1,unsigned short dData2)
{
    unsigned int dData;
    dData = ((dData2<<8)&0xff00)|(dData1&0x00ff);
	return dData;
}

unsigned long LSwapINT32(unsigned long dData1,unsigned long dData2,unsigned long dData3,unsigned long dData4)
{
    unsigned long dData;
    dData = ((dData4<<24)&0xff000000)|((dData3<<16)&0xff0000)|((dData2<<8)&0xff00)|(dData1&0xff);
	return dData;
}


