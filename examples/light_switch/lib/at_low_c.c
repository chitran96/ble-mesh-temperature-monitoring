/**
 @file at_low_c.cpp
 @version 1.0
 @date 2017-12-12
 @brief Implement API to communicate with a AT-command device.
 @attention This is the implement for Nordic SDK xx.yy.zz of nRF52832
 @author Bui Van Hieu <vanhieubk@gmail.com>
*/
#include "at_low_c.h"


char*    spResp;
app_uart_buffers_t buffers;
uint16_t sMaxRespLength;
uint8_t rx_buf[UART_RX_BUF_SIZE];
uint8_t tx_buf[UART_TX_BUF_SIZE];

static char CRLF[] = "\r\n";
static const char EQUAL_SYMB = '=';
static const char COMMA_SYMB = ',';
static const char QUOTE_SYMB = '\"';

static void uart_error_handle(app_uart_evt_t * p_event);

///////////////////////////////////////////////////////////////////////////////////////////////
//********************************************************************
/**
	@brief flush RX buffer
	@return none
*/
void ATLOW_FlushRxBuf(void){
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
*/
bool ATLOW_Reinit(uint32_t baudRate, bool isFlowControl, char* pResp, int16_t maxResp){
  app_uart_comm_params_t  comm_params;
  uint32_t                res;

  //set uart parameters
  switch (baudRate){
    case 9600:   comm_params.baud_rate = UART_BAUDRATE_BAUDRATE_Baud9600;   break;
    case 19200:  comm_params.baud_rate = UART_BAUDRATE_BAUDRATE_Baud19200;  break;
    case 38400:  comm_params.baud_rate = UART_BAUDRATE_BAUDRATE_Baud38400;  break;
    case 57600:  comm_params.baud_rate = UART_BAUDRATE_BAUDRATE_Baud57600;  break;
    case 115200: comm_params.baud_rate = UART_BAUDRATE_BAUDRATE_Baud115200; break;
    default: return false;
  }
  comm_params.rx_pin_no    = RX_PIN_NUMBER; 
  comm_params.tx_pin_no    = TX_PIN_NUMBER;
  comm_params.rts_pin_no   = RTS_PIN_NUMBER;
  comm_params.cts_pin_no   = CTS_PIN_NUMBER;
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
bool ATLOW_SendChar(char sendChar){
  return (app_uart_put(sendChar) == NRF_SUCCESS);
};


//********************************************************************
/**
	@brief Send a string
	@param[in] pSendStr String to send
  @param[in] size size of string
	@return TRUE done success
*/
bool ATLOW_SendStr(char* pSendStr){
	while (*pSendStr != '\0'){
		if (app_uart_put(*pSendStr) != NRF_SUCCESS){
			return false;
    }
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
bool ATLOW_SendNum(uint32_t sendNum){
	char strOfNum[12];
	sprintf(strOfNum, "%u", sendNum);
	return ATLOW_SendStr(strOfNum);
};

//********************************************************************
/**	
	@brief Receive response while waiting timeOut
	@param[in] waitTime Waiting time to receive the response. The unit is mili second
	@param[out] pResp The full received response. If pResp is NULL then use the buffer set in Reinit
	@return TRUE done success
*/
bool ATLOW_WaitNoExpect(uint32_t waitTime, char* pResp){
	bool      firstRecv;
	uint16_t  numReceivedByte = 0;
  uint8_t*  pRecvChar;
  uint32_t  baseWaitTime = SYSTMR_Second();
  uint32_t  baseWaitByte = SYSTMR_Second();
  
  pRecvChar = (pResp) ? ((uint8_t*) pResp) : ((uint8_t*) spResp);
  
  firstRecv = false;
  while (SYSTMR_Second() - baseWaitTime < waitTime){
		if (firstRecv){
			if (SYSTMR_Second() - baseWaitByte > TIMEOUT_INCOMING_BYTE * 1000) {
				*pRecvChar = '\0';
				return true;
			}
		}
    while (app_uart_get(pRecvChar) == NRF_SUCCESS){
			firstRecv = true;
			baseWaitByte = SYSTMR_Second();
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
bool ATLOW_Wait(uint32_t timeOut, char* pWaitStr, char* pResp){
  bool            firstRecv = false;
	uint16_t         numReceivedByte = 0;
  uint8_t*        pRecvChar;
  uint16_t         waitStrLen;
  uint32_t        baseWaitTime = SYSTMR_Second();
  uint32_t        baseWaitByte = SYSTMR_Second();
  
  pRecvChar  = (pResp) ? ((uint8_t*) pResp) : ((uint8_t*) spResp);
  waitStrLen = strlen(pWaitStr);
  while (SYSTMR_Second() - baseWaitTime < timeOut){
		if (firstRecv){
			if (SYSTMR_Second() - baseWaitByte > TIMEOUT_INCOMING_BYTE * 1000) {
				*pRecvChar = '\0';
				return false;
			}
		}
    while (app_uart_get(pRecvChar) == NRF_SUCCESS){
			firstRecv = true;
			baseWaitByte = SYSTMR_Second();
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
            return true;
          }
        }
      }
    }
  } //end while
  
  *pRecvChar = '\0';
	return false; //timeOut
};

//********************************************************************
/**
	@brief Send a string then wait for expected response
	@param[in] pSendStr String to send
	@param[in] timeOut Waiting timeOut. The unit is mili second
  @param[in] pWaitStr String to wait for. If pWaitStr==NULL then just wait for timeout
	@param[out] pResp The full received response
	@return TRUE done success
*/
bool ATLOW_SendAndWait(char* pSendStr, uint32_t timeOut, char* pWaitStr, char* pResp){
  ATLOW_FlushRxBuf();
  if (ATLOW_SendStr(pSendStr)){
    return ATLOW_Wait(timeOut, pWaitStr, pResp);
  }
  else{
     *pResp = '\0';
    return false;
  }
};

void uart_error_handle(app_uart_evt_t * p_event){
    //if (p_event->evt_type == APP_UART_COMMUNICATION_ERROR){
        //APP_ERROR_HANDLER(p_event->data.error_communication);
    //}
    //else if (p_event->evt_type == APP_UART_FIFO_ERROR){
        //APP_ERROR_HANDLER(p_event->data.error_code);
    //}
}
