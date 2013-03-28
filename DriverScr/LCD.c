#include "LCD.h"

u8 ledFalshCouter=0;

/**
*lcd hard ware init
*/
void LCD_Hardware_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*init lcd Backlight hardware*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	//led gpio as output
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//SCK SDI gpio as output
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);
	  
	  
}

#define dCNT	50
void LCD_ledFlash(void)
{
	u8 static cnt=0;
	cnt++;
	if (cnt<dCNT) return;
	cnt=0;

	if (ledFalshCouter)
	{
		ledFalshCouter--;
		if(ledFalshCouter%2) LCD_backlight_on;
		else LCD_backlight_off;
	}
	else
	{
		LCD_backlight_on;
	}
}

void LCD_set_FlashCout(u8 cnt)
{
	ledFalshCouter=cnt;
}
/**
*send one byte to LCD
*/
static void LCD_Write_Byte(unsigned char byte)
{
    unsigned char i;
//		NSS_H;
    CLK_L;
	delayUs(1);
    for(i=0; i<8; i++)
    {
        CLK_L;
			delayUs(1);
        if((byte<<i)&0x80)    //先传高位，再传低位
        {
            SDI_H;
        }
        else
        {
            SDI_L;
        }
				delayUs(1);
        CLK_H;
				delayUs(1);
    }
//		NSS_L;
}

/**
*write a CMD to LCD
*/
static void LCD_Write_Cmd(unsigned char cmd)
{
    LCD_Write_Byte(0xf8);
    LCD_Write_Byte(cmd&0xf0);
    LCD_Write_Byte((cmd<<4)&0xf0);
}

/**
*write one byte data to LCD
*/
static void LCD_Write_Data(unsigned char dat)
{
    LCD_Write_Byte(0xfa);
    LCD_Write_Byte(dat&0xf0);
    LCD_Write_Byte((dat<<4)&0xf0);
}

/**
*init LCD base fucton
*/
void LCD_init(void)
{
    LCD_Write_Cmd(0x30);  //  功能设定 基本指令集
    LCD_Write_Cmd(0x0C);  //  显示设定 开关
}

/**
*set Cursor
*@param	X LCD line 0~3 
*@param Y LCD column 0~7
@note
- 更改X坐标序号，从1~3 修正为 0~3  2012.7.4   lindabell@欧海
*/
void LCD_Set_XY(u8 X,u8 Y)
{
 u8 address;
 switch(X)
 {    
  case 0: 
		address = 0x80 + Y; 
		break; 
  case 1: 
		address = 0x90 + Y; 
		break; 
  case 2: 
       address = 0x88 + Y; 
       break;
  case 3: 
		address = 0x98 + Y; 
		break; 
  default:
		address = 0x80 + Y; 
		break;
 }
    LCD_Write_Cmd(address);
}
/**
*write a string to LCD 
*@param X inline 1~3
*@param Y column 0~8
*/
void LCD_Write_Str(u8 line,u8 column,const u8 *pStr)
{
	LCD_Set_XY( line, column );
    while (*pStr) 
    {
  LCD_Write_Data(*pStr );
     pStr ++;			
	}
}

/**
写整屏

从液晶的第0行 第0列 开始直到液晶最后或字符串结束
@return 返回写多少个字节到液晶屏
*/
u8 LCD_Write_Screen(u8 *pStr)
{
	u8 i=0;
	LCD_Set_XY(0,0);
	while (*pStr)
	{
		i++;
		if (i>64)//由于LCD最多能容纳128 个字节
		{
			return (i-1);
		}
		else if (i==49)
		{
			LCD_Set_XY(3,0);
		}
		else if (i==33)
		{
			LCD_Set_XY(2,0);
		}else if (i==17)  
		{
			LCD_Set_XY(1,0);
		}
		LCD_Write_Data(*pStr++);
	}
	
	return i;
}
/**
*clrear LCD ,there maybe need 10ms to finish
*/
void Lcd_Clr_Scr(void)//清屏函数
{
 LCD_Write_Cmd(0x01);
 delayUs(1000);

}

