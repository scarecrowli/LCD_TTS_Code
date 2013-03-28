#ifndef _USART3_H_BAB
#define _USART3_H_BAB

#define USART3_RX_DISABLE 	USART_ITConfig(USART3, USART_IT_RXNE, DISABLE)	///<使能串口3接收中断
#define USART3_RX_ENABLE 		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE)	///<不使能串口3接收中断
#define USART3_TX_DISABLE 	USART_ITConfig(USART3, USART_IT_TXE, DISABLE)	///<使能串口3发送空中断
#define USART3_TX_ENABLE 		USART_ITConfig(USART3, USART_IT_TXE, ENABLE)	///<不使能串口3发送空中断

void usart3_configure(void);

#endif
