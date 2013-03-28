#include "LOGO.h"
#include "stdio.h"

void Dispay_LOGO(void)
{
//	u32 err=0;
	u8 i;
	u32 *pSTM32ID;
	printf(COMPANY_INFO);
	printf(VERSION);
	printf(COMPILED_DATE);
	printf("ID:");
	pSTM32ID=(u32 *)0x1FFFF7E8;
		
	for (i=0;i<3;i++)
	{
		printf("%x ",*pSTM32ID++);
	}
	printf("\r\n");
		
// 	scanf("%x",&err); 
// 	printf("%x\r\n",err);
}

