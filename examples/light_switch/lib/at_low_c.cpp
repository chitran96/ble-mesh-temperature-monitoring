/**
 @file at_low_c.cpp
 @version 1.0
 @date 2017-12-12
 @brief Implement API to communicate with a AT-command device.
 @attention This is the implement for Nordic SDK xx.yy.zz of nRF52832
 @author Bui Van Hieu <vanhieubk@gmail.com>
*/
#include "at_low_c.h"
#include "board_config.h"
#include "nrf_uart.h"
#include "nrf_delay.h"


///////////////////////////////////////////////////////////////////////////////////////////////
extern "C"{
void uart_error_handle(app_uart_evt_t * p_event);
} /* end extern c */

static char CRLF[] = "\r\n";
static const char EQUAL_SYMB = '=';
static const char COMMA_SYMB = ',';
static const char QUOTE_SYMB = '\"';


///////////////////////////////////////////////////////////////////////////////////////////////
//********************************************************************
/**
	@brief flush RX buffer
	@return none
*/
void at_low_c::FlushRxBuf(void){
  nrf_delay_ms(30);		//WHY NEED DELAY HERE ???
	uint8_t c;
	while (app_uart_get(&c) == NRF_SUCCESS);
};


////////////////////////////////////////////////////////////////////////////////////////////////
//********************************************************************
/**
	@brief Reinit the library
  @param[in] peripheralIndex The index of peripheral used for this communication
	@param[in] baudRate Communicate baud rate
  @param[in] isFlowControl Whether hardware flow control is enabled
  @param[in] pResp Pointer to buffer to store response for future communication
	@param[in] maxResp Maximum response lenght
	@return TRUE done success
  @attention peripheralIndex is not used in this Nordic implementation. The pResp has to be pointed to a 
             static buffer and keepr remain for future communication
*/
bool at_low_c::Reinit(uint8_t peripheralIndex, uint32_t baudRate, bool isFlowControl, char* pResp, int16_t maxResp){
  app_uart_comm_params_t  comm_params;
  uint32_t                res;
  
  (void) peripheralIndex; //to remove warning
  //set uart parameters
  switch (baudRate){
    case 9600:   comm_params.baud_rate = NRF_UART_BAUDRATE_9600;   break;
    case 19200:  comm_params.baud_rate = NRF_UART_BAUDRATE_19200;  break;
    case 38400:  comm_params.baud_rate = NRF_UART_BAUDRATE_38400;  break;
    case 57600:  comm_params.baud_rate = NRF_UART_BAUDRATE_57600;  break;
    case 115200: comm_params.baud_rate = NRF_UART_BAUDRATE_115200; break;
    default: return false;
  }
  comm_params.rx_pin_no    = BOARD_UART_RX_PIN; 
  comm_params.tx_pin_no    = BOARD_UART_TX_PIN;
  comm_params.rts_pin_no   = BOARD_UART_RTS_PIN;
  comm_params.cts_pin_no   = BOARD_UART_CTS_PIN;
  comm_params.flow_control = (isFlowControl) ? APP_UART_FLOW_CONTROL_ENABLED : APP_UART_FLOW_CONTROL_DISABLED;
  comm_params.use_parity   = false;
  buffers.rx_buf           = rx_buf;                                                              
  buffers.rx_buf_size      = sizeof(rx_buf);                                                     
  buffers.tx_buf           = tx_buf;                                                              
  buffers.tx_buf_size      = sizeof(tx_buf); 
  
  // init uart
  app_uart_close();
  res = app_uart_init(&comm_params, &buffers, uart_error_handle, APP_IRQ_PRIORITY_LOWEST);
	
  // store configuration data and return result
  spResp = pResp;
  sMaxRespLength = maxResp;		
  if (res == NRF_SUCCESS){
    return true;
  }
  return false;
};


//********************************************************************
/**
	@brief Send a character
	@param[in] sendChar Character to send
	@return TRUE done success
*/
bool at_low_c::SendChar(char sendChar){
  LOG_PutChar(sendChar);
  return (app_uart_put(sendChar) == NRF_SUCCESS);
};


//********************************************************************
/**
	@brief Send a string
	@param[in] pSendStr String to send
  @param[in] size size of string
	@return TRUE done success
*/
bool at_low_c::SendStr(char* pSendStr){
	while (*pSendStr != '\0'){
		if (app_uart_put(*pSendStr) != NRF_SUCCESS){
			return false;
    }
    LOG_PutChar(*pSendStr);
    pSendStr++;
	}
	return true;
};


//********************************************************************
/**
	@brief Send a unsigned number
	@param[in] sendNum Number to send
	@return TRUE done success
*/
bool at_low_c::SendNum(uint32_t sendNum){
	char strOfNum[12];
	sprintf(strOfNum, "%u", sendNum);
	return SendStr(strOfNum);
};


//********************************************************************
/**
	@brief Send a signed number
	@param[in] sendNum Number to send
	@return TRUE done success
*/
bool at_low_c::SendNum(int32_t sendNum){
	char strOfNum[12];
	sprintf(strOfNum, "%d", sendNum);
	return SendStr(strOfNum);
};


//********************************************************************
/**	
	@brief Receive response while waiting timeOut
	@param[in] waitTime Waiting time to receive the response. The unit is mili second
	@param[out] pResp The full received response. If pResp is NULL then use the buffer set in Reinit
	@return TRUE done success
*/
bool at_low_c::Wait(uint32_t waitTime, char* pResp){
	bool      firstRecv;
	uint16_t  numReceivedByte = 0;
  uint8_t*  pRecvChar;
  poll_timeout_c timeOutWaitResp(waitTime);
	poll_timeout_c timeOutWaitByte(TIMEOUT_INCOMING_BYTE * 1000);
  
  pRecvChar = (pResp) ? ((uint8_t*) pResp) : ((uint8_t*) spResp);
  
  firstRecv = false;
  while (! timeOutWaitResp.IsTimeOut()){
		if (firstRecv){
			if (timeOutWaitByte.IsTimeOut()){
				*pRecvChar = '\0';
        LOG_Printf("%s", pResp);
				return true;
			}
		}
    while (app_uart_get(pRecvChar) == NRF_SUCCESS){
			firstRecv = true;
			timeOutWaitByte.Reinit(TIMEOUT_INCOMING_BYTE * 1000);
      if (numReceivedByte >= sMaxRespLength - 1){
        *pRecvChar = '\0';
        return false;
      }
      else{
        pRecvChar++;      
        numReceivedByte++;
      }
    }
  }
  *pRecvChar = '\0';
  LOG_Printf("%s", pResp);
	return true;
};


//********************************************************************
/**
	@brief Wait for expected response
	@param[in] pWaitStr String to wait for
	@param[in] timeOut Waiting timeOut. The unit is mili second
	@param[out] pResp The full received response
	@return TRUE done success
*/
bool at_low_c::Wait(uint32_t timeOut, char* pWaitStr, char* pResp){
  bool            firstRecv = false;
	uint16_t         numReceivedByte = 0;
  uint8_t*        pRecvChar;
  uint16_t         waitStrLen;
  poll_timeout_c  timeOutWaitResp(timeOut);
	poll_timeout_c  timeOutWaitByte(TIMEOUT_INCOMING_BYTE * 1000);
  
  pRecvChar  = (pResp) ? ((uint8_t*) pResp) : ((uint8_t*) spResp);
  waitStrLen = strlen(pWaitStr);
  while (! timeOutWaitResp.IsTimeOut()){
		if (firstRecv){
			if (timeOutWaitByte.IsTimeOut()){
				*pRecvChar = '\0';
				return false;
			}
		}
    while (app_uart_get(pRecvChar) == NRF_SUCCESS){
			firstRecv = true;
			timeOutWaitByte.Reinit(TIMEOUT_INCOMING_BYTE * 1000);
      if (numReceivedByte >= sMaxRespLength - 1){ //buffer overflow
        *pRecvChar = '\0';
        return false;
      }
      else{
        numReceivedByte++;
        pRecvChar++; 
        *pRecvChar = '\0'; //add end string
        if (numReceivedByte >= waitStrLen){
          if (!strncmp(pResp + numReceivedByte - waitStrLen, pWaitStr, waitStrLen)) { //detect waitString
            LOG_Printf("%s", pResp);
            return true;
          }
        }
      }
    }
  } //end while
  
  *pRecvChar = '\0';
  LOG_Printf("%s", pResp);
	return false; //timeOut
};


//********************************************************************
/**
	@brief Send a string then receive response while waiting timeOut
	@param[in] pSendStr String to send
	@param[in] waitTime Waiting time to receive the response. The unit is mili second
	@param[out] pResp The full received response
	@return TRUE done success
*/
/*bool at_low_c::SendAndWait(char* pSendStr, uint32_t waitTime, char* pResp)
{
  FlushRxBuf();
	if (SendStr(pSendStr))
  {
		return Wait(waitTime, pResp);
  }
  else{
    *pResp = '\0';
    return false;
  }
}; */


//********************************************************************
/**
	@brief Send a string then wait for expected response
	@param[in] pSendStr String to send
	@param[in] timeOut Waiting timeOut. The unit is mili second
  @param[in] pWaitStr String to wait for. If pWaitStr==NULL then just wait for timeout
	@param[out] pResp The full received response
	@return TRUE done success
*/
bool at_low_c::SendAndWait(char* pSendStr, uint32_t timeOut, char* pWaitStr, char* pResp){
  FlushRxBuf();
	if (SendStr(pSendStr)){
    if (pWaitStr == NULL){
      return Wait(timeOut, pResp);
    }
    else{
      return Wait(timeOut, pWaitStr, pResp);
    }
  }
  else{
     *pResp = '\0';
    return false;
  }
};


//********************************************************************
/**
	@brief Send a command then receive response while waiting timeOut
	@param[in] pCmd String to send
	@param[in] timeOut Waiting timeOut. The unit is mili second
  @param[in] pWaitStr String to wait for
	@param[out] pResp The full received response
	@return TRUE done success
*/
bool at_low_c::SendCommandAndWait(char* pCmd, uint32_t timeOut, char* pWaitStr, char* pResp){
  SendStr(pCmd);
	return SendAndWait(CRLF, timeOut, pWaitStr, pResp);
};


//********************************************************************
/**
	@brief Send AT command with one number parameter then wait for expected response
	@param[in] pCmd AT command
  @param[in] firstNumPara number as AT command parameter
  @param[in] pWaitStr String to wait for
	@param[in] timeOut Waiting timeOut. The unit is mili second
	@param[out] pResp The full received response
	@return TRUE done success
*/
bool at_low_c::SendCommandAndWait(char* pCmd, int32_t firstNumPara, uint32_t timeOut, char* pWaitStr, char* pResp){
	SendStr(pCmd);
  SendChar(EQUAL_SYMB);
  SendNum(firstNumPara);
  
	return SendAndWait(CRLF, timeOut, pWaitStr,  pResp);
};


//********************************************************************
/**
	@brief Send AT command with one string parameter then wait for expected response
	@param[in] pCmd AT command
  @param[in] pFirstStrPara string as AT command parameter
	@param[in] timeOut Waiting timeOut. The unit is mili second
	@param[in] pWaitStr String to wait for
  @param[out] pResp The full received response
	@return TRUE done success
*/
bool at_low_c::SendCommandAndWait(char* pCmd, char* pFirstStrPara, uint32_t timeOut, char* pWaitStr, char* pResp){ 
  SendStr(pCmd);
  SendChar(EQUAL_SYMB);
 
  SendChar(QUOTE_SYMB);
  SendStr(pFirstStrPara);
  SendChar(QUOTE_SYMB);
  
	return SendAndWait(CRLF, timeOut, pWaitStr, pResp);
};


//********************************************************************
/**
	@brief Send AT command with two number parameters then wait for expected response
	@param[in] pCmd AT command
  @param[in] firstNumPara number as AT command first parameter
  @param[in] secondNumPara number as AT command second parameter
  @param[in] pWaitStr String to wait for
	@param[in] timeOut Waiting timeOut. The unit is mili second
	@param[out] pResp The full received response
	@return TRUE done success
*/
bool at_low_c::SendCommandAndWait(char* pCmd, int32_t firstNumPara, int32_t secondNumPara, uint32_t timeOut, char* pWaitStr, char* pResp){
	SendStr(pCmd);
  SendChar(EQUAL_SYMB);
  
  SendNum(firstNumPara);
  SendChar(COMMA_SYMB);
  SendNum(secondNumPara);
  
	return SendAndWait(CRLF, timeOut, pWaitStr,  pResp);
};


//********************************************************************
/**
	@brief Send AT command with one number and one string parameters then wait for expected response
	@param[in] pCmd AT command
  @param[in] firstNumPara number as AT command first parameter
  @param[in] pSecondStrPara string as AT command second parameter
  @param[in] pWaitStr String to wait for
	@param[in] timeOut Waiting timeOut. The unit is mili second
	@param[out] pResp The full received response
	@return TRUE done success
*/
bool at_low_c::SendCommandAndWait(char* pCmd, int32_t firstNumPara, char* pSecondStrPara, uint32_t timeOut, char* pWaitStr, char* pResp){
	SendStr(pCmd);
  SendChar(EQUAL_SYMB);
  
  SendNum(firstNumPara);
  
  SendChar(COMMA_SYMB);
  SendChar(QUOTE_SYMB);
  SendStr(pSecondStrPara);
  SendChar(QUOTE_SYMB);
  
	return SendAndWait(CRLF, timeOut, pWaitStr,  pResp);
};


//********************************************************************
/**
	@brief Send AT command with one string and one number parameters then wait for expected response
	@param[in] pCmd AT command
  @param[in] pFirstStrPara string as AT command first parameter
  @param[in] secondNumPara number as AT command second parameter
	@param[in] timeOut Waiting timeOut. The unit is mili second
	@param[in] pWaitStr String to wait for
  @param[out] pResp The full received response
	@return TRUE done success
*/
bool at_low_c::SendCommandAndWait(char* pCmd, char* pFirstStrPara, int32_t secondNumPara, uint32_t timeOut, char* pWaitStr, char* pResp){
	SendStr(pCmd);
  SendChar(EQUAL_SYMB);
  
  SendChar(QUOTE_SYMB);
  SendStr(pFirstStrPara);
  SendChar(QUOTE_SYMB);
  
  SendChar(COMMA_SYMB);
  SendNum(secondNumPara);
	return SendAndWait(CRLF, timeOut, pWaitStr, pResp);
};



//********************************************************************
/**
	@brief Send AT command with two string parameters then wait for expected response
	@param[in] pCmd AT command
  @param[in] pFirstStrPara string as AT command first parameter
  @param[in] pSecondStrPara string as AT command second parameter
	@param[in] timeOut Waiting timeOut. The unit is mili second
	@param[in] pWaitStr String to wait for
  @param[out] pResp The full received response
	@return TRUE done success
*/
bool at_low_c::SendCommandAndWait(char* pCmd, char* pFirstStrPara, char* pSecondStrPara, uint32_t timeOut, char* pWaitStr, char* pResp){
	SendStr(pCmd);
  SendChar(EQUAL_SYMB);
  
  SendChar(QUOTE_SYMB);
  SendStr(pFirstStrPara);
  SendChar(QUOTE_SYMB);
  
  SendChar(COMMA_SYMB);
  SendChar(QUOTE_SYMB);
  SendStr(pSecondStrPara);
  SendChar(QUOTE_SYMB);
  
	return SendAndWait(CRLF, timeOut, pWaitStr, pResp);
};

//********************************************************************
/**
	@brief Send AT command with three number parameters then wait for expected response
	@param[in] pCmd AT command
  @param[in] firstNumPara number as AT command first parameter
  @param[in] secondNumPara number as AT command second parameter
	@param[in] thirdNumPara number as AT command third parameter
  @param[in] timeOut Waiting timeOut. The unit is mili second
  @param[in] pWaitStr String to wait for
	@param[out] pResp The full received response
	@return TRUE done success
*/
bool at_low_c::SendCommandAndWait(char* pCmd, int32_t firstNumPara, int32_t secondNumPara, int32_t thirdNumPara, uint32_t timeOut, char* pWaitStr, char* pResp){
  SendStr(pCmd);
  SendChar(EQUAL_SYMB);

  SendNum(firstNumPara);

  SendChar(COMMA_SYMB);
  SendNum(secondNumPara);

  SendChar(COMMA_SYMB);
  SendNum(thirdNumPara); 

  return SendAndWait(CRLF, timeOut, pWaitStr,  pResp);
}


//********************************************************************
/**
	@brief Send AT command then wait for expected response
	@param[in] pCmd AT command
  @param[in] firstNumPara number as AT command first parameter
  @param[in] secondNumPara number as AT command second parameter
	@param[in] pThirdStrPara string as AT command third parameter
  @param[in] pWaitStr String to wait for
	@param[in] timeOut Waiting timeOut. The unit is mili second
	@param[out] pResp The full received response
	@return TRUE done success
*/
bool at_low_c::SendCommandAndWait(char* pCmd, int32_t firstNumPara, int32_t secondNumPara, char* pThirdStrPara, uint32_t timeOut, char* pWaitStr, char* pResp){
	SendStr(pCmd);
  SendChar(EQUAL_SYMB);
  
  SendNum(firstNumPara);
  
  SendChar(COMMA_SYMB);
  SendNum(secondNumPara);
	
  SendChar(COMMA_SYMB);
	SendChar(QUOTE_SYMB);
  SendStr(pThirdStrPara);
  SendChar(QUOTE_SYMB);
  
	return SendAndWait(CRLF, timeOut, pWaitStr,  pResp);
};


//********************************************************************
/**
	@brief Send AT command then wait for expected response
	@param[in] pCmd AT command
  @param[in] firstNumPara number as AT command first parameter
  @param[in] pSecondStrPara string as AT command second parameter
	@param[in] thirdNumPara number as AT command third parameter
  @param[in] pWaitStr String to wait for
	@param[in] timeOut Waiting timeOut. The unit is mili second
	@param[out] pResp The full received response
	@return TRUE done success
*/
bool at_low_c::SendCommandAndWait(char* pCmd, int32_t firstNumPara, char* pSecondStrPara, int32_t thirdNumPara, uint32_t timeOut, char* pWaitStr, char* pResp){
	SendStr(pCmd);
  SendChar(EQUAL_SYMB);
  
  SendNum(firstNumPara);
  
  SendChar(COMMA_SYMB);
  SendChar(QUOTE_SYMB);
  SendStr(pSecondStrPara);
  SendChar(QUOTE_SYMB);
  
	SendChar(COMMA_SYMB);
	SendNum(thirdNumPara);
  
	return SendAndWait(CRLF, timeOut, pWaitStr,  pResp);
};


//********************************************************************
/**
	@brief Send AT command then wait for expected response
	@param[in] pCmd AT command
  @param[in] firstNumPara number as AT command first parameter
  @param[in] secondNumPara number as AT command second parameter
	@param[in] pThirdStrPara string as AT command third parameter
	@param[in] pFourthStrPara string as AT command fourth parameter
  @param[in] pWaitStr String to wait for
	@param[in] timeOut Waiting timeOut. The unit is mili second
	@param[out] pResp The full received response
	@return TRUE done success
*/
bool at_low_c::SendCommandAndWait(char* pCmd, int32_t firstNumPara, int32_t secondNumPara, char* pThirdStrPara, char* pFourthStrPara, uint32_t timeOut, char* pWaitStr, char* pResp)
{
	SendStr(pCmd);
  SendChar(EQUAL_SYMB);
  
  SendNum(firstNumPara);
  SendChar(COMMA_SYMB);
	
  SendNum(secondNumPara);
	SendChar(COMMA_SYMB);
	
	SendChar(QUOTE_SYMB);
  SendStr(pThirdStrPara);
  SendChar(QUOTE_SYMB);
	SendChar(COMMA_SYMB);
	
	SendChar(QUOTE_SYMB);
  SendStr(pFourthStrPara);
  SendChar(QUOTE_SYMB);
  
	return SendAndWait(CRLF, timeOut, pWaitStr,  pResp);
};








//////////////////////////////////////////////////////////////////////////////////////////////////
//********************************************************************
/**
	@brief dummy callback
	@return none
*/
extern "C"{
  void uart_error_handle(app_uart_evt_t * p_event){
      //if (p_event->evt_type == APP_UART_COMMUNICATION_ERROR){
          //APP_ERROR_HANDLER(p_event->data.error_communication);
      //}
      //else if (p_event->evt_type == APP_UART_FIFO_ERROR){
          //APP_ERROR_HANDLER(p_event->data.error_code);
      //}
  }
} /* end extern "C" */
