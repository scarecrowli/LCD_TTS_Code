/**
  ******************************************************************************
  * @file    USART/Printf/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"

/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup USART_Printf
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
#include "LED.h"
#include "BeatClock.h"
#include "Key.h"
#include "LCD.h"
void SysTick_Handler(void) 	//10MS
{
	static u8 couter=0;
	static u32 keyDelayCouter=0;
	
	//×´Ì¬µÆ
	couter++;
		if(couter>=50) LED_OFF;
 	  else     	LED_ON;
	if(couter>=100) couter=0;
	
	//Èí¼þÑÓÊ±
	if(SoftTimerDelay.couter!=0)
	{
			SoftTimerDelay.couter--;
			if (SoftTimerDelay.couter==0)
			{
				SoftTimerDelay.flage=1;
			}
	}
	
	//key can
	keyDelayCouter++;
	if(keyDelayCouter>=20)
	{
		keyDelayCouter=0;
		key_scan();
	}
	
	//LCD led flashing
	LCD_ledFlash();
	
}


#include <USART2.h>
#include "PCP.h"

void USART2_IRQHandler(void)
{
	u8 temp;
	if(USART_GetITStatus(USART2,USART_IT_TXE)!=RESET)
	{
		 
		if(PcpSendPackage(&temp)) USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
		USART_SendData(USART2, temp);
	}
	
	if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)
	{
		temp=USART_ReceiveData(USART2);
		PcpReceivePackage(temp);
		//if(temp=='A')USART_ITConfig(USART2, USART_IT_TXE, ENABLE);

	}
}

#include <USART3.h>
#include <TTS.h>

void USART3_IRQHandler(void)
{
	 s16 temp;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
  {
    /* Read one byte from the receive data register and clear RXNE interrupt bit by read data in USART_D */
		
		TTS_state(USART_ReceiveData(USART3));
  
  }
  
  if(USART_GetITStatus(USART3, USART_IT_TXE) != RESET)
  {   
    /* Write one byte to the transmit data register and clear TXE interrupt bit by wirte data in USART_DR*/
    //USART_SendData(USART3, 0xaa);
	
 	/* USART_SendData(USART3, string[i++]);
 	if(i==sizeof(string)-1) USART3_TX_DISABLE; */
	  
    temp=TTS_get_char();
		if(temp!=-1)USART_SendData(USART3, temp);
		else USART3_TX_DISABLE;  

   
  }
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
