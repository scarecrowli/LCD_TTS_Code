#include "MenuAPP.h"
#include "I2C_eeprom.h"

void mainPageCallBack(u8 key);
void mainPage_Item1_CallBack(u8 key);

extern const struct PAGE Version_Page;
void Version_CallBack(u8 key);

extern const struct PAGE Setting_Page;
void Setting_CallBack(u8 key);

extern const struct PAGE Time_Page;
void Time_CallBack(u8 key);

extern const struct PAGE SMS_Page;
void SMS_CallBack(u8 key);
 
extern const struct PAGE SMS_Text_Page;
void SMS_Text_CallBack(u8 key);

/******************************************************************************************************/
//���˵�
//����Item��             //��ʾ��ʽ&���  ��Ŀ������    ��Ŀָ���ҳ(Page)
const struct Item main_item[]={	0x00,	"��Ϣ",			&SMS_Page,
								0x01,	"����",			&Setting_Page,
								0x02,	"�汾",			&Version_Page,
								0x03,	"ʱ��",			&Time_Page,
								0x04,	"״̬",			0,
								0x05,	"����",			0,
								0x06,	"����",			0,
								0x07,	"�ʴ�",			0
};
//����һ��Page		      ��ҳ ��ҳ�Ļص�����	��ҳ����          ��ĸ���				
const struct PAGE mainPage={0,mainPageCallBack,main_item,sizeof(main_item)/sizeof(struct Item)};
/*********************************************************************************************************/


const struct PAGE Version_Page={&mainPage,Version_CallBack,0,0};
/***************************************************************************************************************/

//����Item��              //��ʾ��ʽ&���    ��Ŀ������      ��Ŀָ���ҳ(Page)
const struct Item Setting_item[]={	0x10,	" 00.��0",			0,
									0x11,	" 01.��1",			0,
									0x12,	" 02.��2",			0,
									0x13,	" 03.��3",			0,
									0x14,	" 04.��4",			0,
									0x15,	" 05.��5",			0,
									0x16,	" 06.��6 ���",		0,
									0x17,	" 07.��7",			0,
									0x18,	" 08.��8",			0,
									0x19,	" 09.��9",			0,
									0x1A,	" 10.��10",			0
									};
const struct PAGE Setting_Page={&mainPage,Setting_CallBack,Setting_item,sizeof(Setting_item)/sizeof(struct Item)};
/***************************************************************************************************************/

const struct PAGE Time_Page={&mainPage,Time_CallBack,0,0};

/***************************************************************************************************************/
//����Item��              //��ʾ��ʽ&���    ��Ŀ������      ��Ŀָ���ҳ(Page)
const struct Item SMS_item[]={	
									0x10,	" 00.",			&SMS_Text_Page,
									0x11,	" 01.",			&SMS_Text_Page,
									0x12,	" 02.",			&SMS_Text_Page,
									0x13,	" 03.",			&SMS_Text_Page,
									0x14,	" 04.",			&SMS_Text_Page,
									0x15,	" 05.",			&SMS_Text_Page,
									0x16,	" 06.",			&SMS_Text_Page,
									0x17,	" 07.",			&SMS_Text_Page,
									0x18,	" 08.",			&SMS_Text_Page,
									0x19,	" 09.",			&SMS_Text_Page,
									0x1A,	" 10.",			&SMS_Text_Page
									};

const struct PAGE SMS_Page={&mainPage,SMS_CallBack,SMS_item,sizeof(Setting_item)/sizeof(struct Item)};

//�������ж�����
#define THE_NUM_OF_SMS_ITEM  11


#if (THE_NUM_OF_SMS_ITEM>SMS_MAX)
#error "the number  of SMS item must not beyond than SMS EEPROM Item "
#endif
/***************************************************************************************************************/

const struct PAGE SMS_Text_Page={&SMS_Page,SMS_Text_CallBack,0,0};

/**
���˵��ص������������ҳ�ô���ȫ�����ڻص�������
@param key ��������
*/
void mainPageCallBack(u8 key)
{
	switch (key)
	{
		case KEY_UP:	
		case KEY_Down:	
		case KEY_Left:	
		case KEY_Right:
			KeySelItem(key);
			break;
			
		case KEY_Return:///<���˵� �Է��ذ���û�д���
			ShowPage(&mainPage);
			break;
		case KEY_Ok:
			ShowItemPage();
			break;
	}
}
 

void Version_CallBack(u8 key)
{
	LCD_Write_Str(0,0,"�汾��Ϣ");
	LCD_Write_Str(1,0,"���̣�³����ѧ");
	LCD_Write_Str(2,0,"��ַ����̨");
	LCD_Write_Str(3,0,"�汾��V0.4");
	if (key==KEY_Return)
	{
		ShowParentPage();
	}
}


void Setting_CallBack(u8 key)
{
	switch (key)
	{
		case KEY_UP:	
		case KEY_Down:	
		case KEY_Left:	
		case KEY_Right:
			KeySelItem(key);
			break;
			
		case KEY_Return:///<���˵� �Է��ذ���û�д���
			ShowParentPage();
			break;
		case KEY_Ok:
			ShowItemPage();
			break;
	}
}


void Time_CallBack(u8 key)
{
	LCD_Write_Str(0,0,"����: 2012-7-5");
	LCD_Write_Str(1,0,"ʱ��: 16:59");
	if (key==KEY_Return)
	{
		ShowParentPage();
	}
}

void SMS_CallBack(u8 key)
{
	u8 i,tempData[SMS_TITLE_MAX_LEN];
	u8 max,maxTemp;
	u8 min,minTemp;
	u8 SelIndex,SelIndexTemp;
	
	SelIndexTemp=Menu_GetSelItem();	//��õ�ǰѡ�е�index
	GetShowLst(&minTemp,&maxTemp);		 //��ȡ��ǰ��ʾ�ķ�Χ
	
	switch (key)
	{
		case KEY_UP:	
		case KEY_Down:	
		case KEY_Left:	
		case KEY_Right:
			KeySelItem(key);
			
			SelIndex=Menu_GetSelItem();	//��õ�ǰѡ�е�index
			GetShowLst(&min,&max);		 //��ȡ��ǰ��ʾ�ķ�Χ
			if (max==maxTemp) break;//���ʾ��ǰ��ʾ���б�û�з����仯
			
			for (i=min;i<=max;i++)
			{
				//��ȡSMS��Ϣ��title����ʾ
				SMS_Read_Title(i,tempData,SMS_TITLE_MAX_LEN);
				LCD_Write_Str(i-min,2,tempData);
			}
			
			break;
			
		case KEY_Return:///<���˵� �Է��ذ���û�д���
			ShowParentPage();
			break;
		case KEY_Ok:
			ShowItemPage();
			break;
		case KEY_Special://��һ�ν���ʱ��ʾ����
			for (i=0;i<4;i++)
			{
				SMS_Read_Title(i,tempData,SMS_TITLE_MAX_LEN);
				LCD_Write_Str(i,2,tempData);
			}
			
			break;
	}
}

void SMS_Text_CallBack(u8 key)
{
	u8 SelItemIdex;
	u8 tempData[SMS_TEXT_MAX_LEN];
	u8 len;
	static u8 offset=0;
	
	switch (key)
	{
	case KEY_Special: 
		offset=0;
		SelItemIdex=Menu_GetSelItem();		
		SMS_Read_Text(SelItemIdex,offset,tempData,SMS_TEXT_MAX_LEN);  ///<��EEPROM�ж�����Ϣ�ı�
		offset+=LCD_Write_Screen(tempData);
		break;
	case KEY_Left:
		if(offset<=64) break;  ///<offset ���ڵ���64����ǰ�ǵ�һ�����������û�������

		offset=(offset/64-1)*64;
		SelItemIdex=Menu_GetSelItem();		
		SMS_Read_Text(SelItemIdex,offset,tempData,SMS_TEXT_MAX_LEN);  ///<��EEPROM�ж�����Ϣ�ı�
		offset+=LCD_Write_Screen(tempData);
		break;
	case KEY_Right:
		if (offset%64) break; ///<����offset����64������˵����ǰҺ��û����ʾ�꣬��û��������
		SelItemIdex=Menu_GetSelItem();		
		SMS_Read_Text(SelItemIdex,offset,tempData,SMS_TEXT_MAX_LEN);  ///<��EEPROM�ж�����Ϣ�ı�
		Lcd_Clr_Scr();
		offset+=LCD_Write_Screen(tempData);
		break;
	case KEY_Return:
		ShowParentPage();
		return;
		break;
			
	}
	
	

}


