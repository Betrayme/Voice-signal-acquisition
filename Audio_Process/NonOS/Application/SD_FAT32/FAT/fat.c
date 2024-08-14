#include	"DSP2833x_Device.h"

extern	SYS_INFO_BLOCK   DeviceInfo;
extern	FILE_INFO   	 ThisFile;
extern 	FLAGS bFlags;

unsigned  char  List(void)
{
	unsigned char flag;
	if(DeviceInfo.FAT)
		flag = List32();
	else
		flag = List16();
	return	flag;		
}

unsigned  char	OpenFile(char *str)
{
	unsigned char flag;
	if(DeviceInfo.FAT)
		flag = OpenFile32(str);
	else
		flag = OpenFile16(str);
	return	flag;		
}
unsigned  char  ReadFile(unsigned long readLength, unsigned char *pBuffer)
{
	unsigned  char  flag;
	if(DeviceInfo.FAT)
		flag = ReadFile32(readLength,pBuffer);
	else
		flag = ReadFile16(readLength,pBuffer);
	return	flag;		
}

unsigned  char SetFilePointer(unsigned long pointer)
{
	unsigned  char  flag;
	if(DeviceInfo.FAT)
		flag = SetFilePointer32(pointer);
	else
		flag = SetFilePointer16(pointer);
	return	flag;		
}

unsigned  char  CreateFile(char *str,unsigned char attr)
{
	unsigned  char  flag;
	if(DeviceInfo.FAT)
		{
			flag = OpenFile32(str);
			if (flag)
				flag = SetFilePointer32(ThisFile.LengthInByte);
			else
				flag = CreateFile32(str,attr);	
		}	
	else{
			flag = OpenFile16(str);
			if (flag)
				flag = SetFilePointer16(ThisFile.LengthInByte);
			else
				flag = CreateFile16(str,attr);	
		}	
	return	flag;			
}

unsigned  char  WriteFile(unsigned long writeLength,unsigned char *pBuffer)
{
	unsigned  char flag;
	if (DeviceInfo.FAT)
		flag = WriteFile32(writeLength,pBuffer);
	else
		flag = WriteFile16(writeLength,pBuffer);
	return  flag;	
}
unsigned  char  RemoveFile(char *str)
{
	unsigned  char flag;
	if (DeviceInfo.FAT)
		flag = RemoveFile32(str);
	else
		flag = RemoveFile16(str);
	ThisFile.bFileOpen =0;
	return  flag;
}
unsigned long GetCapacity()
{
	if (DeviceInfo.FAT)
		return(GetCapacity32());
	else
		return(GetCapacity16());		
}

unsigned  char  DetectDisk()
{
	if(bFlags.bits.SLAVE_IS_ATTACHED)
		return 1;
	else 
		return 0;	
}
unsigned  char  CreateDir(char *str)
{
	unsigned  char  flag;
	if (DeviceInfo.FAT)
		{
			flag = DownDir32(str);
			if (!flag)
				flag = CreateDir32(str);
		}
	else{
			flag = DownDir16(str);
			if (!flag)
				flag = CreateDir16(str);
		}	
	return  flag;	
}

unsigned  char  DownDir(char *str)
{
	unsigned  char  flag;
	if (DeviceInfo.FAT)
		flag = DownDir32(str);
	else
		flag = DownDir16(str);	
	return	flag;	
}

unsigned  char  UpDir()
{
	unsigned  char  flag;
	if (DeviceInfo.FAT)
		flag = UpDir32();
	else
		flag = UpDir16();
	return  flag;	
}
unsigned  char  UpRootDir()
{
	unsigned  char  flag;
	if (DeviceInfo.FAT)
		flag = UpRootDir32();
	else
		flag = UpRootDir16();
	return  flag;		
}

