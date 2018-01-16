/**
  ******************************************************************************
  * @file    m0_wifi_io.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    12-April-2017
  * @brief   This file implments the IO operations to deal with the M0-wifi
  *          module. It mainly Inits and Deinits the SPI interface. Send and
  *          receive data over it.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "m0_wifi_io.h"
#include "string.h"
#include "aos/kernel.h"
#include "k_types.h"
#include "m0_wifi_conf.h"
#include "soc_init.h"
#include "stdarg.h"
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static aos_mutex_t uart3_tx_mutex;
static aos_mutex_t uart3_rx_mutex;
static aos_sem_t uart3_tx_sem;
static aos_sem_t uart3_rx_sem;

/* Private function prototypes -----------------------------------------------*/
UART_HandleTypeDef huart3;
uint8_t RxFinishFlg=0;
uint16_t RxbufferSize=0;
char USART_RX_BUF[USART3_REC_LEN];
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
                       COM Driver Interface (SPI)
*******************************************************************************/

/**
  * @brief  Initialize UART MSP
  * @param  hspi: UART handle
  * @retval None
  */
void UART_WIFI_MspInit(UART_HandleTypeDef *huart)
{
  
  GPIO_InitTypeDef GPIO_Init;
  
  __HAL_RCC_USART3_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  
	/* configure UART TX pin */
	GPIO_Init.Pin       = GPIO_PIN_10;
  GPIO_Init.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init.Pull      = GPIO_PULLUP;
  GPIO_Init.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_Init.Alternate = GPIO_AF7_USART3;

  HAL_GPIO_Init(GPIOC, &GPIO_Init);

  /* configure UART RX pin */
  GPIO_Init.Pin = GPIO_PIN_11;
  GPIO_Init.Alternate = GPIO_AF7_USART3;

  HAL_GPIO_Init(GPIOC, &GPIO_Init);
    
  /* Configure the NVIC for UART */
  HAL_NVIC_SetPriority(USART3_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(USART3_IRQn);
}

/**
  * @brief  Initialize the USART3
  * @param  None
  * @retval None
  */
int8_t UART_WIFI_Init(void)
{
  huart3.Instance               = USART3;
  UART_WIFI_MspInit(&huart3);
  
  huart3.Init.BaudRate   = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits   = UART_STOPBITS_1;
  huart3.Init.Parity     = UART_PARITY_NONE;
  huart3.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  huart3.Init.Mode       = UART_MODE_TX_RX;
  
	if(HAL_UART_DeInit(&huart3) != HAL_OK)
    return 0;
  if(HAL_UART_Init(&huart3) != HAL_OK)
    return 0;
  aos_mutex_new(&uart3_tx_mutex);
  aos_mutex_new(&uart3_rx_mutex);
  aos_sem_new(&uart3_tx_sem, 0);
  aos_sem_new(&uart3_rx_sem, 0);
  return 1;
}
/**
  * @brief  DeInitialize the UART
  * @param  None
  * @retval None
  */
int8_t UART_WIFI_DeInit(void)
{
  if(HAL_UART_DeInit(&huart3)!=HAL_OK)
		return 0;
  aos_mutex_free(&uart3_tx_mutex);
  aos_mutex_free(&uart3_rx_mutex);
  aos_sem_free(&uart3_tx_sem);
  aos_sem_free(&uart3_rx_sem);
  return 1;
}

/**
  * @brief  Send WIFI AT cmd from USART3
  * @param  cmd : pointer to AT cmd
  * @param  reply1 : Expected response1
  * @param  reply2 : Expected response2
  * @param  timeout : response timeout in mS
  * @retval None
  */
int8_t UART_WIFI_Cmd ( char * cmd, char * reply1, char * reply2, uint32_t waittime )
{    
	uint32_t time=0;
	UART_WIFI_printf("%s", cmd);
	RxFinishFlg=0;
	HAL_UART_Receive_IT(&huart3, (uint8_t *)USART_RX_BUF, USART3_REC_LEN);
	if ( ( reply1 == 0 ) && ( reply2 == 0 ) )                 
		return 1;
	while(RxFinishFlg!=1)
	{
		UART_WIFI_Delay(1);
		time++;
		if(time>8000)
			return 0;
	}
	if(USART_RX_BUF[0]==0x0d)
	{
		RxbufferSize--;
		memcpy(USART_RX_BUF,USART_RX_BUF+1,RxbufferSize);
	}
	USART_RX_BUF[RxbufferSize]  = '\0';
	if ( ( reply1 != 0 ) && ( reply2 != 0 ) ) 
		return ( ( bool ) strstr ( USART_RX_BUF, reply1 ) || 
						 ( bool ) strstr ( USART_RX_BUF, reply2 ) ); 
	else if ( reply1 != 0 )
		return ( ( bool ) strstr ( USART_RX_BUF, reply1 ) );
	else
		return ( ( bool ) strstr ( USART_RX_BUF, reply2 ) );
}
/**
  * @brief  Receive wifi Data from UART
  * @param  pdata : pointer to data
  * @param  len : Data length
  * @param  timeout : send timeout in mS
  * @retval Length of received data (payload)
  */
int16_t UART_WIFI_ReceiveData(uint8_t Socket,uint8_t *pData, uint16_t len, uint32_t timeout)
{
	char cCmd [50];
	uint8_t RecIndex=0;
	int16_t length = 0;
	aos_mutex_lock(&uart3_rx_mutex, RHINO_WAIT_FOREVER);
	sprintf ( cCmd,(Socket==0)? "AT+RECV=%d,%d\r\n":"AT+RCVB=%d,%d\r\n",len,timeout/1000+1);
	if( UART_WIFI_Cmd ( cCmd, "+ok", 0, timeout ) )
	{
		while((USART_RX_BUF[RecIndex+4]!=',')&&(USART_RX_BUF[RecIndex+4]!='\r'))
		{
			length=length*10+USART_RX_BUF[RecIndex+4]-48;
			RecIndex++;
			if(RecIndex>10)
			{
				aos_mutex_unlock(&uart3_rx_mutex);
				return -1;
			}
		}
		memcpy(pData,USART_RX_BUF+RecIndex+5,length);
		aos_mutex_unlock(&uart3_rx_mutex);
		return length;
	}
	aos_mutex_unlock(&uart3_rx_mutex);
	return -1;
}

/**
  * @brief  Send wifi Data thru UART
  * @param  pdata : pointer to data
  * @param  len : Data length
  * @param  timeout : send timeout in mS
  * @retval Length of sent data
  */
int16_t UART_WIFI_SendData(uint8_t Socket, uint8_t *pdata,  uint16_t len)//, uint32_t timeout
{
	char cCmd [50];
	uint16_t SendIndex=0;
	int16_t length = 0;
	uint32_t time=0;
	aos_mutex_lock(&uart3_tx_mutex, RHINO_WAIT_FOREVER);
	sprintf ( cCmd,(Socket==0)? "AT+SEND=%d\r\n":"AT+SNDB=%d\r\n",len);
	if( UART_WIFI_Cmd ( cCmd, ">", 0, 500 ) )
	{
		memcpy(USART_RX_BUF,pdata,len);
		RxFinishFlg=0;
		HAL_UART_Transmit (&huart3,(uint8_t *)&USART_RX_BUF, len, 500 );
		HAL_UART_Receive_IT(&huart3, (uint8_t *)USART_RX_BUF, USART3_REC_LEN);
		while(RxFinishFlg!=1)
		{
			UART_WIFI_Delay(1);
			time++;
			if(time>5000)
				return -1;
		}
		if(USART_RX_BUF[0]==0x0d)
			memcpy(USART_RX_BUF,USART_RX_BUF+1,RxbufferSize);
		USART_RX_BUF[RxbufferSize]  = '\0';
		while(USART_RX_BUF[SendIndex+4]!='\r')
		{
			length=length*10+USART_RX_BUF[SendIndex+4]-48;
			SendIndex++;
			if(SendIndex>10)
			{
				aos_mutex_unlock(&uart3_tx_mutex);
				return -1;
			}
		}
			aos_mutex_unlock(&uart3_tx_mutex);
			return length;
	}
	aos_mutex_unlock(&uart3_tx_mutex);
	return -1;
}

/**
  * @brief  Delay
  * @param  Delay in ms
  * @retval None
  */
void UART_WIFI_Delay(uint32_t Delay)
{
  HAL_Delay(Delay);
}

/**
  * @brief Rx Transfer completed callback.
  * @param huart: UART handle.
  * @retval None
  */
void USART3_RxCpltCallback(UART_HandleTypeDef *huart)
{
		aos_sem_signal(&uart3_rx_sem);
}

/**
  * @brief Tx Transfer completed callback.
  * @param huart: UART handle.
  * @retval None
  */
void USART3_TxCpltCallback(UART_HandleTypeDef *huart)
{
	aos_sem_signal(&uart3_tx_sem);
}


void HAL_UART_RxIdleCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART3)
	{
	__HAL_UART_DISABLE_IT(&huart3 ,UART_IT_IDLE);
	RxbufferSize=huart3.RxXferSize-huart3.RxXferCount;
	HAL_UART_AbortReceive(&huart3);
		RxFinishFlg=1;
		if(RxbufferSize>1200)
		{
		RxbufferSize=RxbufferSize;
		}
	}
}
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
void UART_WIFI_printf (char *fmt,...)
{
	uint16_t i = 0;
	va_list arg_ptr;
	va_start(arg_ptr, fmt); 
	vsnprintf(USART_RX_BUF, USART3_REC_LEN+1, fmt, arg_ptr);
	while ((i <(uint16_t)USART3_REC_LEN) && USART_RX_BUF[i])
	{
		if (HAL_UART_Transmit(&huart3,(uint8_t *) &USART_RX_BUF[i++],1,5000)!= HAL_OK)
			Error_Handler(); 
	}
	va_end(arg_ptr);
}



/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */

/**
  * @}
  */ 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
