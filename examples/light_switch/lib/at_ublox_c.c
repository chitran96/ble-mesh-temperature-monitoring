/**
 @file at_low_c.h
 @version 1.0
 @date 2017-12-12
 @brief Define API to communicate with a UBlox SARA-U201
 @attention The maximum length of the command line is 1024 characters
 @author Bui Van Hieu <vanhieubk@gmail.com>
*/

#include "at_ublox_c.h"

static char AT_RESP_OK[] = "OK\r\n";
//const char RESPONSE_ERROR[] = "ERROR\r\n";
const char RESPONSE_HTTP_OK[] = "\r\nOK\r\n";
static char CRLF[] = "\r\n";
static char QUOTE_STR[] = "\"";
static char SPACE_STR[] = " ";
static char COMMA_STR[] = ",";

static const char EQUAL_SYMB = '=';
static const char COMMA_SYMB = ',';
static const char QUOTE_SYMB = '\"';
static const char COLON_SYMB = ':';

at_cmd_info_t table[NUM_SUPPORT_AT_CMD] =
    {
        {"ATE", 2000}, {"AT+CGMR", 2000}, {"AT+CGSN", 2000}, {"AT+CSMS", 2000}, {"AT+CPMS", 180000},
        {"AT+CNMI", 2000}, {"AT+CMGR", 10000}, {"AT+CMGD", 55000}, {"AT+UGPIOC", 10000}, {"AT+UGPIOR", 10000},
        {"AT+UGPIOW", 10000}, {"AT+UDNSRN", 70000}, {"AT+UDYNDN", 70000}, {"AT+UGPS", 10000}, {"AT+UGIND", 2000},
        {"AT+UGPRF", 2000}, {"AT+UGUBX", 10000}, {"AT+UGTMR", 10000}, {"AT+UGZDA", 10000}, {"AT+UGGGA", 10000},
        {"AT+UGGLL", 10000}, {"AT+UGGSV", 10000}, {"AT+UGRMC", 10000}, {"AT+UGVTG", 10000}, {"AT+UGGSA", 10000},
        {"AT+ULOC", 10000}, {"AT+UTIME", 10000}, {"AT+ULOCIND", 2000}, {"AT+ULOCGNSS", 2000}, {"AT+CIMI", 2000},
        {"AT+CMGF", 2000}, {"AT+CCID", 2000}, {"AT+CMGL", 30000}, {"AT+CGATT", 180000}, {"AT+UPSD", 2000},
        {"AT+UPSDA", 180000}, {"AT+USOCR", 2000}, {"AT+USOCO", 20000}, {"AT+USOWR", 1000}, {"AT+USOCL", 1000},
        {"AT+UHTTP", 2000}, {"AT+UHTTPC", 200000}, {"AT+URDFILE", 2000}, {"AT+UDELFILE", 2000}, {"AT+COPS", 180000},
        {"AT+CREG", 2000}, {"AT+CSDH", 2000}};

#define TIMEOUT_CMD_DEFAULT (2000U)

#define AT_CMD_AT ((char *)"AT\r\n")
#define AT_CMD_ATE0 ((char *)"ATE0\r\n")
#define AT_CMD_ATE1 ((char *)"ATE1\r\n")

#define AT_CMD_CGMR ((char *)"AT+CGMR")
#define AT_CMD_CGSN ((char *)"AT+CGSN")
#define AT_CMD_CSMS ((char *)"AT+CSMS")
#define AT_CMD_CPMS ((char *)"AT+CPMS")
#define AT_CMD_CNMI ((char *)"AT+CNMI")
#define AT_CMD_UMWI ((char *)"AT+UMWI")
#define AT_CMD_CSAS ((char *)"AT+CSAS")
#define AT_CMD_CMGR ((char *)"AT+CMGR")
#define AT_CMD_CMGD ((char *)"AT+CMGD")

#define AT_CMD_UGPIOC ((char *)"AT+UGPIOC")
#define AT_CMD_UGPIOR ((char *)"AT+UGPIOR")
#define AT_CMD_UGPIOW ((char *)"AT+UGPIOW")

#define AT_CMD_UDNSRN ((char *)"AT+UDNSRN")
#define AT_CMD_UDYNDN ((char *)"AT+UDYNDN")
#define AT_CMD_UGPS ((char *)"AT+UGPS")
#define AT_CMD_UGIND ((char *)"AT+UGIND")
#define AT_CMD_UGPRF ((char *)"AT+UGPRF")
#define AT_CMD_UGUBX ((char *)"AT+UGUBX")
#define AT_CMD_UGTMR ((char *)"AT+UGTMR")
#define AT_CMD_UGZDA ((char *)"AT+UGZDA")
#define AT_CMD_UGGGA ((char *)"AT+UGGGA")
#define AT_CMD_UGGLL ((char *)"AT+UGGLL")
#define AT_CMD_UGGSV ((char *)"AT+UGGSV")
#define AT_CMD_UGRMC ((char *)"AT+UGRMC")
#define AT_CMD_UGVTG ((char *)"AT+UGVTG")
#define AT_CMD_UGGSA ((char *)"AT+UGGSA")
#define AT_CMD_ULOC ((char *)"AT+ULOC")
#define AT_CMD_UTIME ((char *)"AT+UTIME")
#define AT_CMD_ULOCIND ((char *)"AT+ULOCIND")
#define AT_CMD_ULOCGNSS ((char *)"AT+ULOCGNSS")
#define AT_CMD_CIMI ((char *)"AT+CIMI")
#define AT_CMD_CMGF ((char *)"AT+CMGF")
#define AT_CMD_CCID ((char *)"AT+CCID")
#define AT_CMD_CMGL ((char *)"AT+CMGL")
#define AT_CMD_CGATT ((char *)"AT+CGATT")
#define AT_CMD_UPSD ((char *)"AT+UPSD")
#define AT_CMD_UPSDA ((char *)"AT+UPSDA")
#define AT_CMD_USOCR ((char *)"AT+USOCR")
#define AT_CMD_USOCO ((char *)"AT+USOCO")
#define AT_CMD_USOWR ((char *)"AT+USOWR")
#define AT_CMD_USOCL ((char *)"AT+USOCL")
#define AT_CMD_UHTTP ((char *)"AT+UHTTP")
#define AT_CMD_UHTTPC ((char *)"AT+UHTTPC")
#define AT_CMD_URDFILE ((char *)"AT+URDFILE")
#define AT_CMD_UDELFILE ((char *)"AT+UDELFILE")
#define AT_CMD_COPS ((char *)"AT+COPS")
#define AT_CMD_CREG ((char *)"AT+CREG")
#define AT_CMD_CSDH ((char *)"AT+CSDH")
#define AT_CMD_UCGATT ((char *)"AT+UCGATT")
#define AT_CMD_UPSV ((char *)"AT+UPSV")
#define AT_CMD_CPWROFF ((char *)"AT+CPWROFF")

#define TIMEOUT_CMD_COPS (180000u)
#define TIMEOUT_CMD_CPMS (180000u)
#define TIMEOUT_CMD_CMGR (10000u)
#define TIMEOUT_CMD_CMGL (60000u)
#define TIMEOUT_CMD_CMGD (55000u)
#define TIMEOUT_CMD_CGATT (90000u)
#define TIMEOUT_CMD_UPSDA (180000u)
#define TIMEOUT_CMD_UHTTPC (200000u)
#define TIMEOUT_CMD_UCGATT (15000u)
#define TIMEOUT_CMD_CPWROFF (40000u)
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//********************************************************************
/**
	@brief Send AT command and wait OK
	@return TRUE done success
*/
bool ATUBLOX_PingTest(char *pResp) {
  return ATLOW_SendAndWait(AT_CMD_AT, TIMEOUT_CMD_DEFAULT, AT_RESP_OK, pResp);
};

//********************************************************************
/**
	@brief Set echo mode
	@param[in] isEnable enable or disable echo mode
	@param[out] pResp response buffer
	@return TRUE done success
*/
bool ATUBLOX_SetEcho(bool isEnable, char *pResp) {
  if (isEnable) {
    return ATLOW_SendAndWait(AT_CMD_ATE1, TIMEOUT_CMD_DEFAULT, AT_RESP_OK, pResp);
  } else {
    return ATLOW_SendAndWait(AT_CMD_ATE0, TIMEOUT_CMD_DEFAULT, AT_RESP_OK, pResp);
  }
};

//********************************************************************
/**
	@brief Read firmware
	@param[out] pResp response buffer
	@return TRUE done success
*/
bool ATUBLOX_ReadFirmware(char *pFirmVer, char *pResp) {
  *pFirmVer = '\0';
  return true;
};

//********************************************************************
/**
	@brief Read IMEI
	@param[out] pResp response buffer
	@return TRUE done success
*/
bool ATUBLOX_ReadIMEI(char *pIMEI, char *pResp) {
  char *pTemp;

  if (ATUBLOX_ReadNoQuestionMark(AT_CMD_CGSN, TIMEOUT_CMD_DEFAULT, AT_RESP_OK, pResp)) {
    pTemp = strtok(pResp, "\r\n");
    strcpy(pIMEI, pTemp);
    ////DB_Printf("IMEI: %s\n", pIMEI);
    return true;
  } else {
    ////DB_Printf("IMEI read fail. Resp: %s\n", pResp);
    return false;
  }
};

//********************************************************************
/**
	@brief Read IMSI
	@param[out] pResp response buffer
	@return TRUE done success
*/
bool ATUBLOX_ReadIMSI(char *pIMSI, char *pResp) {
  char *pTemp;

  if (ATUBLOX_ReadNoQuestionMark(AT_CMD_CIMI, TIMEOUT_CMD_DEFAULT, AT_RESP_OK, pResp)) {
    pTemp = strtok(pResp, "\r\n");
    strcpy(pIMSI, pTemp);
    ////DB_Printf("IMSI: %s\n", pIMSI);
    return true;
  } else {
    ////DB_Printf("IMSI read fail. Resp: %s\n", pResp);
    return false;
  }
};

//********************************************************************
/**
	@brief Read SIMID
	@param[out] pResp response buffer
	@return TRUE done success
*/
bool ATUBLOX_ReadSIMID(char *pSIMID, char *pResp) {
  char *pTemp;

  if (ATUBLOX_ReadNoQuestionMark(AT_CMD_CCID, TIMEOUT_CMD_DEFAULT, AT_RESP_OK, pResp)) {
    pTemp = strstr(pResp, "+CCID: ");
    if (pTemp != NULL) {
      pTemp = strtok(pResp, " ");
      pTemp = strtok(NULL, "\r\n");
      strcpy(pSIMID, pTemp);
      //DB_Printf("SIMID: %s\n", pSIMID);
      return true;
    }
  }

  //DB_Printf("SIMID read fail. Resp: %s\n", pResp);
  return false;
};

//********************************************************************
/**
	@brief Set power mode 
  @param[in] powerMode Power mode
  @param[in] awakeTime Awaken time
	@param[out] pResp response buffer
	@return TRUE done success
*/
/*bool ATUBLOX_SetPowerMode(uint8_t powerMode, int32_t awakeTime, char*pResp){
  return ATLOW_SendAndWait(AT_CMD_UPSV, (int32_t) powerMode, awakeTime, TIMEOUT_CMD_DEFAULT, AT_RESP_OK, pResp);
}*/

//********************************************************************
/**
	@brief Switch off the module
	@param[out] pResp response buffer
	@return TRUE done success
*/
bool ATUBLOX_SwitchOff(char *pResp) {
  return ATLOW_SendAndWait(AT_CMD_CPWROFF, TIMEOUT_CMD_CPWROFF, AT_RESP_OK, pResp);
}

//********************************************************************
/**
	@brief Set network operator mode
	@param[in] mode mode to set, 0 for automatic, ortherwise read datasheet
	@param[out] pResp response buffer
	@return TRUE done success
*/
bool ATUBLOX_SetOperator(uint8_t mode, char *pResp) {
  char *cmd;
  strcat(cmd, AT_CMD_COPS);
  strcat(cmd, "=");
  strcat(cmd, mode);
  strcat(cmd, "\r\n");
  return ATLOW_SendAndWait(cmd, TIMEOUT_CMD_COPS, AT_RESP_OK, pResp);
};

//********************************************************************
/**
	@brief Read network operator name
	@param[out] pInfo network name
	@param[out] pResp response buffer
	@return TRUE done success
*/
bool ATUBLOX_ReadNetworkOperator(char *pInfo, char *pResp) {
  char *pTemp;

  if (ATUBLOX_Read(AT_CMD_COPS, TIMEOUT_CMD_DEFAULT, AT_RESP_OK, pResp)) {
    pTemp = strstr(pResp, "+COPS:");
    if (pTemp != NULL) {
      pTemp = strtok(pResp, "\"");
      pTemp = strtok(NULL, "\"");
      strcpy(pInfo, pTemp);
      //DB_Printf("Joined network: %s\n", pInfo);
    }
    return true;
  } else {
    //DB_Printf("Read network info fail. Resp: %s\n", pResp);
    return false;
  }
};

//********************************************************************
/**
	@brief Read network registration status
	@param[out] stt network status
	@param[out] pResp response buffer
	@return TRUE done success
*/
bool ATUBLOX_ReadNetworkRegistrationStatus(uint8_t *pNwkRegStt, char *pResp) {
  char *pTemp;

  if (ATUBLOX_Read(AT_CMD_CREG, TIMEOUT_CMD_DEFAULT, AT_RESP_OK, pResp)) {
    pTemp = strstr(pResp, "+CREG: ");
    if (pTemp != NULL) {
      strtok(pResp, " ");
      pTemp = strtok(NULL, ",");
      pTemp = strtok(NULL, "\r\n");
      *pNwkRegStt = atoi(pTemp);
      return true;
    }
  }
  return false;
};

//********************************************************************
/**
	@brief Test AT command
	@param[out] pResp response buffer
	@return TRUE done success
*/

bool ATUBLOX_TestATCommand(char *pCmd, char *pResp) {
  bool retResult;
  //DB_Printf("Test AT: %s\n", pCmd);
  retResult = ATLOW_SendAndWait(pCmd, 5000, AT_RESP_OK, pResp);
  //DB_Printf("Response: %s\n", pResp);
  return retResult;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////
////  SMS AT COMMANDs ////////////////////////
//********************************************************************
/**
	@brief Set msg text mode
	@param[in] isEnable enable or disable text mode (disable means PDU mode) 
	@param[out] pResp response buffer
	@return TRUE done success
*/
/*bool ATUBLOX_SMSSetTextMode(bool isEnable, char* pResp){
  char* cmd;
	if (isEnable){
		return ATLOW_SendAndWait("AT+CMGF=1\r\n", TIMEOUT_CMD_DEFAULT, AT_RESP_OK, pResp);
	}
  else{
    return ATLOW_SendAndWait("AT+CMGF=0\r\n", TIMEOUT_CMD_DEFAULT, AT_RESP_OK, pResp);
  }
};*/

//********************************************************************
/**
	@brief Set showing SMS header detail
	@param[in] isEnable enable or disable header detail
	@param[out] pResp response buffer
	@return TRUE done success
*/
/*bool ATUBLOX_SMSSetShowHeader(bool isEnable, char* pResp){
	if (isEnable){
		return ATLOW_SendAndWait(AT_CMD_CSDH, 1, TIMEOUT_CMD_DEFAULT, AT_RESP_OK, pResp);
	}
  else{
    return ATLOW_SendAndWait(AT_CMD_CSDH, 0, TIMEOUT_CMD_DEFAULT, AT_RESP_OK, pResp);
  }
};*/

//********************************************************************
/**
	@brief Set message service
	@param[in] isEnable enable or disable new features, must be enable in this project
	@param[out] pResp response buffer
	@return TRUE done success
*/
/*bool ATUBLOX_SMSSelectService(bool isPhase2Plus, char* pResp){
	if (isPhase2Plus)	{
		return ATLOW_SendAndWait(AT_CMD_CSMS, 1, TIMEOUT_CMD_DEFAULT, AT_RESP_OK,  pResp);
	}
	else{
		return ATLOW_SendAndWait(AT_CMD_CSMS, 0, TIMEOUT_CMD_DEFAULT, AT_RESP_OK, pResp);
	} 
};*/

//********************************************************************
/**
	@brief Set new message URC indicaiton
	@param[in] isForward TRUE forward URC. FALSE keep URC in the MT
	@param[out] pResp response buffer
	@return TRUE done success
*/
/*bool ATUBLOX_SMSSetNewIndication(uint32_t indicateMode, char* pResp){
	return ATLOW_SendAndWait(AT_CMD_CNMI, indicateMode, TIMEOUT_CMD_DEFAULT, AT_RESP_OK, pResp);
};*/

//********************************************************************
/**
	@brief Set waiting message indication URC
  @param[in] inEnable TRUE will send URC. FALSE won't send URC
	@param[out] pResp response buffer
	@return TRUE done success
*/
/*bool ATUBLOX_SMSSetWaitingIndicaition(bool isEnable, char* pResp){
  if (isEnable){
    return ATLOW_SendAndWait(AT_CMD_UMWI, 1, TIMEOUT_CMD_DEFAULT, AT_RESP_OK, pResp);
  }
  else{
    return ATLOW_SendAndWait(AT_CMD_UMWI, 0, TIMEOUT_CMD_DEFAULT, AT_RESP_OK, pResp);
  }
}*/

//********************************************************************
/**
	@brief Set prefferred message storage
	@param[in] isOnSIM TRUE store message on SIM. FALSE store message on device
	@param[out] pResp response buffer
	@attention not sure about <mem1> and <mem2>, <mem3> is message storage
	@return TRUE done success
*/
/*bool ATUBLOX_SMSSetPreferredStorage(bool isOnSIM, char* pResp){
	if (isOnSIM){
		return ATLOW_SendAndWait((char*) "AT+CPMS=\"SM\",\"SM\",\"SM\"\r\n", TIMEOUT_CMD_CPMS, AT_RESP_OK, pResp);
	}
	else{
		return ATLOW_SendAndWait((char*) "AT+CPMS=\"MT\",\"MT\",\"MT\"\r\n", TIMEOUT_CMD_CPMS, AT_RESP_OK,  pResp);
	}
};*/

//********************************************************************
/**
	@brief Save the SMS setting to NVM
  @return TRUE done success
*/
/*bool ATUBLOX_SMSSaveSetting(char* pResp){
  return ATLOW_SendAndWait(AT_CMD_CSAS, TIMEOUT_CMD_DEFAULT, AT_RESP_OK, pResp);
}*/

//********************************************************************
/**
	@brief Read message at specified index
	@param[in] index Index of SMS in storage to read
  @param[out] pSentNum Number that sent this message
  @param[out] pContent Content of the message
	@param[out] pResp response buffer
	@return TRUE done success
  @attention To reduce stack memory requirement, extract text contents directly on pResp
*/
/*bool ATUBLOX_SMSReadAt(uint8_t index, char* pSentNum, char* pContent, char* pResp){
  char* pBuffer;
  char* pNum;
  
	if (ATLOW_SendAndWait(AT_CMD_CMGR, index, TIMEOUT_CMD_CMGR, AT_RESP_OK, pResp)){
    // get sent number 
		pNum = strtok(pResp, ",\"");
		pNum = strtok(NULL, QUOTE_STR);
		strcpy(pSentNum, pNum);
    
    // get message content
		pBuffer = strtok(NULL, "\r\n");
		pBuffer = strtok(NULL, "\r\n");
		strcpy(pContent, pBuffer);
		return true;
	}
	return false;
};*/

//********************************************************************
/**
	@brief Read oldest message in storage
  @param[out] hasSms Having new SMS and return parameters are valid
  @param[out] pIndex Index of SMS in storage memory
	@param[out] pSentNum Number that sent this message
  @param[out] pContent Content of the message
	@param[out] pResp response buffer
	@return TRUE done success
  @attention This implementation wait to receive all SMSs so need large buffer. To reduce stack memory requirement, extract text contents directly on pResp
*/
/*bool ATUBLOX_SMSReadOldest(bool* hasSms, int32_t* pIndex, char* pSentNum, char* pContent, char* pResp){
	char* pStartSms;
  char* pNum = NULL;
	char* pExtractContent = NULL;

  ////delay(50);  //add code to ensure there are times before and after delete AT command
  if (ATLOW_SendAndWait(AT_CMD_CMGL, TIMEOUT_CMD_CMGL, AT_RESP_OK, pResp)){
    pStartSms = strstr(pResp, "+CMGL");
    if (pStartSms != NULL){
      // extract SMS index
      pNum = strtok(pStartSms, SPACE_STR);
      pNum = strtok(NULL, COMMA_STR);
      *pIndex = atoi(pNum);
      
      // extract sent number
      pNum = strtok(NULL,  QUOTE_STR);
      pNum = strtok(NULL,  QUOTE_STR);
      pNum = strtok(NULL,  QUOTE_STR);
      //pNum = strtok(NULL,  QUOTE_STR);
      strcpy(pSentNum, pNum);
      
      // extract SMS content
      pExtractContent = strtok(NULL, "\n");
      pExtractContent = strtok(NULL, "\r");
      strcpy(pContent, pExtractContent);
      
      if ((pSentNum != NULL) && (pContent != NULL)){
        *hasSms = true;
      }
      else{
        *hasSms = false;
        //DB_Puts("SMSReadOldest CMGL without corect sentNum and Content\n");
      }
    } //end if (pStartSms != NULL)
    else{ //no +CMGL in response
      *hasSms = false;
    }
    ////delay(50);  //add code to ensure there are times before and after delete AT command
		return true;
	}
  else{
    //DB_Printf("Read oldest SMS fail. Resp: %s\n", pResp);
    return false;
  }
};*/

//********************************************************************
/**
	@brief Delete message by index
	@param[in] index msg index in storage
	@param[out] pResp response buffer
	@return TRUE done success
*/
//bool ATUBLOX_SMSDeleteAt(int32_t index, char* pResp){
//	bool result;
//
//  //delay(50);  //add code to ensure there are times before and after delete AT command
//  result = ATLOW_SendAndWait(AT_CMD_CMGD, index, TIMEOUT_CMD_CMGD, AT_RESP_OK, pResp);
//  //delay(50);  //add code to ensure there are times before and after delete AT command
//  if (result){
//    //DB_Printf("Delete SMS(%d) success\n", index);
//  }
//  else{
//    //DB_Printf("Delete SMS(%d) fail. Resp: %s\n", index, pResp);
//  }
//  return result;
//};

//********************************************************************
/**
	@brief Delete all message in storage
  @param[in] includeUnread TRUE deleate unread message also
	@param[out] pResp response buffer
	@return TRUE done success
*/
//bool ATUBLOX_SMSDeleteAll(bool includeUnread, char* pResp){
//  bool result;
//
//  //delay(50);  //add code to ensure there are times before and after delete AT command
//  if (includeUnread){
//    result = ATLOW_SendAndWait(AT_CMD_CMGD, 1, 4, TIMEOUT_CMD_CMGD, AT_RESP_OK,  pResp);
//  }
//  else{
//    result = ATLOW_SendAndWait(AT_CMD_CMGD, 1, 3, TIMEOUT_CMD_CMGD, AT_RESP_OK,  pResp);
//  }
//  //delay(50);  //add code to ensure there are times before and after delete AT command
//
//  if (result){
//    //DB_Puts("Delete all SMSs success\n");
//  }
//  else{
//    //DB_Printf("Delete all SMS fail. Resp: %s\n", pResp);
//  }
//  return result;
//};

//********************************************************************
/**
	@brief Config GPIO mode
	@param[in] pinIndex pin number
	@param[in] pinMode mode to set
	@param[out] pResp response buffer
	@return TRUE done success
*/
//bool ATUBLOX_GPIOSetMode(uint16_t pinIndex, uint16_t pinMode, char* pResp)
//{
//	ATLOW_SendStr(table[AT_UBLOX_UGPIOC].code);
//	ATLOW_SendChar(EQUAL_SYMB);
//
//	ATLOW_SendNum(pinIndex);
//	ATLOW_SendChar(COMMA_SYMB);
//
//	ATLOW_SendNum(pinMode);
//
//	return ATLOW_SendAndWait(CRLF, table[AT_UBLOX_UGPIOC].timeout, AT_RESP_OK, pResp);
//};

//********************************************************************
/**
	@brief Read GPIO value, must be configed first
	@param[in] pinIndex pin number
	@param[out] pReadVal read value
	@param[out] pResp response buffer
	@return TRUE done success
*/
//bool ATUBLOX_GPIORead(uint16_t pinIndex, bool* pReadVal, char* pResp)
//{
//	if(ATLOW_SendAndWait(table[AT_UBLOX_UGPIOR].code, pinIndex, table[AT_UBLOX_UGPIOR].timeout, AT_RESP_OK, pResp))
//	{
//		char* p = strstr(pResp, "+UGPIO");
//		if (p != NULL)
//		{
//			p = strtok(pResp, ",");
//			p += 2;
//			*(p + 1) = '\0';
//			*(pReadVal) = atoi(p);
//			return true;
//		}
//	}
//	return false;
//};

//********************************************************************
/**
	@brief Set GPIO value, must be configed first
	@param[in] pinIndex pin number
	@param[in] setVal set value
	@param[out] pResp response buffer
	@return TRUE done success
*/
//bool ATUBLOX_GPIOSet(uint16_t pinIndex, bool setVal, char* pResp)
//{
//	if (setVal)
//	{
//		return ATLOW_SendAndWait(table[AT_UBLOX_UGPIOW].code, pinIndex, 1, table[AT_UBLOX_UGPIOW].timeout, AT_RESP_OK, pResp);
//	}
//	return ATLOW_SendAndWait(table[AT_UBLOX_UGPIOW].code, pinIndex, 0, table[AT_UBLOX_UGPIOW].timeout, AT_RESP_OK, pResp);
//};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//********************************************************************
/**
	@brief Register (attach) or deregister (detach) the MT from GPRS service
	@param[in] isAttach TRUE register. FALSE deregister
	@param[out] pResp response buffer
	@return TRUE done success
*/
bool ATUBLOX_GPRSSetAttach(bool isAttach, char *pResp) {
  bool result;

  if (isAttach) {
    result = ATLOW_SendAndWait("AT+CGATT=1\r\n", TIMEOUT_CMD_CGATT, AT_RESP_OK, pResp);
    if (!result) {
      //DB_Printf("Attach GPRS fail. Resp: %s\n", pResp);
    }
  } else {
    result = ATLOW_SendAndWait("AT+CGATT=0\r\n", TIMEOUT_CMD_CGATT, AT_RESP_OK, pResp);
    if (!result) {
      //DB_Printf("Detach GPRS fail. Resp: %s\n", pResp);
    }
  }
  return result;
};

//********************************************************************
/**
	@brief Check if GPRS was attached
	@param[out] pResult TRUE if if GPRS is attached 
	@param[out] pResp response buffer
	@return TRUE done success
*/
bool ATUBLOX_GPRSIsAttach(bool *pResult, char *pResp) {
  *pResult = false;
  if (ATUBLOX_Read(AT_CMD_CGATT, TIMEOUT_CMD_DEFAULT, AT_RESP_OK, pResp)) {
    if (strstr(pResp, "+CGATT: 1") != NULL) {
      *pResult = true;
    } else {
      *pResult = false;
    }
    return true;
  } else {
    return false;
  }
};

//********************************************************************
/**
	@brief Set GPRS authentication type
	@param[in] type authentication type
	@param[out] pResp response buffer
	@return TRUE done success
*/
bool ATUBLOX_GPRSSetAuthentication(uint8_t profileId, uint8_t type, char *pResp) {
  ATLOW_SendStr(AT_CMD_UPSD);
  ATLOW_SendChar(EQUAL_SYMB);

  ATLOW_SendNum(profileId);

  ATLOW_SendChar(COMMA_SYMB);
  ATLOW_SendNum(6); //opcode to set

  ATLOW_SendChar(COMMA_SYMB);
  ATLOW_SendNum(type);

  if (!ATLOW_SendAndWait(CRLF, TIMEOUT_CMD_DEFAULT, AT_RESP_OK, pResp)) {
    //DB_Printf("Set authentication %d fail. Resp: %s\n", type, pResp);
    return false;
  } else {
    //DB_Printf("Set authentication %d success\n", type);
    return true;
  }
};

//********************************************************************
/**
	@brief Set GPRS APN
	@param[in] profileId PDP profile id
	@param[in] APN APN to set
	@param[out] pResp response buffer
	@return TRUE done success
*/
bool ATUBLOX_GPRSSetAPN(uint8_t profileId, char *pAPN, char *pResp) {
  ATLOW_SendStr(AT_CMD_UPSD);
  ATLOW_SendChar(EQUAL_SYMB);

  ATLOW_SendNum(profileId);

  ATLOW_SendChar(COMMA_SYMB);
  ATLOW_SendNum(1); //opcode to set

  ATLOW_SendChar(COMMA_SYMB);
  ATLOW_SendChar(QUOTE_SYMB);
  ATLOW_SendStr(pAPN);
  ATLOW_SendChar(QUOTE_SYMB);

  if (!ATLOW_SendAndWait(CRLF, TIMEOUT_CMD_DEFAULT, AT_RESP_OK, pResp)) {
    //DB_Printf("Set APN %s fail. Rsp: %s\n", pAPN, pResp);
    return false;
  } else {
    //DB_Printf("Set APN %s success\n", pAPN);
    return true;
  }
};

//********************************************************************
/**
	@brief Set GPRS username
	@param[in] profileId PDP profile id
	@param[in] usname username
	@param[out] pResp response buffer
	@return TRUE done success
*/
bool ATUBLOX_GPRSSetUsername(uint8_t profileId, char *pUserName, char *pResp) {
  ATLOW_SendStr(AT_CMD_UPSD);
  ATLOW_SendChar(EQUAL_SYMB);

  ATLOW_SendNum(profileId);

  ATLOW_SendChar(COMMA_SYMB);
  ATLOW_SendNum(2); //opcode to set

  ATLOW_SendChar(COMMA_SYMB);
  ATLOW_SendChar(QUOTE_SYMB);
  ATLOW_SendStr(pUserName);
  ATLOW_SendChar(QUOTE_SYMB);

  if (!ATLOW_SendAndWait(CRLF, TIMEOUT_CMD_DEFAULT, AT_RESP_OK, pResp)) {
    //DB_Printf("Set username %s fail, Resp: %s\n", pUserName, pResp);
    return false;
  } else {
    //DB_Printf("Set username %s success\n", pUserName);
    return true;
  }
};

//********************************************************************
/**
	@brief Set GPRS password
	@param[in] profileId PDP profile id
	@param[in] pPassword password
	@param[out] pResp response buffer
	@return TRUE done success
*/
bool ATUBLOX_GPRSSetPassword(uint8_t profileId, char *pPassword, char *pResp) {
  ATLOW_SendStr(AT_CMD_UPSD);
  ATLOW_SendChar(EQUAL_SYMB);

  ATLOW_SendNum(profileId);

  ATLOW_SendChar(COMMA_SYMB);
  ATLOW_SendNum(3); //opcode to set

  ATLOW_SendChar(COMMA_SYMB);
  ATLOW_SendChar(QUOTE_SYMB);
  ATLOW_SendStr(pPassword);
  ATLOW_SendChar(QUOTE_SYMB);

  if (!ATLOW_SendAndWait(CRLF, TIMEOUT_CMD_DEFAULT, AT_RESP_OK, pResp)) {
    //DB_Printf("Set password %s fail. Resp: %s\r\n", pPassword, pResp);
    return false;
  } else {
    //DB_Printf("Set password %s success\n", pPassword);
    return true;
  }
};

//********************************************************************
/**
	@brief Config PDP context
	@param[in] profileId PDP profile id
	@param[in] isActive active or deactive PDP context
	@param[out] pResp response buffer
	@return TRUE done success
*/
bool ATUBLOX_GPRSSetPDP(uint8_t profileId, bool isActive, char *pResp) {
  bool result;

  ATLOW_SendStr(AT_CMD_UPSDA);
  ATLOW_SendChar(EQUAL_SYMB);

  ATLOW_SendNum(profileId);

  ATLOW_SendChar(COMMA_SYMB);

  if (isActive) {
    ATLOW_SendNum(3);
  } else {
    ATLOW_SendNum(4);
  }
  if (!ATLOW_SendAndWait(CRLF, TIMEOUT_CMD_UPSDA, AT_RESP_OK, pResp)) {
    //DB_Printf("Set password %s fail. Resp: %s\r\n", pPassword, pResp);
    return false;
  } else {
    //DB_Printf("Set password %s success\n", pPassword);
    return true;
  }
};

//********************************************************************
/**
	@brief Create new socket
	@param[socketID] socket id if success, -1 if fail
	@param[out] pResp response buffer
	@return TRUE done success
*/
/*
bool ATUBLOX_GPRSNewSocket(int8_t* socketID, char* pResp)
{
	*socketID = -1;
	if (ATLOW_SendAndWait(table[AT_UBLOX_USOCR].code, 6, table[AT_UBLOX_USOCR].timeout, AT_RESP_OK, pResp))
	{
		char* pTemp = strstr(pResp, "+USOCR: ");
		if (pTemp != NULL)
		{
			pTemp = pTemp + 9;
			*(pTemp + 1) = '\0'; // one digit number
			*socketID = atoi(pTemp);
		}
		return true;
	}
	return false;
};
*/
//********************************************************************
/**
	@brief Create TCP connection
	@param[in] socketId socket id return from GPRSNewSocket
	@param[in] domain server domain or IP
	@param[in] remotePort server remote port
	@param[out] pResp response buffer
	@return TRUE done success
*/
/*
bool ATUBLOX_GPRSConnectTCP(uint8_t socketId, char* domain, uint16_t remotePort, char* pResp)
{
	return ATLOW_SendAndWait(table[AT_UBLOX_USOCO].code, socketId, domain, remotePort, table[AT_UBLOX_USOCO].timeout, AT_RESP_OK, pResp);
};
*/
//********************************************************************
/**
	@brief Close TCP connection
	@param[in] socketId socket id return from GPRSNewSocket
	@param[out] pResp response buffer
	@return TRUE done success
*/
/*
bool ATUBLOX_GPRSCloseTCP(uint8_t socketId, char* pResp)
{
	return ATLOW_SendAndWait(table[AT_UBLOX_USOCL].code, socketId, table[AT_UBLOX_USOCL].timeout, AT_RESP_OK, pResp);
};
*/
//********************************************************************
/**
	@brief Send data through TCP connection
	@param[in] socketId socket id return from GPRSNewSocket
	@param[in] data data to send
	@param[in] dataLength size of data
	@param[out] pResp response buffer
	@return TRUE done success
*/
/*
bool ATUBLOX_GPRSSendData(uint8_t socketId, char* data, uint16_t dataLength, char* pResp)
{
	return ATLOW_SendAndWait(table[AT_UBLOX_USOWR].code, socketId, dataLength, data, table[AT_UBLOX_USOWR].timeout, AT_RESP_OK, pResp);
};
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//********************************************************************
/**
	@brief Reset all HTTP profile's parameters
	@param[in] httpID HTTP profile id
	@param[out] pResp response buffer
	@return TRUE done success
*/
bool ATUBLOX_HTTPResetParameters(uint8_t httpID, char *pResp) {

  ATLOW_SendStr(AT_CMD_UHTTP);
  ATLOW_SendChar(EQUAL_SYMB);

  ATLOW_SendNum(httpID);
  return ATLOW_SendAndWait(CRLF, TIMEOUT_CMD_DEFAULT, AT_RESP_OK, pResp);
};

//********************************************************************
/**
	@brief Set HTTPS server domain
	@param[in] httpID HTTP profile id
	@param[in] pDomain Server domain, or server IP
	@param[out] pResp response buffer
	@return TRUE done success
*/
bool ATUBLOX_HTTPSetDomain(uint8_t httpID, char *pDomain, char *pResp) {

  ATLOW_SendStr(AT_CMD_UHTTP);
  ATLOW_SendChar(EQUAL_SYMB);

  ATLOW_SendNum(httpID);

  ATLOW_SendChar(COMMA_SYMB);
  ATLOW_SendNum(1);

  ATLOW_SendChar(COMMA_SYMB);
  ATLOW_SendChar(QUOTE_SYMB);
  ATLOW_SendStr(pDomain);
  ATLOW_SendChar(QUOTE_SYMB);

  return ATLOW_SendAndWait(CRLF, TIMEOUT_CMD_DEFAULT, AT_RESP_OK, pResp);
};

//********************************************************************
/**
	@brief Set server remote port
	@param[in] httpID HTTP profile id
	@param[in] port server remote port
	@param[out] pResp response buffer
	@return TRUE done success
*/
bool ATUBLOX_HTTPSetRemotePort(uint8_t httpID, uint32_t port, char *pResp) {

  ATLOW_SendStr(AT_CMD_UHTTP);
  ATLOW_SendChar(EQUAL_SYMB);

  ATLOW_SendNum(httpID);

  ATLOW_SendChar(COMMA_SYMB);
  ATLOW_SendNum(5);

  ATLOW_SendChar(COMMA_SYMB);
  ATLOW_SendNum(port);
  return ATLOW_SendAndWait(AT_CMD_UHTTP, TIMEOUT_CMD_DEFAULT, AT_RESP_OK, pResp);
};

//********************************************************************
/**
	@brief Set username field in HTTP header
	@param[in] httpID HTTP profile id
	@param[in] pUsername username
	@param[out] pResp response buffer
	@return TRUE done success
*/
bool ATUBLOX_HTTPSetUsername(uint8_t httpID, char *pUsername, char *pResp) {

  ATLOW_SendStr(AT_CMD_UHTTP);
  ATLOW_SendChar(EQUAL_SYMB);

  ATLOW_SendNum(httpID);

  ATLOW_SendChar(COMMA_SYMB);
  ATLOW_SendNum(2);

  ATLOW_SendChar(COMMA_SYMB);
  ATLOW_SendChar(QUOTE_SYMB);
  ATLOW_SendStr(pUsername);
  ATLOW_SendChar(QUOTE_SYMB);

  return ATLOW_SendAndWait(CRLF, TIMEOUT_CMD_DEFAULT, AT_RESP_OK, pResp);
};

//********************************************************************
/**
	@brief Set password field in HTTP header
	@param[in] httpID HTTP profile id
	@param[in] pwd password
	@param[out] pResp response buffer
	@return TRUE done success
*/
bool ATUBLOX_HTTPSetPassword(uint8_t httpID, char *pPassword, char *pResp) {

  ATLOW_SendStr(AT_CMD_UHTTP);
  ATLOW_SendChar(EQUAL_SYMB);

  ATLOW_SendNum(httpID);

  ATLOW_SendChar(COMMA_SYMB);
  ATLOW_SendNum(3);

  ATLOW_SendChar(COMMA_SYMB);
  ATLOW_SendChar(QUOTE_SYMB);
  ATLOW_SendStr(pPassword);
  ATLOW_SendChar(QUOTE_SYMB);

  return ATLOW_SendAndWait(CRLF, TIMEOUT_CMD_DEFAULT, AT_RESP_OK, pResp);
};

//********************************************************************
/**
	@brief Set HTTP request's authentication method
	@param[in] httpID HTTP profile id
	@param[in] isEnable enable or disable authentication; if enable it will use username & password has been set by two functions above
	@param[out] pResp response buffer
	@return TRUE done success
*/
bool ATUBLOX_HTTPSetAuthentication(uint8_t httpID, bool isEnable, char *pResp) {

  ATLOW_SendStr(AT_CMD_UHTTP);
  ATLOW_SendChar(EQUAL_SYMB);

  ATLOW_SendNum(httpID);

  ATLOW_SendChar(COMMA_SYMB);
  ATLOW_SendNum(4);

  ATLOW_SendChar(COMMA_SYMB);
  if (isEnable) {
    ATLOW_SendNum(1);
  } else {
    ATLOW_SendNum(0);
  }

  return ATLOW_SendAndWait(CRLF, TIMEOUT_CMD_DEFAULT, AT_RESP_OK, pResp);
};

//********************************************************************
/**
	@brief Set custom field in HTTP header
	@param[in] httpID HTTP profile id
	@param[in] headerIdx custom field's index, from 0 to 4
	@param[in] headerName custom field's name
	@param[in] headerVal custom field's value
	@param[out] pResp response buffer
	@return TRUE done success
*/
bool ATUBLOX_HTTPSetCustomHeader(uint8_t httpID, uint8_t headerIdx, char *pHeaderName, char *pHeaderVal, char *pResp) {
  ATLOW_SendStr(AT_CMD_UHTTP);
  ATLOW_SendChar(EQUAL_SYMB);

  ATLOW_SendNum(httpID);

  ATLOW_SendChar(COMMA_SYMB);
  ATLOW_SendNum(9); //opcode to set custom header

  ATLOW_SendChar(COMMA_SYMB);
  ATLOW_SendChar(QUOTE_SYMB);
  ATLOW_SendNum(headerIdx);
  ATLOW_SendChar(COLON_SYMB);
  ATLOW_SendStr(pHeaderName);
  ATLOW_SendChar(COLON_SYMB);
  ATLOW_SendStr(pHeaderVal);
  ATLOW_SendChar(QUOTE_SYMB);

  return ATLOW_SendAndWait(CRLF, TIMEOUT_CMD_DEFAULT, AT_RESP_OK, pResp);
};

//********************************************************************
/**
	@brief Send a GET request to URI
	@param[in] profileID HTTP profile id
	@param[in] uri URI to GET
	@param[in] saveFile name of the file which response from server will be saved
	@param[out] pResult GET susccess or fail
	@param[out] pResp response buffer
	@return TRUE done success
*/
//********************************************************************
/**
	@brief Send a GET request to URI
	@param[in] httpID HTTP profile id
	@param[in] pUri URI to GET
	@param[in] pSaveFileName name of the file which response from server will be saved
	@param[out] pResult GET susccess or fail
	@param[out] pResp response buffer
	@return TRUE done success
*/
bool ATUBLOX_HTTPGetRequest(uint8_t httpID, char *pUri, char *pSaveFileName, bool *pResult, char *pResp) {
  char waitStr[12];

  *pResp = false;

  ATLOW_SendStr(AT_CMD_UHTTPC);
  ATLOW_SendChar(EQUAL_SYMB);

  ATLOW_SendNum(httpID);

  ATLOW_SendChar(COMMA_SYMB);
  ATLOW_SendNum(1); //opcode to set custom header

  ATLOW_SendChar(COMMA_SYMB);
  ATLOW_SendChar(QUOTE_SYMB);
  ATLOW_SendStr(pUri);
  ATLOW_SendChar(QUOTE_SYMB);

  ATLOW_SendChar(COMMA_SYMB);
  ATLOW_SendChar(QUOTE_SYMB);
  ATLOW_SendStr(pSaveFileName);
  ATLOW_SendChar(QUOTE_SYMB);

  if (ATLOW_SendAndWait(CRLF, TIMEOUT_CMD_UHTTPC, AT_RESP_OK, pResp)) {
    if (ATLOW_Wait(50000, (char *)"+UUHTTPCR:", pResp)) {
      if (ATLOW_Wait(50000, CRLF, pResp)) {
        sprintf(waitStr, "%d,1,1", httpID);
        if (strstr(pResp, waitStr) != NULL) {
          *pResult = true;
        }
      }
    }
    return true;
  } else {
    //DB_Puts("HTTP get request fail\n");
    return false;
  }
};

//********************************************************************
/**
	@brief Read HTTP response stored in Flash, then parse response data (omit header)
  @param[in] pFileName Name of the file store HTTP get response
	@param[in] pRawResp Raw data from file
	@param[out] pResp response buffer
	@return TRUE done success
*/
bool ATUBLOX_HTTPParseResponse(char *pFileName, char *pResp) {
  /*char buffer[MAX_GET_RESPONSE_LENGTH];
  
	if (FlashReadFile(pFileName, buffer)){
		strtok(buffer, "{");
		char* p = strtok(NULL, "}");
		strcpy(pResp, p);
	} */

  *pResp = '\0';
  return true;
};

//********************************************************************
/**
	@brief Read file stored in flash
	@param[in] pFileName filename
	@param[out] pResp response buffer
	@return TRUE done success
*/
//bool ATUBLOX_FlashReadFile(char* pFileName, char* pResp){
//	return ATLOW_SendAndWait(AT_CMD_URDFILE, pFileName, TIMEOUT_CMD_DEFAULT, (char*) RESPONSE_HTTP_OK, pResp);
//};

//********************************************************************
/**
	@brief Delete file stored in flash
	@param[in] pFileName Name of file to delete
	@param[out] pResp response buffer
	@return TRUE done success
*/
//bool ATUBLOX_FlashDelFile(char* pFileName, char* pResp){
//	return ATLOW_SendAndWait(AT_CMD_UDELFILE, pFileName, TIMEOUT_CMD_DEFAULT, AT_RESP_OK, pResp);
//};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//********************************************************************
/**
	@brief Turn on off GPS
	@param[in] isOn on of off GPS
	@attention below parameters works if only GPS is on
	@param[in] aidMode Aiding mode
	@param[in] GPSSytem read UGPS AT command
	@param[out] pResp response buffer
	@return TRUE done success
*/
//bool ATUBLOX_GNSSSetPower(bool isOn, uint8_t aidMode, uint32_t GNSSSystem, char* pResp){
//	if (isOn){
//		ATLOW_SendStr(table[AT_UBLOX_UGPS].code);
//		ATLOW_SendChar(EQUAL_SYMB);
//
//    if (isOn){
//      ATLOW_SendNum(1);
//    }
//    else{
//      ATLOW_SendNum(0);
//    }
//
//    ATLOW_SendChar(COMMA_SYMB);
//		ATLOW_SendNum(aidMode);
//
//		ATLOW_SendChar(COMMA_SYMB);
//		ATLOW_SendNum(GNSSSystem);
//
//		return ATLOW_SendAndWait(CRLF, table[AT_UBLOX_UGPS].timeout, AT_RESP_OK, pResp);
//	}
//	return ATLOW_SendAndWait(table[AT_UBLOX_UGPS].code, 0, table[AT_UBLOX_UGPS].timeout, AT_RESP_OK, pResp);
//};

//********************************************************************
/**
	@brief Turn on off URC (send data to terminal)
	@param[in] isEnable enable or disable URC
	@param[out] pResp response buffer
	@return TRUE done success
*/
//bool ATUBLOX_GNSSSetURC(bool isEnable, char* pResp){
//	if (isEnable){
//		return ATLOW_SendAndWait(table[AT_UBLOX_UGIND].code, 1, table[AT_UBLOX_UGIND].timeout, AT_RESP_OK, pResp);
//	}
//  else{
//    return ATLOW_SendAndWait(table[AT_UBLOX_UGIND].code, 0, table[AT_UBLOX_UGIND].timeout, AT_RESP_OK, pResp);
//  }
//};

//********************************************************************
/**
	@brief Config GNSS profile
	@param[in] code config opcode
	@param[out] pResp response buffer
	@return TRUE done success
*/
//bool ATUBLOX_GNSSSetProfile(uint32_t code, char* pResp)
//{
//	return ATLOW_SendAndWait(table[AT_UBLOX_UGPRF].code, code, table[AT_UBLOX_UGPRF].timeout, AT_RESP_OK, pResp);
//};

//********************************************************************
/**
	@brief Config for ULOC (read GPS data) commands
	@param[in] sensor 0 for last fix, 1 for GNSS, 2 for CellLocate
	@param[in] responseType 0 for standard, 1 for detail, 2 for multi-hypotheses
	@param[in] timeout read data timeout
	@param[in] accuracy expected accuracy
	@param[out] pResp response buffer
	@return TRUE done success
*/
//bool ATUBLOX_GNSSConfigData(uint8_t sensor, uint8_t responseType, uint32_t timeout, uint16_t accuracy, char* pResp)
//{
//	ATLOW_SendStr(table[AT_UBLOX_ULOC].code);
//  ATLOW_SendChar(EQUAL_SYMB);
//
//  ATLOW_SendNum(2);
//  ATLOW_SendChar(COMMA_SYMB);
//
//  ATLOW_SendNum(sensor);
//	ATLOW_SendChar(COMMA_SYMB);
//
//	ATLOW_SendNum(responseType);
//	ATLOW_SendChar(COMMA_SYMB);
//
//	ATLOW_SendNum(timeout);
//	ATLOW_SendChar(COMMA_SYMB);
//
//	ATLOW_SendNum(accuracy);
//
//	return ATLOW_SendAndWait(CRLF, table[AT_UBLOX_ULOC].timeout, AT_RESP_OK, pResp);
//};

//********************************************************************
/**
	@brief Config fix data
	@param[in] isEnable Enable or disable
	@param[out] pResp response buffer
	@return TRUE done success
*/
//bool ATUBLOX_GNSSSetFixData(bool isEnable, char* pResp)
//{
//	if (isEnable){
//		return ATLOW_SendAndWait(table[AT_UBLOX_UGGGA].code, 1, table[AT_UBLOX_UGGGA].timeout, AT_RESP_OK, pResp);
//	}
//	return ATLOW_SendAndWait(table[AT_UBLOX_UGGGA].code, 0, table[AT_UBLOX_UGGGA].timeout, AT_RESP_OK, pResp);
//};

//********************************************************************
/**
	@brief Read fix data
	@param[out] pResp response buffer
	@return TRUE done success
*/
//bool ATUBLOX_GNSSGetFixData(char* pResp)
//{
//	return ATUBLOX_Read(AT_UBLOX_UGGGA, AT_RESP_OK, table[AT_UBLOX_UGGGA].timeout, pResp);
//};

//********************************************************************
/**
	@brief Config geographic position
	@param[in] isEnable Enable or disable
	@param[out] pResp response buffer
	@return TRUE done success
*/
//bool ATUBLOX_GNSSSetGeoPos(bool isEnable, char* pResp)
//{
//	if (isEnable)
//	{
//		return ATLOW_SendAndWait(table[AT_UBLOX_UGGLL].code, 1, table[AT_UBLOX_UGGLL].timeout, AT_RESP_OK, pResp);
//	}
//	return ATLOW_SendAndWait(table[AT_UBLOX_UGGLL].code, 0, table[AT_UBLOX_UGGLL].timeout, AT_RESP_OK, pResp);
//};

//********************************************************************
/**
	@brief Read geographic position
	@param[out] pResp response buffer
	@return TRUE done success
*/
//bool ATUBLOX_GNSSGetGeoPos(char* pResp)
//{
//	return ATUBLOX_Read(AT_UBLOX_UGGLL, AT_RESP_OK, table[AT_UBLOX_UGGLL].timeout, pResp);
//};

//********************************************************************
/**
	@brief Config number of GNSS satellites in view
	@param[in] isEnable Enable or disable
	@param[out] pResp response buffer
	@return TRUE done success
*/
//bool ATUBLOX_GNSSSetNumOfSatellites(bool isEnable, char* pResp)
//{
//	if (isEnable)
//	{
//		return ATLOW_SendAndWait(table[AT_UBLOX_UGGSV].code, 1, table[AT_UBLOX_UGGSV].timeout, AT_RESP_OK, pResp);
//	}
//	return ATLOW_SendAndWait(table[AT_UBLOX_UGGSV].code, 0, table[AT_UBLOX_UGGSV].timeout, AT_RESP_OK, pResp);
//};

//********************************************************************
/**
	@brief Read number of GNSS satellites in view
	@param[out] pResp response buffer
	@return TRUE done success
*/
//bool ATUBLOX_GNSSGetNumOfSatellites(char* pResp)
//{
//	return ATUBLOX_Read(AT_UBLOX_UGGSV, AT_RESP_OK, table[AT_UBLOX_UGGSV].timeout, pResp);
//};

//********************************************************************
/**
	@brief Config recommended minimum GNSS data
	@param[in] isEnable Enable or disable
	@param[out] pResp response buffer
	@return TRUE done success
*/
//bool ATUBLOX_GNSSSetRMC(bool isEnable, char* pResp){
//	if (isEnable){
//		return ATLOW_SendAndWait(table[AT_UBLOX_UGRMC].code, 1, table[AT_UBLOX_UGRMC].timeout, AT_RESP_OK, pResp);
//	}
//	else{
//    return ATLOW_SendAndWait(table[AT_UBLOX_UGRMC].code, 0, table[AT_UBLOX_UGRMC].timeout, AT_RESP_OK, pResp);
//  }
//};

//********************************************************************
/**
	@brief Read recommended minimum GNSS data
	@param[out] pResp response buffer
	@return TRUE done success
*/
//bool ATUBLOX_GNSSGetRMC(char* pResp){
//	ATUBLOX_Read(AT_UBLOX_UGRMC, (char*) "+UGRMC: ", table[AT_UBLOX_UGRMC].timeout, pResp);
//  return ATLOW_Wait(table[AT_UBLOX_UGRMC].timeout, AT_RESP_OK, pResp);
//};

//********************************************************************
/**
	@brief Config course over ground and ground speed
	@param[in] isEnable Enable or disable
	@param[out] pResp response buffer
	@return TRUE done success
*/
//bool ATUBLOX_GNSSSetVTG(bool isEnable, char* pResp)
//{
//	if (isEnable)
//	{
//		return ATLOW_SendAndWait(table[AT_UBLOX_UGVTG].code, 1, table[AT_UBLOX_UGVTG].timeout, AT_RESP_OK, pResp);
//	}
//	return ATLOW_SendAndWait(table[AT_UBLOX_UGVTG].code, 0, table[AT_UBLOX_UGVTG].timeout, AT_RESP_OK, pResp);
//};

//********************************************************************
/**
	@brief Read course over ground and ground speed
	@param[out] pResp response buffer
	@return TRUE done success
*/
//bool ATUBLOX_GNSSGetVTG(char* pResp)
//{
//	return ATUBLOX_Read(AT_UBLOX_UGVTG, AT_RESP_OK, table[AT_UBLOX_UGVTG].timeout, pResp);
//};

//********************************************************************
/**
	@brief Config satellite information
	@param[in] isEnable Enable or disable
	@param[out] pResp response buffer
	@return TRUE done success
*/
//bool ATUBLOX_GNSSSetSatelliteInfo(bool isEnable, char* pResp){
//	if (isEnable){
//		return ATLOW_SendAndWait(table[AT_UBLOX_UGGSA].code, 1, table[AT_UBLOX_UGGSA].timeout, AT_RESP_OK, pResp);
//	}
//	return ATLOW_SendAndWait(table[AT_UBLOX_UGGSA].code, 0, table[AT_UBLOX_UGGSA].timeout, AT_RESP_OK, pResp);
//};

//********************************************************************
/**
	@brief Read satellite information
	@param[out] pResp response buffer
	@return TRUE done success
*/
//bool ATUBLOX_GNSSGetSatelliteInfo(char* pResp)
//{
//	return ATUBLOX_Read(AT_UBLOX_UGGSA, AT_RESP_OK, table[AT_UBLOX_UGGSA].timeout, pResp);
//};

//********************************************************************
/**
	@brief Get localization information by using ULOC command
  @param[in] byGNSS TRUE use GNSS module. FALSE: use CellLocate
  @param[in] timeOutInSec Timeout in second
  @param[in] accuracyInMet Accuracy in meter
  @param[out] pData data out
	@param[out] pResp response buffer
	@return TRUE done success
*/
//bool ATUBLOX_GNSSGetULOCData(bool byGNSS, uint32_t timeOutInSec, uint32_t accuracyInMet, char* pData, char* pResp){
//  char pattern[] = "+UULOC: ";
//
//  ATLOW_SendStr(table[AT_UBLOX_ULOC].code);
//
//  ATLOW_SendChar(EQUAL_SYMB);
//  ATLOW_SendNum(2); // single shot position
//
//  if (byGNSS){
//    ATLOW_SendChar(COMMA_SYMB);
//    ATLOW_SendNum(1);
//  }
//  else{ // use CellLocate
//    ATLOW_SendChar(COMMA_SYMB);
//    ATLOW_SendNum(2);
//  }
//
//  ATLOW_SendChar(COMMA_SYMB);
//  ATLOW_SendNum(0);   //standard output format
//
//  ATLOW_SendChar(COMMA_SYMB);
//  ATLOW_SendNum(timeOutInSec);
//
//  ATLOW_SendChar(COMMA_SYMB);
//  ATLOW_SendNum(accuracyInMet);
//
//	if (ATLOW_SendAndWait(CRLF, table[AT_UBLOX_ULOC].timeout, AT_RESP_OK, pResp)){
//    if (ATLOW_Wait((timeOutInSec+2)*1000, pattern, pResp)){
//      if (ATLOW_Wait(TIMEOUT_CMD_DEFAULT, CRLF, pResp)){
//        strncpy(pData, pResp, strlen(pResp)-2); //to remove crlf
//        return true;
//      }
//      else{
//        //DB_Printf("Wait ULOC data fail. Resp: %s\n", pResp);
//        return false;
//      }
//    }//timeout + 2 seconds
//    else{
//      //DB_Printf("Get ULOC data fail. Resp: %s\n", pResp);
//      return false;
//    }
//  }
//  else{
//    ////DB_Printf("Read ULOC fail. Resp: %s\n", pResp);
//    return false;
//  }
//};

//********************************************************************
/**
	@brief Read info using format AT+COMMAND?
	@param[in] readPara command type
	@param[in] timeOut timeout while waiting for response
	@param[out] pResp response buffer
	@return TRUE done success
*/
/*
bool ATUBLOX_Read(at_ublox_cmd_t readPara, uint32_t timeOut, char* pResp)
{
	char buffer[20];
	strcpy(buffer, table[readPara].code);
	strcat(buffer, "?\r\n");
	return ATLOW_SendAndWait(buffer, timeOut, pResp);
};
*/
//********************************************************************
/**
	@brief Read info using format AT+COMMAND?, then wait for expect string
	@param[in] readPara command type
	@param[in] pWait expect string
	@param[in] timeOut timeout while waiting for response
	@param[out] pResp response buffer
	@return TRUE done success
*/
/*bool ATUBLOX_Read(at_ublox_cmd_t readPara, char* pWait, uint32_t timeOut, char* pResp)
{
	char buffer[20];
	strcpy(buffer, table[readPara].code);
	if (readPara == AT_UBLOX_CGSN || readPara == AT_UBLOX_CIMI || readPara == AT_UBLOX_CCID)
	{
		strcat(buffer, "\r\n");
	}
	else {
		strcat(buffer, "?\r\n");
	}
	return ATLOW_SendAndWait(buffer, timeOut, pWait, pResp);
};*/

//********************************************************************
/**
	@brief Read info using format AT+COMMAND?, then wait for expect string
	@param[in] timeOut timeout while waiting for response
  @param[in] pATCmd command to read
	@param[in] pWait expect string
	@param[out] pResp response buffer
	@return TRUE done success
*/
bool ATUBLOX_Read(char *pATCmd, uint32_t timeOut, char *pWait, char *pResp) {
  ATLOW_SendStr(pATCmd);
  return ATLOW_SendAndWait((char *)"?\r\n", timeOut, pWait, pResp);
}

//********************************************************************
/**
	@brief Read info using format AT+COMMAND, then wait for expect string
	@param[in] timeOut timeout while waiting for response
  @param[in] pATCmd command to read
	@param[in] pWait expect string
	@param[out] pResp response buffer
	@return TRUE done success
*/
bool ATUBLOX_ReadNoQuestionMark(char *pATCmd, uint32_t timeOut, char *pWait, char *pResp) {
  ATLOW_SendStr(pATCmd);
  return ATLOW_SendAndWait(CRLF, timeOut, pWait, pResp);
}