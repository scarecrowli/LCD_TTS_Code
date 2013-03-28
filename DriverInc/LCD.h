#ifndef _LCD_H_BAB
#define _LCD_H_BAB

#include "stm32f10x.h"

/*12864控制位*/
//#define NSS_H       GPIO_SetBits(GPIOA,GPIO_Pin_4)		///< NSS set 
//#define NSS_L       GPIO_ResetBits(GPIOA,GPIO_Pin_4)	///< NSS reset
#define CLK_H       GPIO_SetBits(GPIOA,GPIO_Pin_5)		///< CLK set 
#define CLK_L       GPIO_ResetBits(GPIOA,GPIO_Pin_5)	///< CLK reset
#define SDI_H       GPIO_SetBits(GPIOA,GPIO_Pin_7)		///< SDI set
#define SDI_L       GPIO_ResetBits(GPIOA,GPIO_Pin_7)	///< SDI reset

#define LCD_backlight_on 	 GPIO_ResetBits(GPIOB,GPIO_Pin_1)
#define LCD_backlight_off 	 GPIO_SetBits(GPIOB,GPIO_Pin_1)


/// delay x uS at 72MHz 
#define delayUs(x) { unsigned int _dcnt; \
      _dcnt=(x*16); \
      while(_dcnt-- > 0) \
      { continue; }\
     }
		 
#define LCD_BacklightON			GPIO_ResetBits(GPIOB,GPIO_Pin_1)		///< LCD backlight ON
#define LCD_BacklightOFF  		GPIO_SetBits(GPIOB,GPIO_Pin_1)		///< LCD backlight OFF

#define LCD_MAX_BYTE_OF_SCREEN  64    ///<LCD 一屏的缓冲区最大值，即显示缓冲区大小

void LCD_Hardware_Init(void);
void LCD_init(void);
void Lcd_Clr_Scr(void);
void LCD_Set_XY(u8 X,u8 Y);
void LCD_Write_Str(u8 line,u8 column,const u8 *pStr);
u8 LCD_Write_Screen(u8 *pStr);

 void LCD_ledFlash(void);
 void LCD_set_FlashCout(u8 cnt);

#endif

