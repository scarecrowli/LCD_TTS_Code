/*
车载终端与外设通信的通信协议实现
create ： 2012.4.16 by lindabell@ 欧海
*/
#include "PCP.h"
//#include "stdio.h"

u8 sendBuf[dSendBufSize];
u8 receiveBuf[dSendBufSize];

u8 PcpStatus=0;

/**
检查PCP当前的状态
@param checkStatus 要检查的状态
@return 1 表示这种状态，0 表示不是这种状态
*/
u8 PcpStatus_Is(u8 checkStatus)
{
	return (PcpStatus&checkStatus)==0?0:1;
}
/**
设置PCP的状态，必须和PcpClrStatus一起使用，因为这是一对互斥的
@param Status 设置哪一个状态
*/
static void PcpSetStatus(u8 Status) 
{
	PcpStatus|=Status;
}
/**
设置PCP的状态，必须和PcpSetStatus一起使用，因为这是一对互斥的
@param Status 清除哪一个状态
*/
static void PcpClrStatus(u8 Status) 
{
	PcpStatus&=~Status;
}

void PcpInit(void)
{
	PCP_DRIVER_INIT;
	PcpSetStatus(PcpStatus_SendBuf_Empty|PcpStatus_ReceiveBuf_Empty);
}
/*
in the funtion not to check Buf size ,there may be a BUG here
@param pPCPsrtuct the data srtuct
@return the bytes of Packaged data
*/
u8 PcpPackage(PCPsrtuctTypedef *pPCPsrtuct)
{
	static u8 tempBuf[dSendBufSize];  //if use malloc it was be better 
	u8 *pData;
	u8 CheckSum=0;
	u8 i,couter=0;
	u8 *pTempbuf;	//there need to user malloc memory
	pTempbuf=tempBuf;//pTempbuf=(u8 *)rt_malloc(dSendBufSize);

	//asser(); pPCPsrtuct can not be null
	pData=pTempbuf;///<get tempbuf addr

	*pData++=0x7E;	//there is The begain flage
	*pData++=0x00;		//The checksum will be save at last
	*pData++=pPCPsrtuct->Version>>8;		//get version high byter
	*pData++=pPCPsrtuct->Version & 0x00FF;	//get version low byter

	*pData=pPCPsrtuct->Company>>8;
	CheckSum+=*pData++;
	*pData=pPCPsrtuct->Company & 0x00FF;
	CheckSum+=*pData++;

	*pData=pPCPsrtuct->EquipmentType;
	CheckSum+=*pData++;

	*pData=pPCPsrtuct->CMD;
	CheckSum+=*pData++;

	for (i=0;i<pPCPsrtuct->userDataLen;i++)
	{
		*pData=pPCPsrtuct->userData[i];
		CheckSum+=*pData++;
	}
	*pData=0x7E;	//add the end flage
	pTempbuf[1]=CheckSum;
/*take Package data to the sendBuf*/
	pData=sendBuf; ///<get sendBuf addr
	for (i=0;i<(pPCPsrtuct->userDataLen+9);i++)
	{
		if(pTempbuf[i]==0x7e)
		{
			if(i==0 || i==(pPCPsrtuct->userDataLen+9-1))	*pData++=pTempbuf[i];
			else
			{
				*pData++=0x7d;
				*pData++=0x02;
				couter++;
			}
		}
		else if (pTempbuf[i]==0x7d)
		{
			*pData++=0x7d;
			*pData++=0x01;
			couter++;
		}
		else
		{	
			*pData++=pTempbuf[i];

		}
	}
	
	PcpSetStatus(PcpStatus_SendBuf_Full);
	PcpClrStatus(PcpStatus_SendBuf_Empty);
	PCP_DRIVER_Write_Sart;
//	sendBuf_ready=1; //send buf data is ready ,set the let other fuc to know
	return (i+couter);
}

/**
UnPackage data from PCP
@param pOutPCPsrtuct where UnPackaged data will save
@return 1 is Success UnPackage,0 is err 
*/
u8 PcpUnPackage(PCPsrtuctTypedef *pOutPCPsrtuct)
{
	u8 *pData;
	u8 checkSum=0;
	u8 i=0;
//	if(receiveBuf_ready==0) return 0; //the receiveBuf donot have data
	if(PcpStatus_Is(PcpStatus_ReceiveBuf_Empty)) return 0; //the receiveBuf donot have data
	pData=receiveBuf;

	pData++;
	if (*pData==0x7d)
	{
		pData++;
		if(*pData==0x01)pOutPCPsrtuct->Checksum=0x7d;
		else if(*pData==0x02)pOutPCPsrtuct->Checksum=0x7e;
		pData++;
	}
	else pOutPCPsrtuct->Checksum=*pData++;

	if (*pData==0x7d)
	{
		pData++;
		if(*pData==0x01)pOutPCPsrtuct->Version=0x7d<<8;
		else if(*pData==0x02)pOutPCPsrtuct->Version=0x7e<<8;
		pData++;
	}
	else pOutPCPsrtuct->Version=*pData++<<8;

	if (*pData==0x7d)
	{
		pData++;
		if(*pData==0x01)pOutPCPsrtuct->Version|=0x7d;
		else if(*pData==0x02)pOutPCPsrtuct->Version|=0x7e;
		pData++;
	}
	else pOutPCPsrtuct->Version|=*pData++;

	if (*pData==0x7d)
	{
		pData++;
		if(*pData==0x01){pOutPCPsrtuct->Company=0x7d<<8;checkSum+=0x7d;}
		else if(*pData==0x02){pOutPCPsrtuct->Company=0x7e<<8;checkSum+=0x7e;}
		pData++;
	}
	else {pOutPCPsrtuct->Company=*pData<<8;checkSum+=*pData++;}

	if (*pData==0x7d)
	{
		pData++;
		if(*pData==0x01){pOutPCPsrtuct->Company|=0x7d;checkSum+=0x7d;}
		else if(*pData==0x02){pOutPCPsrtuct->Company|=0x7e;checkSum+=0x7e;}
		pData++;
	}
	else {pOutPCPsrtuct->Company|=*pData;checkSum+=*pData++;}

	if (*pData==0x7d)
	{
		pData++;
		if(*pData==0x01){pOutPCPsrtuct->EquipmentType=0x7d;checkSum+=0x7d;}
		else if(*pData==0x02){pOutPCPsrtuct->EquipmentType=0x7e;checkSum+=0x7e;}
		pData++;
	}
	else {pOutPCPsrtuct->EquipmentType=*pData;checkSum+=*pData++;}
	
	if (*pData==0x7d)
	{
		pData++;
		if(*pData==0x01){pOutPCPsrtuct->CMD=0x7d;checkSum+=0x7d;}
		else if(*pData==0x02){pOutPCPsrtuct->CMD=0x7e;checkSum+=0x7e;}
		pData++;
	}
	else {pOutPCPsrtuct->CMD=*pData;checkSum+=*pData++;}

	i=0;
	while (*pData!=0x7e)
	{
		if (*pData==0x7d)
		{
			pData++;
			if(*pData==0x01){pOutPCPsrtuct->userData[i]=0x7d;checkSum+=0x7d;}
			else if(*pData==0x02){pOutPCPsrtuct->userData[i]=0x7e;checkSum+=0x7e;}
			pData++;
		}
		else {pOutPCPsrtuct->userData[i]=*pData;checkSum+=*pData++;}
		i++;
	}
	pOutPCPsrtuct->userDataLen=i;
	if (checkSum==pOutPCPsrtuct->Checksum) 
	{
//		receiveBuf_ready=0; //the receiveBuf data was decode then crear the flage

		PcpSetStatus(PcpStatus_ReceiveBuf_Empty);
		PcpClrStatus(PcpStatus_ReceiveBuf_Full);
		return 1;
	}
	else 
	{
		PcpSetStatus(PcpStatus_ReceiveBuf_Empty);
		PcpClrStatus(PcpStatus_ReceiveBuf_Full);	
		return 0;
	}

}
/**
receive data stream and save to a buf
@param Data get from driver
@return 1 there get a Package of data,0 is need to continue
*/
u8 PcpReceivePackage(u8 Data)
{
	static u16 i=0;
	if(Data==0x7E)
	{
		//printf("get 7e\r\n");
		if(i!=0)
		{
			if(i==1) //there maybe get "7E 7E", then get the last one only
			{
				//printf("get double 7e\r\n");
				return 0;
			}
			receiveBuf[i]=Data;
			i=0;// reset couter 
//			receiveBuf_ready=1; //set the flage let other fucton to know
			//printf("end\r\n");

			PcpSetStatus(PcpStatus_ReceiveBuf_Full);
			PcpClrStatus(PcpStatus_ReceiveBuf_Empty);
			return 1;	//get a package of data
		}
		receiveBuf[i++]=Data;
	}
	else
	{
		receiveBuf[i++]=Data;
	}
	
	return 0;
}

/**
the fuc is get data from buf,then can use driver to send
@param OutData the data need driver to send
@retrun 1 is the packge finish ,0 need continue to send
*/
u8 PcpSendPackage(u8 *OutData)
{
	static u16 i=0;
//	if(sendBuf_ready==0) return 1;	//send buf donot have any data to send
	if(PcpStatus_Is(PcpStatus_SendBuf_Empty)) return 1;	//send buf donot have any data to send
	if(sendBuf[i]==0x7e)
	{
		if(i!=0)
		{
			*OutData=sendBuf[i];
			i=0;	//a packge is end ,then need to reset couter

			PcpSetStatus(PcpStatus_SendBuf_Empty);
			PcpClrStatus(PcpStatus_SendBuf_Full);
//			sendBuf_ready=0; //send buf data was be take out,so the buf is empyt now
			return 1;
		}
		*OutData=sendBuf[i++];
	}
	else
	{
		*OutData=sendBuf[i++];
	}
	return 0;
}

/*
   this will check vision、Company、Type of Equipment，if not Match with location that 
   todo will be stop and return err code
*/
u8 PcpToDo(PCPsrtuctTypedef *pPCPsrtuct)
{
	//fist need to check CMD is ask for location version or not ,if not to check info
	if(pPCPsrtuct->CMD!=dCmd_Version)
	{
		//check vision
		if(pPCPsrtuct->Version!=dInfo_Version) return 1;
		//check Company
		if(pPCPsrtuct->Version!=dInfo_Company) return 2;
		//check the Type of Equipment
		if(pPCPsrtuct->Version!=dInfo_EquipmentType) return 3;
	}
	
	switch(pPCPsrtuct->CMD)
	{
	case dCmd_PowerOn:
		break;
	case dCmd_LinkInquire:
		break;
	case dCmd_PowerControl:
		break;
	case dCmd_Version:
		break;

	}
	return 0;
}


/**<
@code
//示例程序 \n
//获取汉子“四”的点阵数据
int main(void)
{
	u8 dataBuf[32];

	PcpInit();

	PCPsrtuct.CMD=0x01;
	PCPsrtuct.Company=0x0203;
	PCPsrtuct.EquipmentType=0x04;
	PCPsrtuct.userDataLen=20;
	PCPsrtuct.Version=0x4567;
	for(i=0;i<PCPsrtuct.userDataLen;i++)
	{
		PCPsrtuct.userData[i]=i;
	}
		
	PcpPackage(&PCPsrtuct);
		
	while(1)
	{
		if(PcpStatus_Is(PcpStatus_ReceiveBuf_Full))
		{
			PcpUnPackage(&PCPsrtuct);
			PcpPackage(&PCPsrtuct);
		}
	}
}

@endcode
*/
