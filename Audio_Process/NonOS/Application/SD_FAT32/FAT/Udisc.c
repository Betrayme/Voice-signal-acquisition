/////////////////////////////////////
#include	"DSP2833x_Device.h"

FLAGS bFlags;
extern unsigned char DBUF[BUFFER_LENGTH];
unsigned char usector;
extern unsigned long  DirStartCluster32;		
extern unsigned long  DirStartCluster;
extern SYS_INFO_BLOCK   DeviceInfo;
unsigned char DataBuf[BUFFER_LENGTH];
extern	FILE_INFO   	 ThisFile;

/////////////////////////////////////////////////
char play_Udisc();

char play_Udisc()
{
    int temp,i;

    DirStartCluster=0;
	DirStartCluster32=0;

	for(temp=0;temp<512;temp++)
		DBUF[temp]=0;

	DirStartCluster=0;
	DirStartCluster32=0;
	ThisFile.FatSectorPointer=0;
	DeviceInfo.LastFreeCluster=0;

	if(InitFileSystem())
	{
		bFlags.bits.SLAVE_IS_ATTACHED = TRUE;
	}
	else
	{
		bFlags.bits.SLAVE_IS_ATTACHED = FALSE;
	}
	
	if(bFlags.bits.SLAVE_IS_ATTACHED)
	{
		SCIPuts("Create test.txt file.\r\n", -1);
		CreateFile("test.txt",0x20);

		SCIPuts("Write characters to the file : www.tronlong.com\r\n", -1);
		i=0;
		DataBuf[i++]='w';
		DataBuf[i++]='w';
		DataBuf[i++]='w';
		DataBuf[i++]='.';
		DataBuf[i++]='t';
		DataBuf[i++]='r';
		DataBuf[i++]='o';
		DataBuf[i++]='n';
		DataBuf[i++]='l';
		DataBuf[i++]='o';
		DataBuf[i++]='n';
		DataBuf[i++]='g';
		DataBuf[i++]='.';
		DataBuf[i++]='c';
		DataBuf[i++]='o';
		DataBuf[i++]='m';
		DataBuf[i++]= 13;
		DataBuf[i++]= 10;

		SetFilePointer(ThisFile.LengthInByte);
		WriteFile(18,DataBuf);

		return TRUE;
	}

	return FALSE;
}
