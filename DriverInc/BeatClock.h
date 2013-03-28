#ifndef _BeatClock_H_BAB
#define _BeatClock_H_BAB

#include "stm32f10x.h"

typedef struct 
{
	u16 couter;	///<����
	u8 flage;	///<��־
}SoftTimerTypedef;

extern SoftTimerTypedef SoftTimerDelay;

#define Dleay_1S 100			///<��ʱ1S

void SoftTimerInit(void);
void Set_SoftTimer(u16 Value);
u8 Get_State_SoftTimer(void);

#endif
