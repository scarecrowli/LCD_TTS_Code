#ifndef _TTS_H_BAB
#define _TTS_H_BAB

#include "stm32f10x.h"


#define readRDYpinState  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12) ///<TTS ready state ouput pin

//The texts send now or wait at iidel state and then send 
#define dMode_Immediately	0
#define dMode_nomal			1

#define dCMD_Start 	0x01
#define dCMD_Stop	0x02
#define dCMD_Pause	0x03
#define dCMD_Resume	0x04
#define dCMD_Status	0x21
#define dCMD_PowerDown	0x88

//Select the language code
#define dGB2312		0x00
#define dGBK		0x01
#define dBIG5		0x02
#define dUnicode	0x03

#define dUsingCode dGBK

//TTS return back state
#define dTTS_state_OK	0x41
#define dTTS_state_ERR	0x45
#define dTTS_state_Busy	0x4E
#define dTTS_state_Idle	0x4F 
#define dTTS_state_Nochange	0x00

#define  dTTS_ring_num 30

typedef struct 
{
	u8 *pName;
	u8 len;
}TTS_ring_listTypedef;
extern const TTS_ring_listTypedef TTS_ring_list[dTTS_ring_num]; ///<ring list  0~29
struct FrameFormat 
{
	u8 Header;
	u8 Len_reserve;  ///<²»»áongoingµÄ
	u8 Len;
	u8 cmd;
	u8 Data[200];
};
typedef union 
{
	struct FrameFormat tts_cmd;
	u8 buf[204];
	
}TTScmdTypedef;

#define TTS_DRIVER_INIT 	usart3_configure()
#define TTS_DRIVER_sart		USART3_TX_ENABLE 


void TTS_init(void);
void TTS_state(u8 state);
u8 TTS_get_state(void);
u8 TTS_set_text(u8 mode,u8 cmd,u8 *pStr,u8 len);
void TTS_ring(const u8 Value);
void TTS_set_Volume(u8 Value) ;
s16 TTS_get_char(void);
u8 TTS_is_idel(void);
#endif
