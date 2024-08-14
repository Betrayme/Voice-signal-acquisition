#include	"DSP2833x_Device.h"
////////////////////////////////////////
extern SYS_INFO_BLOCK   DeviceInfo;
extern FILE_INFO   ThisFile;
extern unsigned char   DBUF[BUFFER_LENGTH];
extern	unsigned char FATBUF[512];

////////////////////////////////////////

unsigned long FirstSectorofCluster16(unsigned int clusterNum)
{
	unsigned long temp;
	temp=clusterNum-2;
	temp=temp*DeviceInfo.BPB_SecPerClus;
	temp=temp+DeviceInfo.FirstDataSector;
	return temp;
}

unsigned int ThisFatSecNum16(unsigned int clusterNum)   
{
   unsigned int temp;
   temp=clusterNum*2;
   temp=temp/DeviceInfo.BPB_BytesPerSec;
   temp=temp+DeviceInfo.FatStartSector;
   return temp;
}

unsigned int ThisFatEntOffset16(unsigned int clusterNum)
{
	unsigned int temp1,temp2;
	temp1=2*clusterNum;
	temp2=temp1/DeviceInfo.BPB_BytesPerSec;
	temp1=temp1-temp2*DeviceInfo.BPB_BytesPerSec;
	return temp1;
}

unsigned int GetNextClusterNum16(unsigned int clusterNum)
{
	unsigned int FatSecNum,FatEntOffset;
	
	FatSecNum=ThisFatSecNum16(clusterNum);
	FatEntOffset=ThisFatEntOffset16(clusterNum);
	if(ThisFile.FatSectorPointer!=FatSecNum)
	{	
		
		if(!RBC_Read(FatSecNum,1,FATBUF))
			return 0xFFFF;
		ThisFile.FatSectorPointer=FatSecNum;
	}
	
	///////////////////////////////////////////////////
	clusterNum=FATBUF[FatEntOffset+1];
	clusterNum=clusterNum<<8;
	clusterNum+=FATBUF[FatEntOffset];	
	return clusterNum;
}

unsigned char GoToPointer16(unsigned long pointer)
{
	
	unsigned int clusterSize;
	
	clusterSize=DeviceInfo.BPB_SecPerClus*DeviceInfo.BPB_BytesPerSec;
	ThisFile.ClusterPointer=ThisFile.StartCluster;
	while(pointer>clusterSize)
	{
		pointer-=clusterSize;	
		ThisFile.ClusterPointer=GetNextClusterNum16(ThisFile.ClusterPointer);
		if(ThisFile.ClusterPointer==0xffff)
		{
			return FALSE;
		}
	}
	ThisFile.SectorofCluster=pointer/DeviceInfo.BPB_BytesPerSec;
	ThisFile.SectorPointer=FirstSectorofCluster16(ThisFile.ClusterPointer)+ThisFile.SectorofCluster;
	ThisFile.OffsetofSector=pointer-ThisFile.SectorofCluster*DeviceInfo.BPB_BytesPerSec;
	ThisFile.FatSectorPointer=0;
	return TRUE;
	
}

unsigned char DeleteClusterLink16(unsigned int clusterNum)
{
	unsigned int FatSecNum,FatEntOffset;
	unsigned char i;
	while((clusterNum>1)&&(clusterNum<0xfff0))
	{
		FatSecNum=ThisFatSecNum16(clusterNum);
		FatEntOffset=ThisFatEntOffset16(clusterNum);
		if(RBC_Read(FatSecNum,1,DBUF))
		{
			clusterNum=DBUF[FatEntOffset+1];
			clusterNum=clusterNum<<8;
			clusterNum+=DBUF[FatEntOffset];	
		}
		else
			return FALSE;
		DBUF[FatEntOffset]=0x00;
		DBUF[FatEntOffset+1]=0x00;	
		for(i=0;i<DeviceInfo.BPB_NumFATs;i++)
		{
			if(!RBC_Write(FatSecNum+i*DeviceInfo.BPB_FATSz16,1,DBUF))
				return FALSE;
		}	
	}
	return TRUE;
}

unsigned int GetFreeCusterNum16(void)
{
	unsigned int clusterNum,i;
	unsigned long sectorNum;
	unsigned char j;
	clusterNum=0;
	sectorNum=DeviceInfo.FatStartSector;
	while(sectorNum<DeviceInfo.BPB_FATSz16+DeviceInfo.FatStartSector)
	{
		
		if(!RBC_Read(sectorNum,1,DBUF))
			return 0x0;
		for(i=0;i<DeviceInfo.BPB_BytesPerSec;i=i+2)
		{
		  	 if((DBUF[i]==0)&&(DBUF[i+1]==0))
		  	 {	
		  	 	DBUF[i]=0xff;
		  	 	DBUF[i+1]=0xff;
				for(j=0;j<DeviceInfo.BPB_NumFATs;j++)
				{
					DelayMs(10);
					if(!RBC_Write(sectorNum+j*DeviceInfo.BPB_FATSz16,1,DBUF))
						return FALSE;
				}	
		  	  	return	clusterNum; 
		  	 }
		  	 clusterNum++;
		 }	
				
		sectorNum=2*clusterNum/DeviceInfo.BPB_BytesPerSec+DeviceInfo.FatStartSector;	
	}
	
	return 0x0;
}

unsigned int CreateClusterLink16(unsigned int currentCluster)
{
	unsigned int newCluster;
	unsigned int FatSecNum,FatEntOffset;
	unsigned char i;

	newCluster=GetFreeCusterNum16();

	if(newCluster==0)
		return 0x00;
			
	FatSecNum=ThisFatSecNum16(currentCluster);
	FatEntOffset=ThisFatEntOffset16(currentCluster);
	if(RBC_Read(FatSecNum,1,DBUF))
	{
		DBUF[FatEntOffset]=newCluster;
		DBUF[FatEntOffset+1]=newCluster>>8;
		for(i=0;i<DeviceInfo.BPB_NumFATs;i++)
		{
			DelayMs(10);
			if(!RBC_Write(FatSecNum+i*DeviceInfo.BPB_FATSz16,1,DBUF))
				return FALSE;
		}		
	}
	else
		return 0x00;
	return newCluster;
}
