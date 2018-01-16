/**
  ******************************************************************************
  * @file    m0_wifi_io.h
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    12-April-2017
  * @brief   This file contains the functions prototypes for es_wifi IO operations.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
#ifndef __WIFI_IO__
#define __WIFI_IO__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
#define USART3_REC_LEN 	 1300
//#define RXBUFFERSIZE   100
//#define CMD_BUFFER_LEN 100
																						 
/* Exported functions ------------------------------------------------------- */ 
void UART_WIFI_MspInit(UART_HandleTypeDef *huart);
int8_t UART_WIFI_Init(void);
int8_t UART_WIFI_DeInit(void);
int8_t UART_WIFI_Cmd ( char * cmd, char * reply1, char * reply2, uint32_t waittime );
int16_t UART_WIFI_ReceiveData(uint8_t Socket,uint8_t *pData, uint16_t len, uint32_t timeout);
int16_t UART_WIFI_SendData(uint8_t Socket,uint8_t *pdata,  uint16_t len);
void UART_WIFI_Delay(uint32_t Delay);
void USART3_RxCpltCallback(UART_HandleTypeDef *huart);
void USART3_TxCpltCallback(UART_HandleTypeDef *huart);
void UART_WIFI_printf (char *fmt,...);																						 
																						 
#ifdef __cplusplus
}
#endif

#endif /* __WIFI_IO__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/    
