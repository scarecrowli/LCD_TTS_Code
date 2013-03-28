#include "USART1.h"
#include "stm32f10x.h"
#include <stdio.h>
void usart1_configure(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
//  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure the NVIC Preemption Priority Bits */  
 // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  /* Enable the USARTz Interrupt */
//   NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
//   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//   NVIC_Init(&NVIC_InitStructure);
	/* Enable GPIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA| RCC_APB2Periph_AFIO|RCC_APB2Periph_USART1, ENABLE);
	
	//Configures the different GPIO ports 
	/* Configure USARTy Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* Configure USARTy Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* USARTy and USARTz configuration ------------------------------------------------------*/
  /* USARTy and USARTz configured as follow:
        - BaudRate = 9600 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  /* Configure USART1 */
  USART_Init(USART1, &USART_InitStructure);
	
  

  /* Enable USART1 Receive and Transmit interrupts */
//  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
//  USART_ITConfig(USART1, USART_IT_TXE, ENABLE);

  /* Enable the USART2 */
  USART_Cmd(USART1, ENABLE);	
 	USART_ClearFlag(USART1,USART_FLAG_TC);
// 	USART_ClearFlag(USART1,USART_FLAG_TXE);
//   USART_ClearFlag(USART1,USART_FLAG_RXNE);

}


// void USART1_IRQHandler(void)
// {
// 	 unsigned  char temp;
// 	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
//   {
//     /* Read one byte from the receive data register and clear RXNE interrupt bit by read data in USART_D */
//     temp = USART_ReceiveData(USART1);
// 		USART_SendData(USART1,0xbb);
//   
//   }
//   
//   if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
//   {   
//     /* Write one byte to the transmit data register and clear TXE interrupt bit by wirte data in USART_DR*/
//     USART_SendData(USART1, 0xaa);

//    
//   }
// }


#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART1, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  {}

  return ch;
}

int fgetc(FILE *f)
{

    while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
    {}

    return (int)USART_ReceiveData(USART1);
}
