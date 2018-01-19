/**
  ******************************************************************************
  * @file    wifi.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    12-April-2017
  * @brief   WIFI interface file.
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
#include "stm32_wifi.h"
#include <k_api.h>

/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//M0_WIFIObject_t    M0WifiObj;
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initialiaze the WIFI core
  * @param  None
  * @retval Operation status
  */
WIFI_Status_t WIFI_Init(void)
{
  WIFI_Status_t ret = WIFI_STATUS_ERROR;
//	UART_WIFI_Delay(2000);
	if(M0_WIFI_Init()==M0_WIFI_STATUS_OK)
		ret = WIFI_STATUS_OK;
  return ret;
}

/**
  * @brief  List a defined number of vailable access points
  * @param  APs : pointer to APs structure
  * @param  AP_MaxNbr : Max APs number to be listed
  * @retval Operation status
  */
WIFI_Status_t WIFI_ListAccessPoints(WIFI_APs_t *APs, uint8_t AP_MaxNbr)
{
	WIFI_Status_t ret = WIFI_STATUS_ERROR;
	M0_WIFI_APs_t M0WifiAPs;
	M0WifiAPs.nbr=AP_MaxNbr;
  uint8_t APCount;
  if(M0_WIFI_ListAccessPoints(&M0WifiAPs) == M0_WIFI_STATUS_OK)
  {
    if(M0WifiAPs.nbr > 0)
    {
      APs->count = MIN(M0WifiAPs.nbr, AP_MaxNbr);  
      for(APCount = 0; APCount < APs->count; APCount++)
      {
        APs->ap[APCount].Ecn = (WIFI_Ecn_t)M0WifiAPs.AP[APCount].Security;
        strncpy( (char *)APs->ap[APCount].SSID, (char *)M0WifiAPs.AP[APCount].SSID, MIN (WIFI_MAX_SSID_NAME, M0_WIFI_MAX_SSID_NAME_SIZE));    
        APs->ap[APCount].RSSI = M0WifiAPs.AP[APCount].RSSI;
        memcpy(APs->ap[APCount].MAC, M0WifiAPs.AP[APCount].MAC, 6);
				APs->ap[APCount].Channel=M0WifiAPs.AP[APCount].Channel;
      }
    }
    ret = WIFI_STATUS_OK;  
  }
  return ret;
}

/**
  * @brief  Join an Access Point
  * @param  SSID : SSID string
  * @param  Password : Password string
  * @param  ecn : Encryption type
  * @param  IP_Addr : Got IP Address
  * @param  IP_Mask : Network IP mask
  * @param  Gateway_Addr : Gateway IP address
  * @param  MAC : pointer to MAC Address
  * @retval Operation status
  */
WIFI_Status_t WIFI_Connect(char* const SSID,const char* Password,WIFI_Ecn_t ecn)
{
  WIFI_Status_t ret = WIFI_STATUS_ERROR;  
	uint8_t timeout=20;
	if(M0_WIFI_GetOperationMode()==M0_WIFI_MODE_AP)
		M0_WIFI_SetOperationMode(M0_WIFI_MODE_APSTA);
	if(M0_WIFI_IsConnected()==M0_WIFI_STATUS_OK)
		return WIFI_STATUS_OK;
  if(M0_WIFI_Connect(SSID, Password) == M0_WIFI_STATUS_OK)
		if(M0_WIFI_SaveAndReset()==M0_WIFI_STATUS_OK)
		{
			while(M0_WIFI_IsConnected()!=M0_WIFI_STATUS_OK)
			{
				if((timeout--)==0) 
					return ret;
				UART_WIFI_Delay(1000);
			}
			ret=WIFI_STATUS_OK;
		}
  return ret;
}

/**
  * @brief  This function retrieves the WiFi interface's MAC address.
  * @retval Operation Status.
  */
WIFI_Status_t WIFI_GetMAC_Address(uint8_t  *mac)
{
  WIFI_Status_t ret = WIFI_STATUS_ERROR; 
  if(M0_WIFI_GetMACAddress(mac) == M0_WIFI_STATUS_OK)
    ret = WIFI_STATUS_OK;
  return ret;
}

/**
  * @brief  This function retrieves the WiFi interface's IP address.
  * @retval Operation Status.
  */
WIFI_Status_t WIFI_GetIP_Address (uint8_t  *const ipaddr)
{
	uint8_t ip[4];
	WIFI_Status_t ret = WIFI_STATUS_ERROR; 
  if(M0_WIFI_GetIPAddress(ip) == M0_WIFI_STATUS_OK)
    ret = WIFI_STATUS_OK;
  return ret;
}

/**
  * @brief  Disconnect from a network
  * @param  None
  * @retval Operation status
  */
WIFI_Status_t WIFI_Disconnect(void)
{
  WIFI_Status_t ret = WIFI_STATUS_ERROR;
  krhino_sched_disable();
  if( M0_WIFI_Connect("0","0")== M0_WIFI_STATUS_OK)
		if(M0_WIFI_SaveAndReset()==M0_WIFI_STATUS_OK)
		{
			UART_WIFI_Delay(2000);
      ret = WIFI_STATUS_OK; 
		}
  krhino_sched_enable();
  return ret;
}

/**
  * @brief  Configure an Access Point

  * @param  ssid : SSID string
  * @param  pass : Password string
  * @param  channel : channel number
  * @retval Operation status
  */
WIFI_Status_t WIFI_ConfigureAP(uint8_t *ssid, uint8_t *pass, uint8_t channel)
{
  WIFI_Status_t ret = WIFI_STATUS_ERROR;  
  M0_WIFI_APConfig_t ApConfig;
  strncpy((char*)ApConfig.SSID, (char*)ssid, M0_WIFI_MAX_SSID_NAME_SIZE);
  strncpy((char*)ApConfig.Pass, (char*)pass, M0_WIFI_MAX_PSWD_NAME_SIZE);
  ApConfig.Channel = channel;
  krhino_sched_disable();
	if(M0_WIFI_GetOperationMode()!=M0_WIFI_MODE_AP)
		M0_WIFI_SetOperationMode(M0_WIFI_MODE_APSTA);
  if(M0_WIFI_ActivateAP(&ApConfig) == M0_WIFI_STATUS_OK)
		if(M0_WIFI_SaveAndReset()==M0_WIFI_STATUS_OK)
      ret = WIFI_STATUS_OK; 
  krhino_sched_enable();
  return ret;
}

/**
  * @brief  Handle the background events of the wifi module

  * @retval None
*/
WIFI_Status_t WIFI_HandleAPEvents(WIFI_APSettings_t *setting)
{
  return WIFI_STATUS_NOT_SUPPORTED;
}

/**
  * @brief  Ping an IP address in the network
  * @param  ipaddr : array of the IP address
  * @retval Operation status
  */
WIFI_Status_t WIFI_Ping(uint8_t* ipaddr, uint16_t count, uint16_t interval_ms)
{
	WIFI_Status_t ret = WIFI_STATUS_ERROR;
	if(M0_WIFI_Ping(ipaddr)==M0_WIFI_STATUS_OK)
		ret=WIFI_STATUS_OK;
  return ret;
}

/**
  * @brief  Get IP address from URL using DNS
  * @param  location : Host URL
  * @param  ipaddr : array of the IP address
  * @retval Operation status
  */
WIFI_Status_t WIFI_GetHostAddress( char* location, uint8_t* ipaddr)
{
	WIFI_Status_t ret = WIFI_STATUS_ERROR;
	if(M0_WIFI_DNS_resolution(location,ipaddr)==M0_WIFI_STATUS_OK)
		ret=WIFI_STATUS_OK;
  return ret;
}

/**
  * @brief  Configure and start a client connection
  * @param  type : Connection type TCP/UDP
  * @param  name : name of the connection
  * @param  ipaddr : Client IP address
  * @param  port : Remote port
  * @param  local_port : Local port
  * @retval Operation status
  */
WIFI_Status_t WIFI_OpenClientConnection(uint32_t socket, WIFI_Protocol_t type, const char* name, uint8_t* ipaddr, uint16_t port, uint16_t local_port)
{
  WIFI_Status_t ret = WIFI_STATUS_ERROR;
  M0_WIFI_Protocol_t M0_type;
	uint8_t timeout=40;
  M0_type = (type == WIFI_TCP_PROTOCOL)? M0_WIFI_TCP_PROTOCOL : M0_WIFI_UDP_PROTOCOL;
  krhino_sched_disable();
  if(M0_WIFI_StartClientConnection(socket,M0_type,name,ipaddr,port)== M0_WIFI_STATUS_OK)
		if(M0_WIFI_SaveAndReset()==M0_WIFI_STATUS_OK)
				while(M0_WIFI_TCP_IsConnected()!=M0_WIFI_STATUS_OK)
			{
				if((timeout--)==0) 
					return ret;
				UART_WIFI_Delay(1000);
			}
			ret = WIFI_STATUS_OK;
  krhino_sched_enable();
  return ret;
}

/**
  * @brief  Close client connection
  * @param  socket 
  * @retval Operation status
  */
WIFI_Status_t WIFI_CloseClientConnection(uint32_t socket)
{
  WIFI_Status_t ret = WIFI_STATUS_ERROR;
  krhino_sched_disable();
  if(M0_WIFI_StopConn(socket)== M0_WIFI_STATUS_OK)
			ret = WIFI_STATUS_OK;
  krhino_sched_enable();
  return ret;
}

/**
  * @brief  Configure and start a Server
  * @param  type : Connection type TCP/UDP
  * @param  name : name of the connection
  * @param  port : Remote port
  * @retval Operation status
  */
WIFI_Status_t WIFI_StartServer(uint32_t socket, WIFI_Protocol_t protocol, const char* name, uint16_t port)
{
  WIFI_Status_t ret = WIFI_STATUS_ERROR;
	M0_WIFI_Protocol_t M0Protocol;
	M0Protocol=(protocol==WIFI_TCP_PROTOCOL)? M0_WIFI_TCP_PROTOCOL : M0_WIFI_UDP_PROTOCOL;
  krhino_sched_disable();
  if(M0_WIFI_StartServerConn(socket,M0Protocol,name,port)== M0_WIFI_STATUS_OK)
		if(M0_WIFI_SaveAndReset()==M0_WIFI_STATUS_OK)
			ret = WIFI_STATUS_OK;
  krhino_sched_enable();
  return ret;
}

/**
  * @brief  Stop a server
  * @retval Operation status
  */
WIFI_Status_t WIFI_StopServer(uint32_t socket)
{
  WIFI_Status_t ret = WIFI_STATUS_ERROR;
  krhino_sched_disable();
  if(M0_WIFI_StopConn(socket)== M0_WIFI_STATUS_OK)
		if(M0_WIFI_SaveAndReset()==M0_WIFI_STATUS_OK)
			ret = WIFI_STATUS_OK;
  krhino_sched_enable();
  return ret;
}

/**
  * @brief  Send Data on a socket
  * @param  pdata : pointer to data to be sent
  * @param  len : length of data to be sent
  * @retval Operation status
  */
WIFI_Status_t WIFI_SendData(uint8_t socket, uint8_t *pdata, uint16_t Reqlen, uint16_t *SentDatalen, uint32_t Timeout)
{
  WIFI_Status_t ret = WIFI_STATUS_ERROR;
	uint16_t Sendsize=0;
  krhino_sched_disable();
  if(M0_WIFI_SendData(socket, pdata, Reqlen,&Sendsize, Timeout) == M0_WIFI_STATUS_OK)
	{
		*SentDatalen=Sendsize;
    ret = WIFI_STATUS_OK;
	}
	else if(M0_WIFI_SendData(socket, pdata, Reqlen,&Sendsize, Timeout) == M0_WIFI_STATUS_OK)
	{
		*SentDatalen=Sendsize;
    ret = WIFI_STATUS_OK;
	}
  krhino_sched_enable();
  return ret;
}

/**
  * @brief  Receive Data from a socket
  * @param  pdata : pointer to Rx buffer
  * @param  *len :  pointer to length of data
  * @retval Operation status
  */
WIFI_Status_t WIFI_ReceiveData(uint8_t socket, uint8_t *pdata, uint16_t Reqlen, uint16_t *RcvDatalen, uint32_t Timeout)
{
  WIFI_Status_t ret = WIFI_STATUS_ERROR;
	uint16_t Rcvsize=0;
	*RcvDatalen=0;
	krhino_sched_disable();	
   if(M0_WIFI_ReceiveData(socket, pdata, Reqlen,&Rcvsize, Timeout) == M0_WIFI_STATUS_OK)
	{
		*RcvDatalen=Rcvsize;
    ret = WIFI_STATUS_OK; 
	}
	else if(M0_WIFI_ReceiveData(socket, pdata, Reqlen,&Rcvsize, Timeout) == M0_WIFI_STATUS_OK)
	{
		*RcvDatalen=Rcvsize;
    ret = WIFI_STATUS_OK; 
	}
	krhino_sched_enable();
  return ret;
}

/**
  * @brief  Customize module data
  * @param  name : MFC name
  * @param  Mac :  Mac Address
  * @retval Operation status
  */
WIFI_Status_t WIFI_SetOEMProperties(const char *name, uint8_t *Mac)
{
  return WIFI_STATUS_NOT_SUPPORTED;
}

/**
  * @brief  Reset the WIFI module
  * @retval Operation status
  */
WIFI_Status_t WIFI_ResetModule(void)
{
  WIFI_Status_t ret = WIFI_STATUS_ERROR; 
	krhino_sched_disable();
		if(M0_WIFI_SaveAndReset()==M0_WIFI_STATUS_OK)
			ret=WIFI_STATUS_OK;
  krhino_sched_enable();
  return ret;
}

/**
  * @brief  Restore module default configuration
  * @retval Operation status
  */
WIFI_Status_t WIFI_SetModuleDefault(void)
{
  WIFI_Status_t ret = WIFI_STATUS_ERROR; 
  krhino_sched_disable();
	if( M0_WIFI_ResetToFactoryDefault()==M0_WIFI_STATUS_OK)
		ret = WIFI_STATUS_OK;
  krhino_sched_enable();
  return ret;
}

/**
  * @brief  Update module firmware
  * @param  location : Binary Location IP address
  * @retval Operation status
  */
WIFI_Status_t WIFI_ModuleFirmwareUpdate(void)
{
 WIFI_Status_t ret = WIFI_STATUS_ERROR; 
	krhino_sched_disable();
	if( M0_WIFI_OTA_Upgrade()==M0_WIFI_STATUS_OK)
		ret = WIFI_STATUS_OK;
	krhino_sched_enable();
  return ret;
}

/**
  * @brief  Return Module firmware revision
  * @param  rev : revision string
  * @retval Operation status
  */
WIFI_Status_t WIFI_GetModuleFwRevision(char *rev)
{
  WIFI_Status_t ret = WIFI_STATUS_ERROR; 
	krhino_sched_disable();
	if( M0_WIFI_GetModuleFwRevision(rev)==M0_WIFI_STATUS_OK)
		ret = WIFI_STATUS_OK;
	krhino_sched_enable();
  return ret;
}

/**
  * @brief  Return Module ID
  * @param  Info : Module ID string
  * @retval Operation status
  */
WIFI_Status_t WIFI_GetModuleID(char *Id)
{
  return WIFI_STATUS_NOT_SUPPORTED;
}

/**
  * @brief  Return Module Name
  * @param  Info : Module Name
  * @retval Operation status
  */
WIFI_Status_t WIFI_GetModuleName(char *ModuleName)
{
	WIFI_Status_t ret = WIFI_STATUS_ERROR; 
	memcpy(ModuleName, "MYLINKS M0M100D0 WIFI Module",M0_WIFI_MAX_SSID_NAME_SIZE);
	ret = WIFI_STATUS_OK;
  return ret;
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

