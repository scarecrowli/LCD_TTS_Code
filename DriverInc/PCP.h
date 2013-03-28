#ifndef _PCP_H_BAB
#define _PCP_H_BAB

#include "USART2.h"
#include "stm32f10x.h"
//Type of Equipment
#define dType_infoTerminal					0x01	//��ҵ��Ϣ�ն˻�
#define dType_SchedulingScreen				0x02	//������ʾ��
#define dType_CarNavigation					0x03	//���ص�����ʾ��
#define dType_OilDetectInstrument			0x04	//���������
#define dType_AccelerateDetectInstrument	0x05	//���ٶȼ����
#define dType_BurglarAlarm					0x06	//����������
#define dType_InterfaceExtension			0x07	//�ӿ���չ��
#define dType_LoadDetectInstrument			0x08	//���ؼ����
#define dType_PassengerDetectInstrument		0x09	//���������
#define dType_GeneralSensor					0x0a	//ͨ�ô�����

// cmd
#define dCmd_PowerOn		0x01	//�ϵ�ָʾ/Ӧ��
#define dCmd_LinkInquire	0x02	//��·̽ѯ/Ӧ��
#define dCmd_PowerControl	0x03	//�����Դ����
#define dCmd_Version		0x04	//��ѯ����汾��


//���������� ����0x01~0x04��PCP��׼������ ����μ�PCP.h
#define LCDCMD_Text					0x05	///<LCD��ʾ�ı�
#define LCDCMD_VoiceText			0x06	///<TTS�����ı�
#define LCDCMD_VoiceAndText			0x07	///<LCD��ʾ�ı���TTS�����ı�


typedef struct  
{	u8	Checksum;	//У���
	u16	Version;	//�汾
	u16	Company;	//����
	u8	EquipmentType;		//��������
	u8	CMD;		//����
	u8 userData[0xFF];//�û�����
	u8 userDataLen; //�û����ݳ���
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

