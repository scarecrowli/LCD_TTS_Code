#include "Menu.h"

//����ѡ�еĲ˵������
static u8 SelItem=0;

/**
���ڵ�ǰLCD�б�����ʾ���ļ���
��4λ��������
��4Ϊ����С���
*/
static u8 ListShow=0x00;

const struct PAGE *pPage;

void SelItemOfList(u8 index);

void SetMainPage(const struct PAGE *pMainPage)
{
	pPage=pMainPage;
}
/**
��õ�ǰѡ�еĲ˵���
@return ���ز˵����
*/
u8 Menu_GetSelItem(void)
{
	return SelItem;
}

/**
��ȡ��ǰ��ʾ�б�ķ�Χ
@param pOutMin ��ǰ��ʾ����С���
@param pOutMax ��ǰ��ʾ��������
*/
void GetShowLst(u8 *pOutMin,u8 *pOutMax)
{
	*pOutMin=ListShow&0x0f; 
	*pOutMax=ListShow>>4;
}
void ShowList(u8 min,u8 max)
{
	u8 i=0,index=0;
	#if MENU_DEBUG
		if(max-min>3)
		{
			Lcd_Clr_Scr();
			LCD_Write_Str(0,0,"err:ShowList>3");
			while (1);
		}
		
		if ((pPage->pItem[0].TypeAndIndex & 0x10)==0)///<�����ʹ���б�ʽ
		{
			
				Lcd_Clr_Scr();
				LCD_Write_Str(0,0,"�����б����Ͳ��ܲ����г�");
				while (1);	
		}
	#endif
	
	Lcd_Clr_Scr();
	for (index=min;index<=max;index++)
	{

		LCD_Write_Str(i++,0,pPage->pItem[index].pText);
	}
	ListShow=(max<<4)|min; ///<��¼��ǰ��ʾ��Item
	
}
/**
ҳ��ʾ

1.�����ҳ����Ŀ(Item)ʱ����ʾItem��ͬʱѡ��Item 0  	\n
2.û��ʱ:����ø�Page�Ļص�����������KEY_Special ����	\n
@param pPage ָ��һ��page
*/
void ShowPage( const struct PAGE *pPage)
{
	s8 i;
	///����
	Lcd_Clr_Scr();
	  
	if(pPage->pItem==0) 
	{
		pPage->Function(KEY_Special);
		return; ///<���û��Item������ʾItem��ֱ�ӷ���
	}
		
	if (pPage->pItem[0].TypeAndIndex & 0x10)///<�����ʹ���б�ʽ
	{
		ShowList(0,3);
		SelItemOfList(0);
		pPage->Function(KEY_Special);
	}
	else
	{	
		///ȡ��page�е�Item����ʾ
		for (i=0;i<pPage->ItemNum;i++)
		{
			if (i<4)
			{
				LCD_Write_Str(i,1,pPage->pItem[i].pText);
			}
			else
			{
				LCD_Write_Str(i-4,5,pPage->pItem[i].pText);
			}
			
		}
		SelPageItem(0);///<ѡ��Item 0
		pPage->Function(KEY_Special);
	}
	
};

/**
��ʾ��ҳ(ParentPage)
*/
void ShowParentPage(void)
{
	pPage=pPage->pParent;
	ShowPage(pPage);
}

/**
��ʾ��Ŀ(Item)�¶�Ӧ��ҳ(Page)
*/
void ShowItemPage(void)
{
	//���������û��ҳ���⾯��򷵻�
	if (pPage->pItem[Menu_GetSelItem()].pChildrenPage ==0)
	{
		#if MENU_DEBUG
			Lcd_Clr_Scr();
			LCD_Write_Str(0,0,"����������ʾ������");
			while (1);
		#else
			return;
		#endif 
	}
	pPage=pPage->pItem[Menu_GetSelItem()].pChildrenPage; //��ò˵���(Item)��Ӧ��page

	ShowPage(pPage);
}

/**
ѡ��page�е�Item��
@param ItemIndex page��Item�������� 0~7
*/
void SelPageItem(u8 ItemIndex)
{
	///����Ƿ��д������
#if MENU_DEBUG

	if (ItemIndex>=8)
	{
		LCD_Write_Str(0,0,"���ò˵������");
		return;
	}
#endif

///����ϴ�ѡ�е�
   if (SelItem<4)
   {
		LCD_Write_Str(SelItem,0,"  ");
		LCD_Write_Str(SelItem,3,"  ");
	
   }
   else
   {
		LCD_Write_Str(SelItem-4,4,"  ");
		LCD_Write_Str(SelItem-4,7,"  ");
   }
///ѡ�����Ҫѡ�е�  
   if (ItemIndex<4)
   {
		LCD_Write_Str(ItemIndex,0,"��");
		LCD_Write_Str(ItemIndex,3,"��");
		SelItem=ItemIndex;
   }
   else
   {
		LCD_Write_Str(ItemIndex-4,4,"��");
		LCD_Write_Str(ItemIndex-4,7,"��");
		SelItem=ItemIndex;
   }	
};
void SelItemOfList(u8 index)
{
	u8 max;
	u8 min;
	
	max=ListShow>>4;
	min=ListShow&0x0f;
	
	if (index>max) ///<�������ǰ��ʾ�����
	{
		
		LCD_Write_Str(Menu_GetSelItem()-min,0," ");
		
		min+=1;
		max+=1;
		ShowList(min,max);
		ListShow=(max<<4)|min;
		
		LCD_Write_Str(index-min,0,">");
		
	}
	else if(index>=min)///<����С��������֮��
	{
		LCD_Write_Str(Menu_GetSelItem()-min,0," ");
		LCD_Write_Str(index-min,0,">");
	}
	else					///<������С��ǰ��ʾ��С���
	{
		LCD_Write_Str(Menu_GetSelItem()-min,0," ");
		
		min-=1;
		max-=1;
		ShowList(min,max);
		ListShow=(max<<4)|min;
		
		LCD_Write_Str(index-min,0,">");
	}
	SelItem=index;
}
void KeySelItem(u8 key)
{
	s8 index;
	if (pPage->pItem[0].TypeAndIndex & 0x10)///<�����ʹ���б�ʽ
	{
		switch(key)
		{
			case KEY_UP:
				index=Menu_GetSelItem()-1;
				if(index<0) break;
				
				SelItemOfList(index);
				break;
			case KEY_Down:
				index=Menu_GetSelItem()+1;
				if(index>(pPage->ItemNum-1)) break;;
				SelItemOfList(index);
				break;
		}
		return;
	}
	switch(key)
	{
		case KEY_UP:
			index=Menu_GetSelItem()-1;
			if(index<0) index=pPage->ItemNum-1;
			SelPageItem(index);
			break;
		case KEY_Down:
			index=Menu_GetSelItem()+1;
			if(index>(pPage->ItemNum-1)) index=0;
			SelPageItem(index);
			break;
		case KEY_Left:
		case KEY_Right:	
			index=Menu_GetSelItem();
			if (index<4)
			{
				if((index+4)>(pPage->ItemNum-1)) return; //��û��Item��޷�ѡ���ұ�����Է���
				index+=4;								//�ұ���Itemʱ��index��λ���ұߵ�Item
			}	
			else	    index-=4;						//��Ϊ�ұ���Item��ʱ�����һ����Item���Ϊ�ǰ�˳���ŵ�
			SelPageItem(index);
			break;
	}
}

