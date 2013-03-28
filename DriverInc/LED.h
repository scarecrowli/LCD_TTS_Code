#ifndef _LED_H_BAB
#define _LED_H_BAB

#define LED_OFF 	GPIO_SetBits(GPIOB,GPIO_Pin_0)
#define LED_ON 		GPIO_ResetBits(GPIOB,GPIO_Pin_0)

void led_configure(void);

#endif
