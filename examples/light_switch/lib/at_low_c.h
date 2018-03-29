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
#include "boards.h"

#define TIMEOUT_INCOMING_BYTE		(3) 													// in second
#define UART_TX_BUF_SIZE 				(1024)                        /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE 				(1024)                         /**< UART RX buffer size. */

bool ATLOW_Reinit(uint32_t baudRate, bool isFlowControl, char* pResp, int16_t maxResp);
bool ATLOW_SendChar(char sendChar);
bool ATLOW_SendStr(char* pSendStr);
bool ATLOW_SendNum(uint32_t sendNum);

bool ATLOW_WaitNoExpect(uint32_t waitTime, char* pResp);
bool ATLOW_Wait(uint32_t timeOut, char* pWaitStr, char* pResp);

bool ATLOW_SendAndWait(char* pSendStr, uint32_t timeOut, char* pWaitStr, char* pResp);

#endif /*  __AT_LOW_C_H */

