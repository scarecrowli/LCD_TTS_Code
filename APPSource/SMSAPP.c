#include "SMSAPP.h"



/**
获得指定index的基地址
@param index 短信的序号
@return 返回指定序号的信息在EEPROM的基地址
*/
static u16 SMS_Get_BaseAddrOfIndex(u8 index)
{
	u8 i;
	u8 indexTemp;
	u16 BaseAddr;
	for (i=0;i<SMS_MAX;i++)
	{
		BaseAddr=SMS_Base_Addr(i); ///<获得第i个存储区的基地址
		eeprom_read(&indexTemp,BaseAddr,1);
		if (indexTemp==index) break; ///<找到要找的序号(index)的基地址了
	}
	
	return BaseAddr;
}

/**
格式化短消息用到的地址
*/
#ifdef SMS_DEBUG_ON
#include "stdio.h"
#endif 
void SMS_Format(void)
{
	u8 i;
	u16 BaseAddr;
#ifdef SMS_DEBUG_ON
	u8 index;
#endif 

	for (i=0;i<SMS_MAX;i++)
	{
		BaseAddr=SMS_Base_Addr(i); ///<获得第i个存储区的基地址
		eeprom_write(&i,BaseAddr,1);
		eeprom_write("\0",BaseAddr+SMS_INDEX_LEN,1);
		eeprom_write("\0",BaseAddr+SMS_INDEX_LEN+SMS_TITLE_MAX_LEN,1);
	}
	///用于验证格式化的结果
 #ifdef SMS_DEBUG_ON
	for (i=0;i<SMS_MAX;i++)
	{
		BaseAddr=SMS_Base_Addr(i); ///<获得第i个存储区的基地址
		eeprom_read(&index,BaseAddr,1);
		printf("%d->%d\r\n",i,index);
	}
	printf("格式化短信息存储区成功\r\n");
 #endif
}
/**
保存一条信息到EEPROM中
@param pOutTitle 信息的标题
@param pOutText  信息的内容
*/
void SMS_Save(u8 *pOutTitle,u8 *pOutText)
{
	u16 i;
	u8 index;
	u16 BaseAddrTemp;
	u16 BaseAddr;
	//首先将所有信息的index加1，然后将最后一个信息的index置为0，并把这条信息保存到这个地址上
	for (i=0;i<SMS_MAX;i++)
	{
		BaseAddr=SMS_Base_Addr(i); ///<获得第i个存储区的基地址
		eeprom_read(&index,BaseAddr,1);
		if (index==(SMS_MAX-1))///<找到序号（index）最大的地址了，保存起来
		{
			BaseAddrTemp=BaseAddr;
			continue;
		}
		index++;
		eeprom_write(&index,BaseAddr,1);
	}
	//保存index
	index=0;
	BaseAddr=BaseAddrTemp;	///获得index地址
	eeprom_write(&index,BaseAddr,1);
	
	//保存title
	i=0;
	BaseAddr=BaseAddrTemp+SMS_INDEX_LEN;///<获得title地址
	while(*pOutTitle)
	{ 
		i++;
		if (i>(SMS_TITLE_MAX_LEN-1))
		{
			#ifdef SMS_DEBUG_ON
			printf("标题太长了,已自动截去了\r\n");
			#endif
			break;   ///<已经超出能够保存的字节数了
		}
		eeprom_write(pOutTitle++,BaseAddr++,1);
	}
	eeprom_write("\0",BaseAddr++,1);///<最后面补一个0，形成字符串
	
	//保存text
	i=0;
	BaseAddr=BaseAddrTemp+SMS_INDEX_LEN+SMS_TITLE_MAX_LEN;///<获得text地址
	while(*pOutText)
	{
		i++;
		if(i>(SMS_TEXT_MAX_LEN-1)) 
		{
			#ifdef SMS_DEBUG_ON
			printf("保存短信息的长度超出了,多出的部分已截去\r\n");
			#endif
			break;   ///<已经超出能够保存的字节数了
		}
			
		eeprom_write(pOutText++,BaseAddr++,1);		
	}
	eeprom_write("\0",BaseAddr++,1);///<最后面补一个0，形成字符串
}



/**
读出指定序号的短信的标题
@param index 序号
@param pOutTitle 标题输出
@param MaxLen 最多只能读出多少个字节，用于限制读出数量
@return 输出的标题有多少字节
*/
u8 SMS_Read_Title(u8 index,u8 *pOutTitle,u8 MaxLen)
{
	u8 i;
	u16 BaseAddr;
	//计算出基地址
	BaseAddr=SMS_Get_BaseAddrOfIndex(index);  ///<获得index域的值为指定值的基地址
	BaseAddr+=SMS_INDEX_LEN;			///<获得标题的地址
	for (i=0;i<MaxLen;i++)
	{
		eeprom_read(pOutTitle,BaseAddr++,1);
		if(*pOutTitle==0) break; ///字符串结束了
		pOutTitle++;
	}
	return i;
}

/**
读出指定序号的短信的内容
@param index 序号
@param pOutText 内容输出
@param MaxLen 最多只能读出多少个字节，用于限制读出数量
@return 输出的内容有多少字节
*/
u8 SMS_Read_Text(u8 index,u8 offset,u8 *pOutText,u8 MaxLen)
{
	u8 i;
	u16 BaseAddr;
	if (offset>(SMS_TEXT_MAX_LEN-1)) ///<请求的开始位置超出了 
	{
		#ifdef SMS_DEBUG_ON
		printf("请求的开始位置超出了");
		#endif 
		return 0;
	}
	//计算出基地址
	BaseAddr=SMS_Get_BaseAddrOfIndex(index);  ///<获得index域的值为指定值的基地址
	BaseAddr+=SMS_INDEX_LEN+SMS_TITLE_MAX_LEN+offset;			///<获得内容的地址
	for (i=0;i<MaxLen;i++)
	{
		eeprom_read(pOutText,BaseAddr++,1);
		if(*pOutText==0) break; ///字符串结束了
		pOutText++;
	}
	return i;
}
/**
示例程序
@code
void main(void)
{
	eeprom_init();

	//SMS_Format();
	for (i=0;i<SMS_MAX;i++)
	{
		SMS_Save("标题哈","标题下的");
	}
	for (i=0;i<SMS_MAX;i++)
	{
		SMS_Read_Title(i,tempData,SMS_TITLE_MAX_LEN);
		printf("index=%d\r\ntitle:%s\r\n",i,tempData);
		
		SMS_Read_Text(i,tempData,SMS_TITLE_MAX_LEN);
		printf("text:%s\r\n",tempData);
	}
}
@endcode
*/













