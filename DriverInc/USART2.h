#ifndef _USART2_H_BAB
#define _USART2_H_BAB

#define USART2_RX_DISABLE 	USART_ITConfig(USART2, USART_IT_RXNE, DISABLE)	///<ʹ�ܴ���2�����ж�
#define USART2_RX_ENABLE 		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE)	///<��ʹ�ܴ���2�����ж�
#define USART2_TX_DISABLE 	USART_ITConfig(USART2, USART_IT_TXE, DISABLE)	///<ʹ�ܴ���2���Ϳ��ж�
#define USART2_TX_ENABLE 		USART_ITConfig(USART2, USART_IT_TXE, ENABLE)	///<��ʹ�ܴ���2���Ϳ��ж�

void usart2_configure(void);

#endif
