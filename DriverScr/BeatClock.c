#include "BeatClock.h"

SoftTimerTypedef SoftTimerDelay;

void SoftTimerInit(void)
{
	SoftTimerDelay.couter=0;
	SoftTimerDelay.flage=0;
}
/**
*set soft timer couter
*@param Value The Value of coter
*/
void Set_SoftTimer(u16 Value)
{
	SoftTimerDelay.couter=Value;
	SoftTimerDelay.flage=0;
}

/**
get the state of soft timer
@return The state of soft timer . 1:soft time is end 0:is not end now
*/
u8 Get_State_SoftTimer(void)
{
	return SoftTimerDelay.flage;
}
