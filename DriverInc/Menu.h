#ifndef _Menu_H_BAB
#define _Menu_H_BAB

#include "stm32f10x.h"
#include "LCD.h"
#include "Key.h"


#define KEY_Special 	255	///<������������ر��¼�


//�˵����ԣ��ڵ���ʱ��ö��壬���԰����������⣻������ʱ������Ϊ0���Լӿ��ٶ�
#define MENU_DEBUG 1



void Menu_Show(void);



struct PAGE
{
	const struct PAGE *pParent;
	void (*Function)(u8 key);
	const struct Item *pItem;
	const u8 ItemNum;
};
struct Item
{
	/**
	��4λ��Ϊ������;��bit4=1��ʾ�б���ʾ����������ʾ������4λ���ڱ��Item�����  \n
	���Ϊ�б�ģʽʱ*pText�ĸ�ʽΪ��" xx.string",��ǰ�汣��һ���ո����ڸ����(>)ʹ�ã�xx.Ϊ��λ��Ų�Ҫ"."һ��Ҫ�У�string��Ҫ��ʾ�����֣��������ʾ6������  \n
	�����������ʾ��pText����ΪҪ��ʾ���ı������2�����֣�
	*/
	const u8 TypeAndIndex;	
	const u8 *pText;
	const struct PAGE *pChildrenPage;
};


extern const struct PAGE *pPage;

void SetMainPage(const struct PAGE *pMainPage);
void ShowMenu(const struct PAGE *pPage);
void ShowPage(const struct PAGE *pPage);
void ShowParentPage(void);
void ShowItemPage(void);
void SelPageItem(u8 ItemIndex);
u8 Menu_GetSelItem(void);

void GetShowLst(u8 *pOutMin,u8 *pOutMax);

void KeySelItem(u8 key);

#endif 
