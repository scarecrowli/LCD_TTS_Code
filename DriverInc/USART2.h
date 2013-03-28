#ifndef _USART2_H_BAB
#define _USART2_H_BAB

#define USART2_RX_DISABLE 	USART_ITConfig(USART2, USART_IT_RXNE, DISABLE)	///<使能串口2接收中断
#define USART2_RX_ENABLE 		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE)	///<不使能串口2接收中断
#define USART2_TX_DISABLE 	USART_ITConfig(USART2, USART_IT_TXE, DISABLE)	///<使能串口2发送空中断
#define USART2_TX_ENABLE 		USART_ITConfig(USART2, USART_IT_TXE, ENABLE)	///<不使能串口2发送空中断

void usart2_configure(void);

#endif
