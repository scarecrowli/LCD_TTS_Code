#include "Includes.h"


#define MYVAL0 10
#define MYVAL1 9
/**
*DSFKALJ
*KK
*肯定是建立
*/
PCPsrtuctTypedef PCPsrtuct;

#define BufferSize 10
#define EEPROM_Address 40
u8 Tx1_Buffer[BufferSize];
u8 Rx1_Buffer[BufferSize];

#define delayUs(x) { unsigned int _dcnt; \
      _dcnt=(x*16); \
      while(_dcnt-- > 0) \
      { continue; }\
     }
u32 *pSTM32ID;

u8 tempData[255];
int main(void)
{
   u8 result=0;
	u8 err=0;
	u8 i;
	Key_init();
	PcpInit();
	TTS_init();
	LCD_Hardware_Init();
	led_configure();
	eeprom_init();
	
	///Usart init
	usart1_configure();	
	usart3_configure();
	
		///等外设准备好
		///SysTick confige
	SoftTimerInit();
	err=SysTick_Config(SystemCoreClock/100);
	if(err==1)
	{
		printf("SysTick_Config fail\r\n");
	} 
	///等外设准备好
	Set_SoftTimer(Dleay_1S/5);
	while(!Get_State_SoftTimer());///<waite soft timer end up
	LCD_init(); 
	Lcd_Clr_Scr();
	
	err=TTS_set_text(dMode_nomal,dCMD_Start,"启动",sizeof("启动")-1);
	
	
	//检查eeprom是否格式化了
	eeprom_read(&result,SMS_FORMAT_FLAGE_ADDR,1);
	if (result!=0xAA)
	{
		LCD_Write_Str(0,0,"格式化eeprom!");
		result=0xAA;
		eeprom_write(&result,SMS_FORMAT_FLAGE_ADDR,1);
		SMS_Format();
		
		 for (i=0;i<SMS_MAX;i++)
		{
			SMS_Save("标题哈花生壳","标题下的科科技大沙发上即可垃圾时间上来就暗示大家法拉烈火继续可化快递首付款");
		}
		
	}

	
	///-----------------for test
	Dispay_LOGO();
	printf("系统启动成功\r\n"); 
		
     
	//LCD_Write_Str(1,0,"LCD TTS Demo!"); 
		SetMainPage(&mainPage);
		 ShowPage(pPage);
		 
		/* pPage->Function(KEY_UP);
		pPage->Function(KEY_UP);
		pPage->Function(KEY_Down);
		pPage->Function(KEY_Down);
		pPage->Function(KEY_Ok);
		pPage->Function(KEY_Ok);
		pPage->Function(KEY_Return); */
	//err=TTS_set_text(dMode_nomal,dCMD_Start,"系统正在启动",sizeof("系统正在启动")-1);
	
	PCPsrtuct.CMD=LCDCMD_VoiceAndText;
	PCPsrtuct.Company=0x0203;
	PCPsrtuct.EquipmentType=0x04;
	PCPsrtuct.userDataLen=10;
	PCPsrtuct.Version=0x4567;
	for(i=0;i<PCPsrtuct.userDataLen;i++)
	{
		PCPsrtuct.userData[i]=i+0x30;
	}
		
	PcpPackage(&PCPsrtuct);
	 
// 	 SMS_Format();
// 	 for (i=0;i<SMS_MAX;i++)
// 	{
// 		SMS_Save("标题哈花生壳","标题下的科科技大沙发上即可垃圾时间上来就暗示大家法拉烈火继续可化快递首付款");
// 	}
// 	for (i=0;i<SMS_MAX;i++)
// 	{
// 		SMS_Read_Title(i,tempData,SMS_TITLE_MAX_LEN);
// 		printf("index=%d\r\ntitle:%s\r\n",i,tempData);
// 		
// 		SMS_Read_Text(i,0,tempData,SMS_TEXT_MAX_LEN);
// 		printf("text:%s\r\n",tempData);
// 	}  
	
	

	
   while (1)
  {
			
	i= Key_Get_key();
	if (i!=0)
	{
		pPage->Function(i);
		printf("Kye=%x\r\n",i);
	}
	
		
// 	   if (TTS_get_state()==dTTS_state_Idle)
// 					{
// 						err=TTS_set_text(dMode_nomal,dCMD_Start,"你好",4);
// 					}  
		
		if(PcpStatus_Is(PcpStatus_ReceiveBuf_Full))
		{
			PcpUnPackage(&PCPsrtuct);
			
			switch(PCPsrtuct.CMD)
			{
				case LCDCMD_VoiceAndText:
					LCD_set_FlashCout(10);
					PCPsrtuct.userData[PCPsrtuct.userDataLen]=0; //add "\0",chang to string
					
					Lcd_Clr_Scr();
					LCD_Write_Screen(PCPsrtuct.userData);
					//LCD_Write_Str(2,0,PCPsrtuct.userData);
					printf("%s\n",PCPsrtuct.userData);
					 if (TTS_is_idel())
					{
						printf("开始发送TTS数据\r\n");
						err=TTS_set_text(dMode_nomal,dCMD_Start,PCPsrtuct.userData,PCPsrtuct.userDataLen);
						//err=TTS_set_text(dMode_nomal,dCMD_Start,"您有一条新信息！",sizeof("您有一条新信息！")-1);
						
					} 
					else
					{
						printf("TTS忙\r\n");
					}
					//回复应答
					PCPsrtuct.userDataLen=0;
					PcpPackage(&PCPsrtuct);
					break;
			}
			
			for (i=0;i<PCPsrtuct.userDataLen;i++)
			{
				printf("%x ",PCPsrtuct.userData[i]);
			}
						
			//PcpPackage(&PCPsrtuct);
			

		}
  }
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
