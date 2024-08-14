#include "common.h"
#include "DEVICE.H"
#include "HAL.H"
#include "HPI16.H"
#include "HPI32.H"
//#include "SD.H"
//////////////////////////////////
extern unsigned char DBUF[BUFFER_LENGTH];
extern SYS_INFO_BLOCK DeviceInfo;
extern FILE_INFO ThisFile;
extern unsigned int DirStartCluster;
extern unsigned long  DirStartCluster32;

extern void sd_read_block(unsigned int sector, unsigned char *pBuffer);
extern void sd_write_block(unsigned int sector, unsigned char *pBuffer);

unsigned char  DBUF[BUFFER_LENGTH];
	
///////////////////////////////////////////////////////////////////////////
unsigned char InitFileSystem(void)
{
	unsigned int ReservedSectorsNum;

	////////////////////////////////////////////////////
	DeviceInfo.BPB_BytesPerSec=512; //暂假设为512
	

	////////////////////////////////////////////////////
	if(!RBC_Read(0x0,1,DBUF))
		return FALSE;
    if(DBUF[510] != 0x55 || DBUF[511] != 0xaa) return FALSE;

	//////////////////////////////////
	if(DBUF[0]==0xeb||DBUF[0]==0xe9)
		{
		DeviceInfo.StartSector=0;
		}
	else
		{
		 if(DBUF[446] != 0x80 && DBUF[446] != 0)  return FALSE;
		 DeviceInfo.StartSector=LSwapINT32(DBUF[454],DBUF[455],DBUF[456],DBUF[457]);
		}
	///////////////////////////////////////////////////////
	if(!RBC_Read(DeviceInfo.StartSector,1,DBUF))
		return FALSE;
	
	if(DBUF[510] != 0x55 || DBUF[511] != 0xaa) return FALSE;

	DeviceInfo.BPB_BytesPerSec=LSwapINT16(DBUF[11],DBUF[12]);
	DeviceInfo.BPB_SecPerClus=DBUF[13];
	ReservedSectorsNum=LSwapINT16(DBUF[14],DBUF[15]);
	DeviceInfo.BPB_NumFATs=DBUF[16];

	if(DBUF[82]=='F'&&DBUF[83]=='A'&&DBUF[84]=='T'&&DBUF[85]=='3'&&DBUF[86]=='2')
	{
		DeviceInfo.BPB_RootEntCnt=LSwapINT16(DBUF[17],DBUF[18]);
		DeviceInfo.BPB_RootEntCnt=(DeviceInfo.BPB_RootEntCnt)*32/DeviceInfo.BPB_BytesPerSec;
		DeviceInfo.BPB_TotSec32=LSwapINT32(DBUF[32],DBUF[33],DBUF[34],DBUF[35]);
		DeviceInfo.BPB_FATSz32=LSwapINT32(DBUF[36],DBUF[37],DBUF[38],DBUF[39]);
		DeviceInfo.RootStartCluster=LSwapINT32(DBUF[44],DBUF[45],DBUF[46],DBUF[47]);
		DeviceInfo.FatStartSector=DeviceInfo.StartSector+ReservedSectorsNum;
		DeviceInfo.FirstDataSector=DeviceInfo.FatStartSector+DeviceInfo.BPB_NumFATs*DeviceInfo.BPB_FATSz32;
		//DeviceInfo.TotCluster=(DeviceInfo.BPB_TotSec32-DeviceInfo.FirstDataSector+1)/DeviceInfo.BPB_SecPerClus+1;
		DeviceInfo.TotCluster=(DeviceInfo.BPB_TotSec32-ReservedSectorsNum-DeviceInfo.BPB_NumFATs*DeviceInfo.BPB_FATSz32-DeviceInfo.BPB_RootEntCnt)/DeviceInfo.BPB_SecPerClus;
		DirStartCluster32=DeviceInfo.RootStartCluster;
		DeviceInfo.FAT=1;	//FAT16=0,FAT32=1;
		
	}
	else
	{		
		DeviceInfo.BPB_RootEntCnt=LSwapINT16(DBUF[17],DBUF[18]);
		DeviceInfo.BPB_RootEntCnt=(DeviceInfo.BPB_RootEntCnt)*32/DeviceInfo.BPB_BytesPerSec;	
		DeviceInfo.BPB_TotSec16=LSwapINT16(DBUF[19],DBUF[20]);	
		if(DeviceInfo.BPB_TotSec16==0)
		  DeviceInfo.BPB_TotSec16=LSwapINT32(DBUF[32],DBUF[33],DBUF[34],DBUF[35]);
		DeviceInfo.BPB_FATSz16=LSwapINT16(DBUF[22],DBUF[23]);			
		DeviceInfo.FatStartSector=DeviceInfo.StartSector+ReservedSectorsNum;
		DeviceInfo.RootStartSector=DeviceInfo.StartSector+DeviceInfo.BPB_NumFATs*DeviceInfo.BPB_FATSz16+ReservedSectorsNum;	
		DeviceInfo.FirstDataSector=DeviceInfo.FatStartSector+DeviceInfo.BPB_NumFATs*DeviceInfo.BPB_FATSz16+DeviceInfo.BPB_RootEntCnt;
		DeviceInfo.TotCluster=(DeviceInfo.BPB_TotSec16-DeviceInfo.BPB_RootEntCnt-DeviceInfo.BPB_NumFATs*DeviceInfo.BPB_FATSz16-1)/DeviceInfo.BPB_SecPerClus;
        if(DeviceInfo.TotCluster<4085) return FALSE;	//FAT12 不被支持
		DeviceInfo.FAT=0;
	}		
	///////////////////////////////////////////////////////
	ThisFile.bFileOpen=0;	
	///////////////////////////////////////////////////////
	return TRUE;
}

unsigned char RBC_Read(unsigned long sector,unsigned char len,unsigned char *pBuffer)
{
	while(len--)
	{
	  	sd_read_block(sector,pBuffer); 
    	pBuffer+=512;
	 }
  return 1;
}

unsigned char RBC_Write(unsigned long sector,unsigned char len,unsigned char *pBuffer)
{
	while(len--)
	 {
	  sd_write_block(sector,pBuffer);
    pBuffer+=512;
	 }
 return 1;  
}

