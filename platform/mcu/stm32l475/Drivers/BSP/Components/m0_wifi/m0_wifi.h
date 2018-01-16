/**
  ******************************************************************************
  * @file    M0-wifi.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-02-2017
  * @brief   header file for the m0-wifi module.
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __M0_WIFI_H
#define __M0_WIFI_H

#ifdef __cplusplus
 extern "C" {
#endif  

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "string.h"
#include "stdio.h"
#include "m0_wifi_conf.h"

/* Exported Constants --------------------------------------------------------*/
#define M0_WIFI_PAYLOAD_SIZE     1200
/* Exported macro-------------------------------------------------------------*/
#define MIN(a, b)  ((a) < (b) ? (a) : (b))


/* Exported typedef ----------------------------------------------------------*/
typedef enum {
  M0_WIFI_THROUGHPUTMODE          = 0,
  M0_WIFI_CMDMODE				 					= 1,  
}M0_WIFI_TransMode_t;

typedef enum {
  M0_WIFI_STATUS_OK             = 0,
  M0_WIFI_STATUS_ERROR 					= 1,  
  M0_WIFI_STATUS_TIMEOUT        = 2,
}M0_WIFI_Status_t;

typedef enum {
  M0_WIFI_MODE_SINGLE           = 0,
  M0_WIFI_MODE_MULTI            = 1,
}M0_WIFI_ConnMode_t;

typedef enum {
  M0_WIFI_MODE_AP         		  = 0,
  M0_WIFI_MODE_STA           	  = 1,
	M0_WIFI_MODE_APSTA            = 2,
}M0_WIFI_OperationMode_t;

typedef enum {
  M0_WIFI_TCP_CONNECTION        = 0,
  M0_WIFI_UDP_CONNECTION        = 1,
  M0_WIFI_UDP_LITE_CONNECTION   = 2,
  M0_WIFI_TCP_SSL_CONNECTION    = 3,    
  M0_WIFI_MQTT_CONNECTION       = 4,    
}M0_WIFI_ConnType_t;

typedef enum {
  M0_WIFI_TCP_PROTOCOL = 0,
  M0_WIFI_UDP_PROTOCOL = 1,
}M0_WIFI_Protocol_t;

/* Security settings for wifi network */
typedef enum {
  M0_WIFI_SEC_OPEN = 0x00,          /*!< Wifi is open */
  M0_WIFI_SEC_WEP  = 0x01,          /*!< Wired Equivalent Privacy option for wifi security. \note This mode can't be used when setting up ES_WIFI wifi */
  M0_WIFI_SEC_WPA_PSK  = 0x02,          /*!< Wi-Fi Protected Access */
  M0_WIFI_SEC_WPA2_PSK = 0x03,          /*!< Wi-Fi Protected Access 2 */
  M0_WIFI_SEC_WPA_WPA2_PSK= 0x04,       /*!< Wi-Fi Protected Access with both modes */
  M0_WIFI_SEC_WPA2_PSK_WPA_WPA2_PSK= 0x05,      /*!< Wi-Fi Protected Access with both modes */        
  M0_WIFI_SEC_UNKNOWN = 0xFF,       /*!< Wi-Fi Unknown Security mode */        
} M0_WIFI_SecurityType_t;

typedef enum {
  M0_WIFI_AP_NONE     = 0x00,   
  M0_WIFI_AP_JOINED   = 0x02,  
  M0_WIFI_AP_ERROR    = 0xFF,    
} M0_WIFI_APState_t;

typedef enum {
  M0_WIFI_STAIP_DHCP   = 0x00,   
  M0_WIFI_STAIP_static = 0x01,   
} M0_WIFI_STAIPmode_t;

typedef struct 
{
	M0_WIFI_STAIPmode_t mode;
  uint8_t IP[4];
	uint8_t	mask[4];
	uint8_t	gateway[4];
} M0_WIFI_STAparameter_t;

typedef struct {
  uint8_t SSID[M0_WIFI_MAX_SSID_NAME_SIZE + 1]; /*!< Service Set Identifier value.Wi-Fi spot name */
  M0_WIFI_SecurityType_t Security;         /*!< Security of Wi-Fi spot.  */ 
  int16_t RSSI;                            /*!< Signal strength of Wi-Fi spot */
  uint8_t MAC[6];                          /*!< MAC address of spot */
  uint8_t Channel;                         /*!< Wi-Fi channel */
} M0_WIFI_AP_t;

/* Access point configuration */
typedef struct {
  uint8_t SSID[M0_WIFI_MAX_SSID_NAME_SIZE + 1];  /*!< Network public name for ESP AP mode */
  uint8_t Pass[M0_WIFI_MAX_PSWD_NAME_SIZE + 1];  /*!< Network password for ESP AP mode */
	uint8_t Channel;                          /*!< Channel Wi-Fi is operating at */
} M0_WIFI_APConfig_t;


typedef struct {
  uint8_t SSID[M0_WIFI_MAX_SSID_NAME_SIZE + 1];  /*!< Network public name for ESP AP mode */  
  uint8_t IP_Addr[4];                       /*!< IP Address */
  uint8_t MAC_Addr[6];                      /*!< MAC address */  
} M0_WIFI_APSettings_t;

typedef struct {
  M0_WIFI_AP_t AP[M0_WIFI_MAX_DETECTED_AP];
  uint8_t nbr;
  
}M0_WIFI_APs_t;

typedef struct {
  uint8_t          SSID[M0_WIFI_MAX_SSID_NAME_SIZE + 1];
  uint8_t          pswd[M0_WIFI_MAX_PSWD_NAME_SIZE + 1];
  M0_WIFI_SecurityType_t Security;   
  uint8_t          DHCP_IsEnabled;
  uint8_t          JoinRetries;
  uint8_t          IsConnected; 
  uint8_t          AutoConnect;    
  uint8_t          IP_Addr[4]; 
  uint8_t          IP_Mask[4];  
  uint8_t          Gateway_Addr[4]; 
  uint8_t          DNS1[4];   
  uint8_t          DNS2[4]; 
} M0_WIFI_Network_t;
  
typedef struct {
  M0_WIFI_ConnType_t Type;     
  uint8_t            Number;             
  uint16_t           RemotePort;                  
  uint8_t            RemoteIP[4];         
} M0_WIFI_Conn_t;

typedef struct {    
  uint8_t          					 VER[M0_WIFI_VER_SIZE];                           
  M0_WIFI_OperationMode_t    Mode;  
  M0_WIFI_SecurityType_t		 Security;   
  M0_WIFI_Network_t					 NetSettings;
  M0_WIFI_APSettings_t       APSettings;
	M0_WIFI_STAparameter_t		 STAparameter;
}M0_WIFIObject_t;


/* Exported functions --------------------------------------------------------*/
M0_WIFI_Status_t M0_WIFI_Init(void);
M0_WIFI_Status_t M0_WIFI_ListAccessPoints(M0_WIFI_APs_t *APs);
M0_WIFI_Status_t M0_WIFI_Connect(const char* SSID,const char* Password);
M0_WIFI_Status_t M0_WIFI_IsConnected(void);
M0_WIFI_Status_t M0_WIFI_TCP_IsConnected(void);
M0_WIFI_Status_t M0_WIFI_ActivateAP(M0_WIFI_APConfig_t *ApConfig);
M0_WIFI_Status_t M0_WIFI_Ping(uint8_t* ipaddr);
M0_WIFI_Status_t M0_WIFI_DNS_resolution(char* location, uint8_t* ipaddr);
M0_WIFI_Status_t M0_WIFI_GetMACAddress(uint8_t *mac);
M0_WIFI_Status_t M0_WIFI_GetAPState(M0_WIFI_STAparameter_t *STAparameter);
M0_WIFI_Status_t M0_WIFI_GetIPAddress(uint8_t *const ipaddr);
M0_WIFI_Status_t M0_WIFI_ResetToFactoryDefault(void);
M0_WIFI_Status_t M0_WIFI_OTA_Upgrade(void);
M0_WIFI_Status_t M0_WIFI_StartClientConnection(uint32_t socket, M0_WIFI_Protocol_t type, const char* name,uint8_t* ipaddr, uint16_t port);
M0_WIFI_Status_t M0_WIFI_StartServerConn(uint32_t socket, M0_WIFI_Protocol_t protocol, const char* name, uint16_t port);
M0_WIFI_Status_t M0_WIFI_StopConn(uint32_t socket);
M0_WIFI_Status_t M0_WIFI_SendData(uint8_t Socket, uint8_t *pdata, uint16_t Reqlen , uint16_t *SentLen , uint32_t Timeout);
M0_WIFI_Status_t M0_WIFI_ReceiveData(uint8_t Socket, uint8_t *pdata, uint16_t Reqlen, uint16_t *Receivedlen, uint32_t Timeout);
M0_WIFI_Status_t M0_WIFI_GetModuleFwRevision(char *rev);
M0_WIFI_Status_t M0_WIFI_EnterCmdMode(void);
M0_WIFI_Status_t M0_WIFI_SaveAndReset(void);
M0_WIFI_OperationMode_t M0_WIFI_GetOperationMode(void);
M0_WIFI_Status_t M0_WIFI_SetOperationMode(M0_WIFI_OperationMode_t Mode);
M0_WIFI_Status_t M0_WIFI_SetTransMode(M0_WIFI_TransMode_t Mode);














#ifdef __cplusplus
}
#endif
#endif /*__ES_WIFI_H*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/ 
