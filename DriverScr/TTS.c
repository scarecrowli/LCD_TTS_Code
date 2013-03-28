/**
*** TTS diver code 
***Author:lindabell@欧海
***Date: 2012.5.10
*log 2012.6.11 修正由于大小端而引起的数据不对 
***********************************************************/
#include <TTS.h>
#include <USART3.h>

/**
* TTS common list
*/
const TTS_ring_listTypedef TTS_ring_list[dTTS_ring_num]=
{
	{"sounda",6}, 	//声音提示
	{"soundb",6},
	{"soundc",6},
	{"soundd",6},
	{"soundf",6},
	{"soundg",6},
	{"msga",4},	//和弦提示音
	{"msgb",4},
	{"msgc",4},
	{"msgd",4},
	{"msge",4},
	{"msgf",4},
	{"msgi",4},
	{"ringa",5},
	{"ringb",5},
	{"ringc",5},
	{"ringd",5},
	{"ringe",5},
	{"ringf",5},
	{"ringg",5},
	{"ringl",5},
	{"ringm",5},
	{"ringn",5},
	{"ringo",5},
	{"ringp",5},
	{"ringq",5},
	{"ringr",5},
	{"rings",5},
	{"ringt",5},
	{"ringu",5}
};

TTScmdTypedef TTScmd;

u8 ttsState=dTTS_state_Idle;

/**
* TTS hardware init
*/
static void TTS_hardware_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*init lcd Backlight hardware*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	//TTS TT_RDY# pin as input
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	TTS_DRIVER_INIT;
}

/**
*TTS struct init
*/
void TTS_init(void)
{
	TTS_hardware_init();
	
	TTScmd.tts_cmd.Len=0;

}

/**
*begin to send datas to TTS 
*/
static void TTS_start_send(void)
{
	//S2CON|=S2TI;	//触发串口2发送中断
	//USART3_TX_ENABLE;
	TTS_DRIVER_sart;
}


/**
*it was call at   RX series interrupt
*/
void TTS_state(u8 state)
{
	switch (state)
	{
	case dTTS_state_OK:
		ttsState=dTTS_state_OK;
		break;
	case dTTS_state_ERR:
		ttsState=dTTS_state_ERR;
		break;
	case dTTS_state_Busy:
		ttsState=dTTS_state_Busy;
		break;
	case dTTS_state_Idle:
		ttsState=dTTS_state_Idle;
		TTS_start_send(); //TTS chip at idle mode ,then send text again if no char to send will do nothing other will be send to tts chip  
		break;
	default:
		break;
	}
}
/**
*	get the last TTS state and clear the flage ,deail see "TTS_state()"
*/
u8 TTS_get_state(void)
{
	static u8 temp;
	if(temp==ttsState) return dTTS_state_Nochange;
	
	temp=ttsState;
	return temp;
}

/**
* check TTS is ready or not
*/
u8 TTS_is_idel(void)
{
	u8 temp;
	 temp=(readRDYpinState)==0?1:0;
	return temp;
}


/**
* return not 0 was the number of char will be sended,if 0 the last send not finish or
* the number of char beyond The max limit
*/
u8 TTS_set_text(u8 mode,u8 cmd,u8 *pStr,u8 len)
{
	u8 i;

	
	
 	if (TTScmd.tts_cmd.Len!=0) return 0; //the last send not finish
	if(len>199) return 0;
	
	TTScmd.tts_cmd.Header=0xFd;
	TTScmd.tts_cmd.Len_reserve=0;

	switch (cmd)
	{
		case dCMD_Start:
			TTScmd.tts_cmd.cmd=dCMD_Start;
			TTScmd.tts_cmd.Data[0]=dUsingCode;	// 
			for(i=0;i<len;i++)	
	{
		TTScmd.tts_cmd.Data[i+1]=*pStr++;
	}
			TTScmd.tts_cmd.Len=len+2;
			if(mode==dMode_nomal)
			{
				if(!TTS_is_idel()) return (TTScmd.tts_cmd.Len+3); 	//if tts busy The texts will be sended when tts chip return idel mode(0x4F) 
																	//other way will be sended Immediately
			}
			break;
		case dCMD_Stop:
			TTScmd.tts_cmd.cmd=dCMD_Stop;
			TTScmd.tts_cmd.Len=1;
			break;
		case dCMD_Pause:
			TTScmd.tts_cmd.cmd=dCMD_Pause;
			TTScmd.tts_cmd.Len=1;
			break;
		case dCMD_Resume:
			TTScmd.tts_cmd.cmd=dCMD_Resume;
			TTScmd.tts_cmd.Len=1;
			break;
		case dCMD_Status:
			TTScmd.tts_cmd.cmd=dCMD_Status;
			TTScmd.tts_cmd.Len=1;
			break;
		case dCMD_PowerDown:
			TTScmd.tts_cmd.cmd=dCMD_PowerDown;
			TTScmd.tts_cmd.Len=1;
			break;
			
	default: return 0;
	}
	
	TTS_start_send();
	return (TTScmd.tts_cmd.Len+3); 
}

void TTS_ring(const u8 Value)
{
	TTS_set_text(dMode_Immediately,dCMD_Start,TTS_ring_list[Value].pName,TTS_ring_list[Value].len);
}

/**
*set the Volume of TTS output
*@param Value:0~15 level default 8
*/
void TTS_set_Volume(u8 Value) 
{
	u8 temp[5];
	temp[0]='[';
	temp[1]='v';
	if (Value>=10)
	{
		temp[2]='0'+Value/10;
		temp[3]='0'+Value%10;
		temp[4]=']';
	}
	else
	{
		temp[2]='0'+Value;
		temp[3]=']';
	}
	TTS_set_text(dMode_Immediately,dCMD_Start,temp,Value>=10?5:4);
}
/** 
*the funtion will return char will be send 
*@return -1 there no char to send other is the data which need to send to TTS
*/
s16 TTS_get_char(void)
{
	static u8 index=0;
	static u8 len=0;
	
	if(TTScmd.tts_cmd.Len==0) return -1;
	
	if(len==0)
	{
		len=TTScmd.tts_cmd.Len+3;
		index=0;
	}
	else
	{
		len--;
		index++;
		
		if(len==0) 
		{
			TTScmd.tts_cmd.Len=0;
			return -1;
		}
		
	}
	
	return TTScmd.buf[index];
}

