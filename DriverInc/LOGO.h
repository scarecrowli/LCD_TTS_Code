#ifndef _LOGO_H_BAB
#define _LOGO_H_BAB

#include "stm32f10x.h"

#define COMPANY_INFO "Company:烟台兰鑫电子科技有限责任公司\r\n"
#define VERSION		"Version:LCD_TTS V0.5\r\n"
#define DATE(DATE) 		"Compiled:"##DATE"\r\n"
#define COMPILED_DATE	DATE(__DATE__)

void Dispay_LOGO(void);

#endif
