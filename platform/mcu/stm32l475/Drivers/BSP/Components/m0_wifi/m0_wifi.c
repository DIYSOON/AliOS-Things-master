/**
  ******************************************************************************
  * @file    m0-wifi.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-02-2017
  * @brief   This file provides a set of functions to manage the M0-wifi module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright © 2017 STMicroelectronics International N.V. 
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
#include "m0_wifi.h"
#include "m0_wifi_io.h"

#define CHARISHEXNUM(x)                 (((x) >= '0' && (x) <= '9') || \
                                         ((x) >= 'a' && (x) <= 'f') || \
                                         ((x) >= 'A' && (x) <= 'F'))

#define CHARISNUM(x)                    ((x) >= '0' && (x) <= '9')
#define CHAR2NUM(x)                     ((x) - '0')
/* Private function prototypes -----------------------------------------------*/
extern char USART_RX_BUF[];
extern uint8_t RxbufferSize;
static  uint8_t Hex2Num(char a);
static uint32_t ParseHexNumber(char* ptr, uint8_t* cnt);
static uint32_t ParseHexNumber(char* ptr, uint8_t* cnt);
static void ParseMAC(char* ptr, uint8_t* arr);
static void ParseIP(char* ptr, uint8_t* arr);
static M0_WIFI_Status_t AT_ExecuteCommand(char* cmd, char *pdata);

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Convert char in Hex format to integer.
  * @param  a: character to convert
  * @retval integer value.
  */
static  uint8_t Hex2Num(char a) 
{
    if (a >= '0' && a <= '9') {                             /* Char is num */
        return a - '0';
    } else if (a >= 'a' && a <= 'f') {                      /* Char is lowercase character A - Z (hex) */
        return (a - 'a') + 10;
    } else if (a >= 'A' && a <= 'F') {                      /* Char is uppercase character A - Z (hex) */
        return (a - 'A') + 10;
    }
    
    return 0;
}

/**
  * @brief  Extract a hex number from a string.
  * @param  ptr: pointer to string
  * @param  cnt: pointer to the number of parsed digit
  * @retval Hex value.
  */
static uint32_t ParseHexNumber(char* ptr, uint8_t* cnt) 
{
    uint32_t sum = 0;
    uint8_t i = 0;
    
    while (CHARISHEXNUM(*ptr)) {                    		/* Parse number */
        sum <<= 4;
        sum += Hex2Num(*ptr);
        ptr++;
        i++;
    }
    
    if (cnt != NULL) {                               		/* Save number of characters used for number */
        *cnt = i;
    }
    return sum;                                        		/* Return number */
}

/**
  * @brief  Parses and returns number from string.
  * @param  ptr: pointer to string
  * @param  cnt: pointer to the number of parsed digit
  * @retval integer value.
  */
static int32_t ParseNumber(char* ptr, uint8_t* cnt) 
{
    uint8_t minus = 0, i = 0;
    int32_t sum = 0;
    
    if (*ptr == '-') {                                		/* Check for minus character */
        minus = 1;
        ptr++;
        i++;
    }
    while (CHARISNUM(*ptr)) {                        		/* Parse number */
        sum = 10 * sum + CHAR2NUM(*ptr);
        ptr++;
        i++;
    }
    if (cnt != NULL) {                                		/* Save number of characters used for number */
        *cnt = i;
    }
    if (minus) {                                    		/* Minus detected */
        return 0 - sum;
    }
    return sum;                                       		/* Return number */
}

/**
  * @brief  Parses and returns MAC address.
  * @param  ptr: pointer to string
  * @param  arr: pointer to MAC array
  * @retval None.
  */
static void ParseMAC(char* ptr, uint8_t* arr) 
{
  uint8_t hexnum = 0, hexcnt;
  
  while(* ptr) { 
    hexcnt = 1;
    if(*ptr != ':')
    {
      arr[hexnum++] = ParseHexNumber(ptr, &hexcnt);
    }
    ptr = ptr + hexcnt;
  }
}

/**
  * @brief  Parses and returns IP address.
  * @param  ptr: pointer to string
  * @param  arr: pointer to IP array
  * @retval None.
  */
static void ParseIP(char* ptr, uint8_t* arr) 
{
  uint8_t hexnum = 0, hexcnt;
  
  while(* ptr) { 
    hexcnt = 1;
    if(*ptr != '.')
    {
      arr[hexnum++] = ParseNumber(ptr, &hexcnt);
    }
    ptr = ptr + hexcnt;
  }
}

/**
  * @brief  Execute AT command.
  * @param  Obj: pointer to module handle
  * @param  cmd: pointer to command string
  * @param  pdata: pointer to returned data
  * @retval Operation Status.
  */
static M0_WIFI_Status_t AT_ExecuteCommand(char* cmd, char *pdata)
{
	uint32_t timeout=5000;
	M0_WIFI_Status_t ret=M0_WIFI_STATUS_ERROR;
	if(strstr(cmd,"AT+WSCAN\r\n")||strstr(cmd,"AT+UPGRADE\r\n"))
		timeout=8000;
	if(strstr(cmd,"+++"))
	{
		if(UART_WIFI_Cmd(cmd,"a","a",timeout))
			ret=M0_WIFI_STATUS_OK;
	}
	else if(cmd[0]=='a')
	{
		if(UART_WIFI_Cmd(cmd, "+ok",0,timeout))
			ret=M0_WIFI_STATUS_OK;
	}
	else if(UART_WIFI_Cmd(cmd,"+ok",0,timeout))
	{
		ret=M0_WIFI_STATUS_OK;
		strncpy(pdata,USART_RX_BUF+4,RxbufferSize+1);
	}
	UART_WIFI_Delay(20);
  return ret;
}

/**
  * @brief  Initialize WIFI module.
  * @param  Obj: pointer to module handle
  * @retval Operation Status.
  */
M0_WIFI_Status_t  M0_WIFI_Init(void)
{
  M0_WIFI_Status_t ret = M0_WIFI_STATUS_ERROR;
  UART_WIFI_DeInit();
	UART_WIFI_Init();
	M0_WIFI_EnterCmdMode();
	if(M0_WIFI_SetTransMode(M0_WIFI_CMDMODE)==M0_WIFI_STATUS_OK)
		ret=M0_WIFI_STATUS_OK;
  return ret;
}

/**
  * @brief  List all detected APs.
  * @param  Obj: pointer to module handle
  * @param  APs: pointer Access points structure
  * @retval Operation Status.
  */
M0_WIFI_Status_t  M0_WIFI_ListAccessPoints(M0_WIFI_APs_t *APs)
{  
	char data[1500],macstr[18];
	char *pdata=data;
	uint8_t SSIDIndex=0,APIndex=0,StrIndex=0,MACIndex=0;
  M0_WIFI_Status_t ret=M0_WIFI_STATUS_ERROR;
	if(AT_ExecuteCommand("AT+WSCAN\r\n",data)==M0_WIFI_STATUS_OK)
	{
		while(pdata[0]!=0x0A)
		{
			pdata++;
			StrIndex++;
			if(StrIndex>50) return ret;
		}
		pdata++;
		for(SSIDIndex=0,StrIndex=0;SSIDIndex<M0_WIFI_MAX_DETECTED_AP;SSIDIndex++)
		{
			while(pdata[APIndex]!=',')
			{
				APs->AP[SSIDIndex].Channel=APs->AP[SSIDIndex].Channel*10+pdata[APIndex]-48;
				APIndex++;
				if(APIndex>5) 
					return ret;
			}
			APIndex++;
			while(pdata[APIndex]!=',')
			{
				APs->AP[SSIDIndex].SSID[StrIndex++]=pdata[APIndex];
				APIndex++;
			}
			APIndex++;
			memcpy(macstr,pdata+APIndex,17);
			macstr[17]='\0';
			ParseMAC(macstr,&APs->AP[SSIDIndex].MAC[MACIndex]);
			APIndex+=18;
			if(strstr(pdata+APIndex,"WPA2_PSK_WPA_WPA2_PSK"))
				APs->AP[SSIDIndex].Security=M0_WIFI_SEC_WPA2_PSK_WPA_WPA2_PSK;
			else if(strstr(pdata+APIndex,"WPA_WPA2_PSK"))
				APs->AP[SSIDIndex].Security=M0_WIFI_SEC_WPA_WPA2_PSK;
			else if(strstr(pdata+APIndex,"WPA2_PSK"))
				APs->AP[SSIDIndex].Security=M0_WIFI_SEC_WPA2_PSK;
			else if(strstr(pdata+APIndex,"WPA_PSK"))
				APs->AP[SSIDIndex].Security=M0_WIFI_SEC_WPA_PSK;
			else if(strstr(pdata+APIndex,"WEP"))
				APs->AP[SSIDIndex].Security=M0_WIFI_SEC_WEP;
			else if(strstr(pdata+APIndex,"OPEN"))
				APs->AP[SSIDIndex].Security=M0_WIFI_SEC_OPEN;
			else
				APs->AP[SSIDIndex].Security=M0_WIFI_SEC_UNKNOWN;
			while(pdata[APIndex++]!=',');
			while(pdata[APIndex]!='\r')
			{
				APs->AP[SSIDIndex].RSSI=APs->AP[SSIDIndex].RSSI*10+pdata[APIndex]-48;
				APIndex++;
			}
			if(pdata[APIndex]=='\r')
				pdata+=APIndex+2;
			else return ret;
			APIndex=0;
			MACIndex=0;
			StrIndex=0;
			if(*pdata=='\0') 
			{
				APs->nbr=SSIDIndex+1;
				break;
			}
		}
	}
  ret = M0_WIFI_STATUS_OK;
  return ret;
}

/**
  * @brief  Join an Access point.
  * @param  Ssid: the access point id.
  * @param  Password: the Access point password.
  * @retval Operation Status.
  */
M0_WIFI_Status_t M0_WIFI_Connect(const char* SSID,const char* Password)
{
	char data[20];
	char cCmd [100];
  M0_WIFI_Status_t ret=M0_WIFI_STATUS_ERROR;
  sprintf(cCmd,"AT+WJAP=%s,%s\r\n",SSID,Password);
  ret = AT_ExecuteCommand(cCmd,data);
  return ret;
}

/**
  * @brief  Check whether the module is connected to an access point.
  * @retval Operation Status.
  */
M0_WIFI_Status_t M0_WIFI_IsConnected(void)
{
	char data[100];
	M0_WIFI_Status_t ret=M0_WIFI_STATUS_ERROR;
	if(AT_ExecuteCommand("AT+WSLK\r\n",data)==M0_WIFI_STATUS_OK)
		if(strstr(data,"Connected"))
			ret=M0_WIFI_STATUS_OK;
	return ret;
}

M0_WIFI_Status_t M0_WIFI_TCP_IsConnected(void)
{
	char data[100];
	M0_WIFI_Status_t ret=M0_WIFI_STATUS_ERROR;
	if(AT_ExecuteCommand("AT+TCPLK\r\n",data)==M0_WIFI_STATUS_OK)
		if(strstr(data,"on"))
			ret=M0_WIFI_STATUS_OK;
	return ret;
}

/**
  * @brief  Configure and activate SoftAP.
  * @param  Obj: pointer to module handle
  * @param  ApConfig : Pointer to AP config structure.
  * @param  ip : AP IP address
  * @param  mac : AP MAC address
  * @retval Operation Status.
  */
M0_WIFI_Status_t M0_WIFI_ActivateAP(M0_WIFI_APConfig_t *ApConfig)
{
	M0_WIFI_Status_t ret = M0_WIFI_STATUS_ERROR; 
	char data[20];
	char Ccmd[50]={0};
	sprintf(Ccmd,"AT+WSAP=%s,%s,CH%d\r\n",ApConfig->SSID,ApConfig->Pass,ApConfig->Channel);
	ret = AT_ExecuteCommand(Ccmd,data);
  return ret;
}

/**
  * @brief  Ping an IP address in the network
  * @param  ipaddr : array of the IP address
  * @retval Operation status
  */
M0_WIFI_Status_t M0_WIFI_Ping(uint8_t* ipaddr)
{
	M0_WIFI_Status_t ret = M0_WIFI_STATUS_ERROR;
	char data[20];
	char Ccmd[50]={0};
	uint8_t i,StrIndex=0;
	for(i=0;i<4;i++)
	{
		if(ipaddr[i]/100)
		{data[StrIndex]=ipaddr[i]/100;StrIndex++;}
		if(ipaddr[i]/10)
		{data[StrIndex]=ipaddr[i]/10%10;StrIndex++;}
		data[StrIndex]=ipaddr[i]%10;
		StrIndex++;
		if(i==3) data[StrIndex]='\0';
		else data[StrIndex]='.';
		StrIndex++;
	}
	sprintf(Ccmd,"AT+PING=%s\r\n",data);
	if(AT_ExecuteCommand(Ccmd,data)==M0_WIFI_STATUS_OK)
		if(strstr(data,"Success"))
			ret = M0_WIFI_STATUS_OK;
  return ret;
}

/**
  * @brief  Get IP address from URL using DNS
  * @param  location : Host URL
  * @param  ipaddr : array of the IP address
  * @retval Operation status
  */
M0_WIFI_Status_t M0_WIFI_DNS_resolution(char* location, uint8_t* ipaddr)
{
	M0_WIFI_Status_t ret = M0_WIFI_STATUS_ERROR;
	char data[20];
	char *pdata=data;
	char Ccmd[100]={0};
	uint8_t StrIndex=0,err=0;
	sprintf(Ccmd,"AT+DNS=%s\r\n",location);
	ret = AT_ExecuteCommand(Ccmd,data);
	while(pdata[StrIndex]!='\r')
	{
		StrIndex++;
		if(err>20) return M0_WIFI_STATUS_ERROR;
		err++;
	}
	pdata[StrIndex]='\0';
	ParseIP(pdata,ipaddr);
  return ret;
}

/**
  * @brief  retrn the MAC address of the M0 module.
  * @param  Obj: pointer to module handle
  * @param  mac: pointer to the MAC address array.
  * @retval Operation Status.
  */
M0_WIFI_Status_t M0_WIFI_GetMACAddress(uint8_t *mac)
{
	char data[20];
	uint8_t StrIndex=0;
  M0_WIFI_Status_t ret = M0_WIFI_STATUS_ERROR; 
  if(AT_ExecuteCommand("AT+WSMAC\r\n",data)==M0_WIFI_STATUS_OK)
	{
		for(StrIndex=0;StrIndex<6;StrIndex++)
			mac[StrIndex]=Hex2Num(data[StrIndex*2])*16+Hex2Num(data[StrIndex*2+1]);
		ret=M0_WIFI_STATUS_OK;
	}
	return ret;
}
/**
  * @brief  retrn the IP address of the M0 module.
  * @param  mac: pointer to the IP address array.
  * @retval Operation Status.
  */
M0_WIFI_Status_t M0_WIFI_GetAPState(M0_WIFI_STAparameter_t *STAparameter)
{
	char data[50];
	char *pdata=data;
	uint8_t StrIndex=0,err=0;
	char str[20]={0};
  M0_WIFI_Status_t ret = M0_WIFI_STATUS_ERROR; 
  if(AT_ExecuteCommand("AT+WANN\r\n",data)==M0_WIFI_STATUS_OK)
	{
		if(strstr(pdata,"DHCP"))
			STAparameter->mode=M0_WIFI_STAIP_DHCP;
		else if(strstr(pdata,"static"))
			STAparameter->mode=M0_WIFI_STAIP_static;
		while(*pdata++!=',')
		{
			if(err>20) return ret;
			err++;
		}
		err=0;
		while(pdata[StrIndex]!=',')
		{
			StrIndex++;
			if(err>20) return ret;
			err++;
		}
		err=0;
		strncpy(str,pdata,StrIndex);
		str[StrIndex]='\0';
		ParseIP(str,STAparameter->IP);
		pdata+=StrIndex+1;
		StrIndex=0;
		while(pdata[StrIndex]!=',')
		{
			StrIndex++;
			if(err>20) return ret;
			err++;
		}
		err=0;
		strncpy(str,pdata,StrIndex);
		str[StrIndex]='\0';
		ParseIP(str,STAparameter->mask);
		pdata+=StrIndex+1;
		strncpy(str,pdata,20);
		StrIndex=0;
		while(pdata[StrIndex]!='\r')
		{
			StrIndex++;
			if(err>20) return ret;
			err++;
		}
		str[StrIndex]='\0';
		ParseIP(str,STAparameter->gateway);
		ret=M0_WIFI_STATUS_OK;
	}
	return ret;
}

/**
  * @brief  retrn the IP address of the M0 module.
  * @param  Obj: pointer to module handle
  * @param  mac: pointer to the IP address array.
  * @retval Operation Status.
  */
M0_WIFI_Status_t M0_WIFI_GetIPAddress(uint8_t *const ipaddr)
{
	M0_WIFI_STAparameter_t STAparameter;
	M0_WIFI_Status_t ret = M0_WIFI_STATUS_ERROR; 
	if(M0_WIFI_GetAPState(&STAparameter)==M0_WIFI_STATUS_OK)
	{
		memcpy(ipaddr, STAparameter.IP, 4);         
		ret = M0_WIFI_STATUS_OK;
	}
  return ret;
}

/**
  * @brief  Reset To factory defaults.
  * @param  Obj: pointer to module handle
  * @retval Operation Status.
  */
M0_WIFI_Status_t M0_WIFI_ResetToFactoryDefault(void)
{
	char data[20];
	M0_WIFI_Status_t ret = M0_WIFI_STATUS_ERROR;
	ret=AT_ExecuteCommand("AT+RELD\r\n",data);
	UART_WIFI_Delay(2000);
	return ret;
}

/**
  * @brief  OTA Firmware Upgrade.
  * @param  Obj: pointer to module handle
  * @param  Upgrade link path
  * @retval Operation Status.
  */
M0_WIFI_Status_t M0_WIFI_OTA_Upgrade(void)
{
	char data[20];
	M0_WIFI_Status_t ret = M0_WIFI_STATUS_ERROR;
	ret=AT_ExecuteCommand("AT+UPGRADE\r\n",data);
	UART_WIFI_Delay(3000);
	return ret;
}

/**
  * @brief  Configure and Start a Client connection.
  * @param  Obj: pointer to module handle
  * @param  conn: pointer to the connection structure
  * @retval Operation Status.
  */
M0_WIFI_Status_t M0_WIFI_StartClientConnection(uint32_t socket, M0_WIFI_Protocol_t type,const char* name,uint8_t* ipaddr, uint16_t port)
{
	M0_WIFI_Status_t ret = M0_WIFI_STATUS_ERROR;
	char data[20];
	char cCmd [100];
	char *protocol_p,*CS;
	protocol_p=(type==M0_WIFI_TCP_PROTOCOL)? "TCP" : "UDP";
	CS="CLIENT";
	if(name[0])
		sprintf ( cCmd, ((socket==0)? "AT+NETP=%s,%s,%d,%s\r\n" : "AT+SOCKB=%s,%s,%d,%s\r\n"),protocol_p,CS,port,name);
	else
		sprintf ( cCmd, ((socket==0)? "AT+NETP=%s,%s,%d,%d.%d.%d.%d\r\n" : "AT+SOCKB=%s,%s,%d,%d.%d.%d.%d\r\n"),protocol_p,CS,port,ipaddr[0],ipaddr[1],ipaddr[2],ipaddr[3]);
	if(AT_ExecuteCommand(cCmd,data)==M0_WIFI_STATUS_OK)
		ret=M0_WIFI_STATUS_OK;
	return ret;
}

/**
  * @brief  Configure and Start a Server.
  * @param  Obj: pointer to module handle
  * @param  conn: pointer to the connection structure
  * @retval Operation Status.
  */
M0_WIFI_Status_t M0_WIFI_StartServerConn(uint32_t socket, M0_WIFI_Protocol_t protocol, const char* name, uint16_t port)
{
	M0_WIFI_Status_t ret = M0_WIFI_STATUS_ERROR;
	char data[20];
	char cCmd [100];
	char *protocol_p,*CS;
	protocol_p=(protocol==M0_WIFI_TCP_PROTOCOL)? "TCP" : "UDP";
	CS="SERVER";
	sprintf ( cCmd, (socket==0)? "AT+NETP=%s,%s,%d,%s\r\n" : "AT+SOCKB=%s,%s,%d,%s\r\n",protocol_p,CS,port,name);
	if(AT_ExecuteCommand(cCmd,data)==M0_WIFI_STATUS_OK)
			ret=M0_WIFI_STATUS_OK;
	return ret;
}

/**
  * @brief  Stop a Net Socket connect.
  * @param  Obj: pointer to module handle
  * @retval Operation Status.
  */
M0_WIFI_Status_t M0_WIFI_StopConn(uint32_t socket)
{
	char data[20];
	char cCmd [20];
	M0_WIFI_Status_t ret = M0_WIFI_STATUS_ERROR;
	sprintf(cCmd,((socket==0)? "AT+TCPDIS=off\r\n":"AT+TCPDISB=off\r\n"));
	ret=AT_ExecuteCommand(cCmd,data);
	return ret;
}

/**
  * @brief  Send an amount data over WIFI.
  * @param  Obj: pointer to module handle
  * @param  Socket: number of the socket
  * @param  pdata: pointer to data
  * @param  len : length of the data to be sent
  * @retval Operation Status.
  */
M0_WIFI_Status_t M0_WIFI_SendData(uint8_t Socket, uint8_t *pdata, uint16_t Reqlen , uint16_t *SentLen , uint32_t Timeout)
{
  M0_WIFI_Status_t ret = M0_WIFI_STATUS_ERROR; 
	int16_t Sendsize;	
  Sendsize=UART_WIFI_SendData(Socket,pdata,Reqlen);
	if(Sendsize!=-1)
	{
		*SentLen=Sendsize;
		ret = M0_WIFI_STATUS_OK;
	}
	return ret;
}

/**
  * @brief  Receive an amount data over WIFI.
  * @param  Obj: pointer to module handle
  * @param  Socket: number of the socket
  * @param  pdata: pointer to data
  * @param  len : pointer to the length of the data to be received
  * @retval Operation Status.
  */
M0_WIFI_Status_t M0_WIFI_ReceiveData(uint8_t Socket, uint8_t *pdata, uint16_t Reqlen, uint16_t *Receivedlen, uint32_t Timeout)
{
  M0_WIFI_Status_t ret = M0_WIFI_STATUS_ERROR;  
	int16_t Rcvsize;
  Rcvsize=UART_WIFI_ReceiveData(Socket,pdata,Reqlen,Timeout);
	if(Rcvsize!=-1)
	{
		*Receivedlen=Rcvsize;
		ret=M0_WIFI_STATUS_OK;
	}
  return ret;
}
/**
  * @brief  Return M0_WIFI Module firmware revision
  * @param  rev : revision string
  * @retval Operation status
  */
M0_WIFI_Status_t M0_WIFI_GetModuleFwRevision(char *rev)
{
	char data[20];
  M0_WIFI_Status_t ret = M0_WIFI_STATUS_ERROR; 
  if(AT_ExecuteCommand("AT+VER\r\n",data)==M0_WIFI_STATUS_OK)
	{
		strncpy(rev,data,10);
		ret=M0_WIFI_STATUS_OK;
	}
	return ret;
}

M0_WIFI_Status_t M0_WIFI_EnterCmdMode(void)
{
	char data[20];
	M0_WIFI_Status_t ret = M0_WIFI_STATUS_ERROR;
	if(AT_ExecuteCommand("+++",data)==M0_WIFI_STATUS_OK)
		ret=AT_ExecuteCommand("a",data);
	return ret;
}

M0_WIFI_Status_t M0_WIFI_SaveAndReset(void)
{
	char data[20];
	M0_WIFI_Status_t ret = M0_WIFI_STATUS_ERROR;
	ret=AT_ExecuteCommand("AT+Z\r\n",data);
	UART_WIFI_Delay(2000);
	return ret;
}

M0_WIFI_OperationMode_t M0_WIFI_GetOperationMode(void)
{
	char data[20];
	M0_WIFI_OperationMode_t Mode;
	if(AT_ExecuteCommand("AT+WMODE\r\n",data)==M0_WIFI_STATUS_OK)
	{
		if(strstr ( data, "APSTA" ))
			Mode=M0_WIFI_MODE_APSTA;
		else if(strstr ( data, "STA" ))
			Mode=M0_WIFI_MODE_STA;
		else if(strstr ( data, "AP" ))
			Mode=M0_WIFI_MODE_AP;
	}
	return Mode;
}

M0_WIFI_Status_t M0_WIFI_SetOperationMode(M0_WIFI_OperationMode_t Mode)
{
	char data[20];
	M0_WIFI_Status_t ret = M0_WIFI_STATUS_ERROR;
	switch(Mode)
	{
		case M0_WIFI_MODE_AP:
			ret=AT_ExecuteCommand("AT+WMODE=AP\r\n",data);
			break;
		case M0_WIFI_MODE_STA:
			ret=AT_ExecuteCommand("AT+WMODE=STA\r\n",data);
			break;
		case M0_WIFI_MODE_APSTA:
			ret=AT_ExecuteCommand("AT+WMODE=APSTA\r\n",data);
			break;
	}
	return ret;
}

M0_WIFI_Status_t M0_WIFI_SetTransMode(M0_WIFI_TransMode_t Mode)
{
	char data[20];
	M0_WIFI_Status_t ret = M0_WIFI_STATUS_ERROR;
	M0_WIFI_EnterCmdMode();
	ret=AT_ExecuteCommand((Mode==M0_WIFI_CMDMODE)? "AT+TMODE=cmd\r\n" : "AT+TMODE=throughput\r\n",data);
	M0_WIFI_SaveAndReset();
	return ret;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
