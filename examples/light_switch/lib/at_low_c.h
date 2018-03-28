/**
 @file at_low_c.h
 @version 1.0
 @date 2017-12-12
 @brief Define API to communicate with a AT-command device
 @author Bui Van Hieu <vanhieubk@gmail.com>
*/

#ifndef __AT_LOW_C_H
#define __AT_LOW_C_H

#include <stdint.h>
#include <string.h>
#include "app_uart.h"
#include "systmr.h"

#define TIMEOUT_INCOMING_BYTE		(3) 													// in second
#define UART_TX_BUF_SIZE 				(1024)                        /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE 				(1024)                         /**< UART RX buffer size. */

bool ATLOW_Reinit(uint8_t peripheralIndex, uint32_t baudRate, bool isFlowControl, char* pResp, int16_t maxResp);
bool ATLOW_SendChar(char sendChar);
bool ATLOW_SendStr(char* pSendStr);
bool ATLOW_SendNum(uint32_t sendNum);
bool ATLOW_SendNum(int32_t sendNum);

bool ATLOW_Wait(uint32_t waitTime, char* pResp);
bool ATLOW_Wait(uint32_t timeOut, char* pWaitStr, char* pResp);

	//bool SendAndWait(char* pSendStr, uint32_t waitTime, char* pResp);
bool ATLOW_SendAndWait(char* pSendStr, uint32_t timeOut, char* pWaitStr, char* pResp);

	// send command without any parameters
bool ATLOW_SendCommandAndWait(char* pCmd, uint32_t timeOut, char* pWaitStr, char* pResp);

  // send command with one parameter
bool ATLOW_SendCommandAndWait(char* pCmd, int32_t firstNumPara, uint32_t timeOut, char* pWaitStr, char* pResp);
bool ATLOW_SendCommandAndWait(char* pCmd, char* pFirstStrPara,  uint32_t timeout, char* pWaitStr, char* pResp);

  // send command with two parameters
bool ATLOW_SendCommandAndWait(char* pCmd, int32_t firstNumPara, int32_t secondNumPara, uint32_t timeOut, char* pWaitStr, char* pResp);
bool ATLOW_SendCommandAndWait(char* pCmd, int32_t firstNumPara, char* pSecondStrPara,  uint32_t timeOut, char* pWaitStr, char* pResp);
bool ATLOW_SendCommandAndWait(char* pCmd, char* pFirstStrPara,  int32_t secondNumPara, uint32_t timeout, char* pWaitStr, char* pResp);
bool ATLOW_SendCommandAndWait(char* pCmd, char* pFirstStrPara,  char* secondStrPara,   uint32_t timeout, char* pWaitStr, char* pResp);

  // send command with three parameters
bool ATLOW_SendCommandAndWait(char* pCmd, int32_t firstNumPara, int32_t secondNumPara, int32_t thirdNumPara, uint32_t timeOut, char* pWaitStr, char* pResp);
bool ATLOW_SendCommandAndWait(char* pCmd, int32_t firstNumPara, int32_t secondNumPara, char* pThirdStrPara,  uint32_t timeOut, char* pWaitStr, char* pResp);
bool ATLOW_SendCommandAndWait(char* pCmd, int32_t firstNumPara, char* pSecondStrPara,  int32_t thirdNumPara, uint32_t timeOut, char* pWaitStr, char* pResp);

  // send command with four parameters
bool ATLOW_SendCommandAndWait(char* pCmd, int32_t firstNumPara, int32_t secondNumPara, char* pThirdStrPara, char* pFourthStrPara, uint32_t timeOut, char* pWaitStr, char* pResp);


#endif /*  __AT_LOW_C_H */

