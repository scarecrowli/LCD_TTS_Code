#ifndef _PCP_H_BAB
#define _PCP_H_BAB

#include "USART2.h"
#include "stm32f10x.h"
//Type of Equipment
#define dType_infoTerminal					0x01	//行业信息终端机
#define dType_SchedulingScreen				0x02	//调度显示屏
#define dType_CarNavigation					0x03	//车载导航显示屏
#define dType_OilDetectInstrument			0x04	//油量检测仪
#define dType_AccelerateDetectInstrument	0x05	//加速度检测仪
#define dType_BurglarAlarm					0x06	//防盗报警器
#define dType_InterfaceExtension			0x07	//接口扩展器
#define dType_LoadDetectInstrument			0x08	//载重检测器
#define dType_PassengerDetectInstrument		0x09	//客流检测器
#define dType_GeneralSensor					0x0a	//通用传感器

// cmd
#define dCmd_PowerOn		0x01	//上电指示/应答
#define dCmd_LinkInquire	0x02	//链路探询/应答
#define dCmd_PowerControl	0x03	//外设电源控制
#define dCmd_Version		0x04	//查询外设版本号


//定义命令字 其中0x01~0x04是PCP标准命令字 具体参见PCP.h
#define LCDCMD_Text					0x05	///<LCD显示文本
#define LCDCMD_VoiceText			0x06	///<TTS播放文本
#define LCDCMD_VoiceAndText			0x07	///<LCD显示文本、TTS播放文本


typedef struct  
{	u8	Checksum;	//校验和
	u16	Version;	//版本
	u16	Company;	//厂商
	u8	EquipmentType;		//外设类型
	u8	CMD;		//命令
	u8 userData[0xFF];//用户数据
	u8 userDataLen; //用户数据长度
}PCPsrtuctTypedef;

#define dSendBufSize	(sizeof(PCPsrtuctTypedef)+2+10) 



//define location info
#define dInfo_Version			0x0001
#define dInfo_Company			0x0BAB
#define dInfo_EquipmentType		dType_infoTerminal

#define PcpStatus_SendBuf_Empty			(1<<0)
#define PcpStatus_SendBuf_Full			(1<<1)
#define PcpStatus_ReceiveBuf_Empty	(1<<2)
#define PcpStatus_ReceiveBuf_Full		(1<<3)

///Driver 
#define PCP_DRIVER_INIT 		usart2_configure()	///<PCP' driver init
#define PCP_DRIVER_Write_Sart	USART_ITConfig(USART2, USART_IT_TXE, ENABLE)
#define PCP_DRIVER_Write_Stop


u8 PcpPackage(PCPsrtuctTypedef *pPCPsrtuct);
u8 PcpUnPackage(PCPsrtuctTypedef *pOutPCPsrtuct);
u8 PcpReceivePackage(u8 Data);
u8 PcpSendPackage(u8 *OutData);
u8 PcpStatus_Is(u8 checkStatus);
void PcpInit(void);

#endif

