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

#define TIMEOUT_INCOMING_BYTE		(3) 													// in second
#define UART_TX_BUF_SIZE 				(1024)                        /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE 				(1024)                         /**< UART RX buffer size. */

class at_low_c{

private:
	uint16_t sMaxRespLength;
  char*    spResp;
	app_uart_buffers_t buffers;
	uint8_t rx_buf[UART_RX_BUF_SIZE];
	uint8_t tx_buf[UART_TX_BUF_SIZE];
	void FlushRxBuf(void);

public:
	/* revised */ bool Reinit(uint8_t peripheralIndex, uint32_t baudRate, bool isFlowControl, char* pResp, int16_t maxResp);
	/* revised */ bool SendChar(char sendChar);
	/* revised */ bool SendStr(char* pSendStr);
	/* revised */ bool SendNum(uint32_t sendNum);
	/* revised */ bool SendNum(int32_t sendNum);

	/* revised */ bool Wait(uint32_t waitTime, char* pResp);
	/* revised */ bool Wait(uint32_t timeOut, char* pWaitStr, char* pResp);

	//bool SendAndWait(char* pSendStr, uint32_t waitTime, char* pResp);
	/* revised */ bool SendAndWait(char* pSendStr, uint32_t timeOut, char* pWaitStr, char* pResp);

	// send command without any parameters
  /* revised */ bool SendCommandAndWait(char* pCmd, uint32_t timeOut, char* pWaitStr, char* pResp);

  // send command with one parameter
	/* revised */ bool SendCommandAndWait(char* pCmd, int32_t firstNumPara, uint32_t timeOut, char* pWaitStr, char* pResp);
  /* revised */ bool SendCommandAndWait(char* pCmd, char* pFirstStrPara,  uint32_t timeout, char* pWaitStr, char* pResp);

  // send command with two parameters
  /* revised */ bool SendCommandAndWait(char* pCmd, int32_t firstNumPara, int32_t secondNumPara, uint32_t timeOut, char* pWaitStr, char* pResp);
	/* revised */ bool SendCommandAndWait(char* pCmd, int32_t firstNumPara, char* pSecondStrPara,  uint32_t timeOut, char* pWaitStr, char* pResp);
	/* revised */ bool SendCommandAndWait(char* pCmd, char* pFirstStrPara,  int32_t secondNumPara, uint32_t timeout, char* pWaitStr, char* pResp);
  /* revised */ bool SendCommandAndWait(char* pCmd, char* pFirstStrPara,  char* secondStrPara,   uint32_t timeout, char* pWaitStr, char* pResp);

  // send command with three parameters
  bool SendCommandAndWait(char* pCmd, int32_t firstNumPara, int32_t secondNumPara, int32_t thirdNumPara, uint32_t timeOut, char* pWaitStr, char* pResp);
	bool SendCommandAndWait(char* pCmd, int32_t firstNumPara, int32_t secondNumPara, char* pThirdStrPara,  uint32_t timeOut, char* pWaitStr, char* pResp);
	bool SendCommandAndWait(char* pCmd, int32_t firstNumPara, char* pSecondStrPara,  int32_t thirdNumPara, uint32_t timeOut, char* pWaitStr, char* pResp);

  // send command with four parameters
  bool SendCommandAndWait(char* pCmd, int32_t firstNumPara, int32_t secondNumPara, char* pThirdStrPara, char* pFourthStrPara, uint32_t timeOut, char* pWaitStr, char* pResp);

}; /* end class */


#endif /*  __AT_LOW_C_H */

