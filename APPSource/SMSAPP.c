#include "SMSAPP.h"



/**
���ָ��index�Ļ���ַ
@param index ���ŵ����
@return ����ָ����ŵ���Ϣ��EEPROM�Ļ���ַ
*/
static u16 SMS_Get_BaseAddrOfIndex(u8 index)
{
	u8 i;
	u8 indexTemp;
	u16 BaseAddr;
	for (i=0;i<SMS_MAX;i++)
	{
		BaseAddr=SMS_Base_Addr(i); ///<��õ�i���洢���Ļ���ַ
		eeprom_read(&indexTemp,BaseAddr,1);
		if (indexTemp==index) break; ///<�ҵ�Ҫ�ҵ����(index)�Ļ���ַ��
	}
	
	return BaseAddr;
}

/**
��ʽ������Ϣ�õ��ĵ�ַ
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
		BaseAddr=SMS_Base_Addr(i); ///<��õ�i���洢���Ļ���ַ
		eeprom_write(&i,BaseAddr,1);
		eeprom_write("\0",BaseAddr+SMS_INDEX_LEN,1);
		eeprom_write("\0",BaseAddr+SMS_INDEX_LEN+SMS_TITLE_MAX_LEN,1);
	}
	///������֤��ʽ���Ľ��
 #ifdef SMS_DEBUG_ON
	for (i=0;i<SMS_MAX;i++)
	{
		BaseAddr=SMS_Base_Addr(i); ///<��õ�i���洢���Ļ���ַ
		eeprom_read(&index,BaseAddr,1);
		printf("%d->%d\r\n",i,index);
	}
	printf("��ʽ������Ϣ�洢���ɹ�\r\n");
 #endif
}
/**
����һ����Ϣ��EEPROM��
@param pOutTitle ��Ϣ�ı���
@param pOutText  ��Ϣ������
*/
void SMS_Save(u8 *pOutTitle,u8 *pOutText)
{
	u16 i;
	u8 index;
	u16 BaseAddrTemp;
	u16 BaseAddr;
	//���Ƚ�������Ϣ��index��1��Ȼ�����һ����Ϣ��index��Ϊ0������������Ϣ���浽�����ַ��
	for (i=0;i<SMS_MAX;i++)
	{
		BaseAddr=SMS_Base_Addr(i); ///<��õ�i���洢���Ļ���ַ
		eeprom_read(&index,BaseAddr,1);
		if (index==(SMS_MAX-1))///<�ҵ���ţ�index�����ĵ�ַ�ˣ���������
		{
			BaseAddrTemp=BaseAddr;
			continue;
		}
		index++;
		eeprom_write(&index,BaseAddr,1);
	}
	//����index
	index=0;
	BaseAddr=BaseAddrTemp;	///���index��ַ
	eeprom_write(&index,BaseAddr,1);
	
	//����title
	i=0;
	BaseAddr=BaseAddrTemp+SMS_INDEX_LEN;///<���title��ַ
	while(*pOutTitle)
	{ 
		i++;
		if (i>(SMS_TITLE_MAX_LEN-1))
		{
			#ifdef SMS_DEBUG_ON
			printf("����̫����,���Զ���ȥ��\r\n");
			#endif
			break;   ///<�Ѿ������ܹ�������ֽ�����
		}
		eeprom_write(pOutTitle++,BaseAddr++,1);
	}
	eeprom_write("\0",BaseAddr++,1);///<����油һ��0���γ��ַ���
	
	//����text
	i=0;
	BaseAddr=BaseAddrTemp+SMS_INDEX_LEN+SMS_TITLE_MAX_LEN;///<���text��ַ
	while(*pOutText)
	{
		i++;
		if(i>(SMS_TEXT_MAX_LEN-1)) 
		{
			#ifdef SMS_DEBUG_ON
			printf("�������Ϣ�ĳ��ȳ�����,����Ĳ����ѽ�ȥ\r\n");
			#endif
			break;   ///<�Ѿ������ܹ�������ֽ�����
		}
			
		eeprom_write(pOutText++,BaseAddr++,1);		
	}
	eeprom_write("\0",BaseAddr++,1);///<����油һ��0���γ��ַ���
}



/**
����ָ����ŵĶ��ŵı���
@param index ���
@param pOutTitle �������
@param MaxLen ���ֻ�ܶ������ٸ��ֽڣ��������ƶ�������
@return ����ı����ж����ֽ�
*/
u8 SMS_Read_Title(u8 index,u8 *pOutTitle,u8 MaxLen)
{
	u8 i;
	u16 BaseAddr;
	//���������ַ
	BaseAddr=SMS_Get_BaseAddrOfIndex(index);  ///<���index���ֵΪָ��ֵ�Ļ���ַ
	BaseAddr+=SMS_INDEX_LEN;			///<��ñ���ĵ�ַ
	for (i=0;i<MaxLen;i++)
	{
		eeprom_read(pOutTitle,BaseAddr++,1);
		if(*pOutTitle==0) break; ///�ַ���������
		pOutTitle++;
	}
	return i;
}

/**
����ָ����ŵĶ��ŵ�����
@param index ���
@param pOutText �������
@param MaxLen ���ֻ�ܶ������ٸ��ֽڣ��������ƶ�������
@return ����������ж����ֽ�
*/
u8 SMS_Read_Text(u8 index,u8 offset,u8 *pOutText,u8 MaxLen)
{
	u8 i;
	u16 BaseAddr;
	if (offset>(SMS_TEXT_MAX_LEN-1)) ///<����Ŀ�ʼλ�ó����� 
	{
		#ifdef SMS_DEBUG_ON
		printf("����Ŀ�ʼλ�ó�����");
		#endif 
		return 0;
	}
	//���������ַ
	BaseAddr=SMS_Get_BaseAddrOfIndex(index);  ///<���index���ֵΪָ��ֵ�Ļ���ַ
	BaseAddr+=SMS_INDEX_LEN+SMS_TITLE_MAX_LEN+offset;			///<������ݵĵ�ַ
	for (i=0;i<MaxLen;i++)
	{
		eeprom_read(pOutText,BaseAddr++,1);
		if(*pOutText==0) break; ///�ַ���������
		pOutText++;
	}
	return i;
}
/**
ʾ������
@code
void main(void)
{
	eeprom_init();

	//SMS_Format();
	for (i=0;i<SMS_MAX;i++)
	{
		SMS_Save("�����","�����µ�");
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













