#ifndef _USART3_H_BAB
#define _USART3_H_BAB

#define USART3_RX_DISABLE 	USART_ITConfig(USART3, USART_IT_RXNE, DISABLE)	///<ʹ�ܴ���3�����ж�
#define USART3_RX_ENABLE 		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE)	///<��ʹ�ܴ���3�����ж�
#define USART3_TX_DISABLE 	USART_ITConfig(USART3, USART_IT_TXE, DISABLE)	///<ʹ�ܴ���3���Ϳ��ж�
#define USART3_TX_ENABLE 		USART_ITConfig(USART3, USART_IT_TXE, ENABLE)	///<��ʹ�ܴ���3���Ϳ��ж�

void usart3_configure(void);

#endif
