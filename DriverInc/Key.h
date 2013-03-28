#ifndef _Key_H_BAB
#define _Key_H_BAB

#include "stm32f10x.h"

#define KEY1_PIN	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)
#define KEY2_PIN	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)
#define KEY3_PIN	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)
#define KEY4_PIN	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)
#define KEY5_PIN	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)
#define KEY6_PIN	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)

///根据通用键盘代码分配
#define KEY_NONE	0
#define KEY_UP		38
#define KEY_Down	40
#define KEY_Left	37
#define KEY_Right	39
#define KEY_Return	27
#define KEY_Ok 		13

void Key_init(void);

void key_scan(void);
u8 Key_Get_key(void);

#endif
