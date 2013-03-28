#ifndef _SMSAPP_H_BAB
#define _SMSAPP_H_BAB

#include "I2C_eeprom.h"

#define SMS_DEBUG_ON

#define SMS_FORMAT_FLAGE_ADDR	0	//是否已经格式化标志，当为0xAA时表明已经格式化了

#define SMS_EEPROM_BASEADDR 2			///<短信在EEPROM中开始存储的首地址
#define SMS_INDEX_LEN       1
#define SMS_TITLE_MAX_LEN   (6*2+1)  	///<包括字符串结束符\0
#define SMS_TEXT_MAX_LEN    ((32*2)*2+1)	///<包括字符串结束符\0

#define SMS_Base_Addr(n)		(SMS_EEPROM_BASEADDR+(SMS_INDEX_LEN+SMS_TITLE_MAX_LEN+SMS_TEXT_MAX_LEN)*n)		///<计算第n个存储区的的基地址


#define SMS_MAX				20			///<最多能保存多少条短消息

void SMS_Format(void);
void SMS_Save(u8 *pOutTitle,u8 *pOutText);
u8 SMS_Read_Title(u8 index,u8 *pOutTitle,u8 MaxLen);
u8 SMS_Read_Text(u8 index,u8 offset,u8 *pOutText,u8 MaxLen);
#endif
