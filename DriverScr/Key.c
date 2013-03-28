#include "Key.h"

static u8 key=KEY_NONE;


/**
按键初始化
*/
void Key_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
}

/**
获得按键代码
*/
u8 Key_Get_key(void)
{
	u8 temp;
	temp=key;
	key=KEY_NONE;
	return temp;
}

/**
按键扫描
*/
void key_scan(void)
{	
	if (KEY1_PIN)
	{
		key=KEY_Right;
	}
	else if(KEY2_PIN)
	{
		key=KEY_Down;
	}
	else if(KEY3_PIN)
	{
		key=KEY_Ok;
	}
	else if(KEY4_PIN)
	{
		key=KEY_Return;
	}
	else if(KEY5_PIN)
	{
		key=KEY_UP;
	}
	else if(KEY6_PIN)
	{
		key=KEY_Left;
	}
}
