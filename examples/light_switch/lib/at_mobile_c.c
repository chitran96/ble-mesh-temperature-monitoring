#include "at_mobile_c.h"
#include "nrf_delay.h"
#include "log.h"
#include <string.h>

//////////////////////////////////////////////////////////////
////    PRIVATE METHODs     //////////////////
//*************************************************************

/* some functions more here ... */
char      sIMEI[MAX_IMEI_LENGTH];
char      sIMSI[MAX_IMSI_LENGTH];
char      sSIMID[MAX_SIMID_LENGTH];

char      sGPRSAPN[MAX_APN_LENGTH];
char      sGPRSUsername[MAX_USERNAME_LENGTH];
char      sGPRSPassword[MAX_PASSWORD_LENGTH];

char      sServerDomain[MAX_DOMAIN_LENGTH];
uint32_t  sServerPort;
char      _uri[MAX_URI_LENGTH];

uint32_t  readPeriod;

////////////////////////////////////////////////////////////////////////////////////////////
////    PUBLIC METHODs     //////////////////

//*************************************************************
/**
    @brief Constructor
    @return none
*/
void ATMOBILE_Init(void) {
  strcpy(sIMEI, "");
  strcpy(sIMSI, "");
  strcpy(sSIMID, "");
  strcpy(sGPRSAPN, VIETTEL_APN);
  strcpy(sGPRSUsername, VIETTEL_USERNAME);
  strcpy(sGPRSPassword, VIETTEL_PASSWORD);
  strcpy(sServerDomain, SERVER_DOMAIN_DEFAULT);
  sServerPort = SERVER_PORT_DEFAULT;
};

//*************************************************************
/**
    @brief Set read GPS period
    @param[out] pLastResp The last received response
    @return TRUE done success
*/
bool ATMOBILE_SetReadPeriod(uint32_t newPeriod) {
  readPeriod = newPeriod;
  return true;
};

//*************************************************************
/**
    @brief Get read GPS period
    @return current period
*/
uint32_t ATMOBILE_GetReadPeriod(void) {
  return readPeriod;
};

//*************************************************************
/**
    @brief Perform all MOBI init for uniTraKa project
    @param[out] pLastResp The last received response
    @return TRUE done success
*/
bool ATMOBILE_MOBIReinit(char *pLastResp) {
  uint8_t i = 0;

  /* send AT to sync */
  i = 0;
  while ((i < AT_SYNC_TIMES) && (!ATUBLOX_PingTest(pLastResp))) {
    i++;
  }

  /* test AT */
  if (i >= AT_SYNC_TIMES) {
    //DB_Printf("AT test fail. Response: %s\n", pLastResp);
    __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "AT test fail. Response: %s\n", pLastResp);
    return false;
  } else {
    //DB_Puts("AT sync OK\n");
  }

  //__LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "AT sync OK. Resp: %s\n", pLastResp);

  /* set ECHO off */
  if (!ATUBLOX_SetEcho(false, pLastResp)) {
    //DB_Printf("ECHO off fail. Resp: %s\n", pLastResp);
    __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "ECHO off fail. Resp: %s\n", pLastResp);
    return false;
  }

  // if (!MOBIDeRegisterNetwork(pLastResp)) {
  //   return false;
  // }

  // if (!InitGNSSParameters(true, pLastResp)) {
  //   ERR_Puts("GNSS init failed!\r\n");
  //   return false;
  // } else {
  //   //DB_Puts("GNSS init success!\r\n");
  // }

//  if (!ATUBLOX_GPRSSetAuthentication(GPRS_PROFILE_ID, GPRS_AUTH_TYPE, pLastResp)) {
//    __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "GPRS author fail. Resp: %s\n", pLastResp);
//    return false;
//  }


  if (!ATMOBILE_TurnOffGPRSAndPDP(pLastResp))
  {
  __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "Turn off GPRS failed Resp: %s\n", pLastResp);
  }


  if (!ATUBLOX_GPRSSetAPN(GPRS_PROFILE_ID, sGPRSAPN, pLastResp)) {
    __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "GPRS apn fail. Resp: %s\n", pLastResp);
    return false;
  }

  if (!ATUBLOX_GPRSSetUsername(GPRS_PROFILE_ID, sGPRSUsername, pLastResp)) {
    __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "GPRS username fail. Resp: %s\n", pLastResp);
    return false;
  }
  if (!ATUBLOX_GPRSSetPassword(GPRS_PROFILE_ID, sGPRSPassword, pLastResp)) {
    __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "GPRS password fail. Resp: %s\n", pLastResp);
    return false;
  }

  /* wait for register network */
  // if (!MOBIRegisterNetwork(pLastResp)) {
  //   return false;
  // }
  //ensure GPRS is detached after reset


  //delay(500);
  //if (! SMSDeleteAll(true, pLastResp))	{
  //	//DB_Printf("SMS Delete all fail. Resp %s\n", pLastResp);
  //	return false;
  //}

  //delay(500); //wait for network Identify updating
  /* read identify */
  /*if (!ATMOBILE_MOBIReadIdentify(pLastResp)) {
    __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "Identify fail. Resp: %s\n", pLastResp);
    return false;
  }*/
  return true;
};

//*************************************************************
/**
    @brief Read IMEI, IMSI and SIMID then store in private vars
    @param[out] pLastResp The last received response
    @return TRUE done success
*/
bool ATMOBILE_MOBIReadIdentify(char *pLastResp) {
  if (!ATUBLOX_ReadIMEI(sIMEI, pLastResp)) {
    return false;
  }
  if (!ATUBLOX_ReadIMSI(sIMSI, pLastResp)) {
    return false;
  }
  if (!ATUBLOX_ReadSIMID(sSIMID, pLastResp)) {
    return false;
  }

  return true;
};

//*************************************************************
/**
    @brief Deregister from network
    @param[out] pLastResp The last received response
    @return TRUE done success
*/
bool ATMOBILE_MOBIDeRegisterNetwork(char *pLastResp) {
  if (!SetOperator(2, pLastResp)) {
    //DB_Printf("Deregister network fail. Resp: %s\n", pLastResp);
    return false;
  }
  return true;
};

//*************************************************************
/**
    @brief Register to network
    @param[out] pLastResp The last received response
    @return TRUE done success
*/
bool ATMOBILE_MOBIRegisterNetwork(char *pLastResp) {
  //DB_Puts("Wait for network registration\n");
  if (!SetOperator(0, pLastResp)) {
    //DB_Printf("Set operator fail. Resp: %s\n", pLastResp);
    return false;
  }
  return MOBIWaitRegisterNetwork(pLastResp);
};

//*************************************************************
/**
    @brief Wait for network register success
    @param[out] pLastResp The last received response
    @return TRUE done success
*/
// bool ATMOBILE_MOBIWaitRegisterNetwork(char *pLastResp) {
//   uint8_t networkStat;
//   char networkName[32];
//   poll_timeout_c waitRegister(TIMEOUT_WAIT_REGISTER);

//   waitRegister.Reinit(TIMEOUT_WAIT_REGISTER);
//   while (!waitRegister.IsTimeOut()) {
//     if (!ReadNetworkRegistrationStatus(&networkStat, pLastResp)) {
//       //DB_Printf("Read network status error %s \n", pLastResp);
//       return false;
//     }
//     if (networkStat != 0 || networkStat != 2 || networkStat != 3 || networkStat != 4) {
//       ReadNetworkOperator(networkName, pLastResp);
//       return true;
//     }
//   }

//   //DB_Printf("Register network failed %s\n", pLastResp);
//   return false;
// }
//*************************************************************
/**
    @brief Perform all SMS init for uniTraKa project
    @param[out] pLastResp The last received response
    @return TRUE done success
*/
bool ATMOBILE_MOBISMSReinit(char *pLastResp) {
  //delay(50); //ensure delay between AT commands
  // if (!SMSSetTextMode(true, pLastResp)) {
  //   //DB_Printf("SMS Set text fail. Resp: %s\n", pLastResp);
  //   return false;
  // }

  // //delay(50);  //ensure delay between AT commands
  // //if (! SMSSetWaitingIndicaition(false, pLastResp)){
  // //  //DB_Printf("SMS Set waiting indication fail. Resp: %s\n", pLastResp);
  // //	return false;
  // //}

  // delay(50); //ensure delay between AT commands
  // if (!SMSSetShowHeader(false, pLastResp)) {
  //   //DB_Printf("SMS Set header fail. Resp: %s\n", pLastResp);
  //   return false;
  // }

  // delay(50); //ensure delay between AT commands
  // if (!SMSSelectService(false, pLastResp)) {
  //   //DB_Printf("SMS Select service fail. Resp: %s \r\n", pLastResp);
  //   return false;
  // }

  // delay(50); //ensure delay between AT commands
  // Read((char *)"AT+CNMI", 2000, (char *)"OK\r\n", pLastResp);
  // if (!SMSSetNewIndication(0, pLastResp)) {
  //   //DB_Printf("SMS Set notify fail. Resp: %s \r\n", pLastResp);
  //   return false;
  // }

  //delete all SMS in SARA
  /* delay(50);  //ensure delay between AT commands
	if (! SMSSetPreferredStorage(false, pLastResp)){
		//DB_Printf("SMS Set storage fail. Resp: %s\n", pLastResp);
		return false;
	}
  
	delay(500); //ensure delay between AT commands
  if (! SMSDeleteAll(true, pLastResp))	{
		//DB_Printf("SMS Delete all fail. Resp %s\n", pLastResp);
		return false;
	}*/

  //delete all SMS in SIM
  //delay(50);  //ensure delay between AT commands
  //if (! SMSSetPreferredStorage(true, pLastResp)){
  //	//DB_Printf("SMS Set storage fail. Resp: %s\n", pLastResp);
  //	return false;
  //}

  // delay(50); //ensure delay between AT commands
  // if (!SMSSaveSetting(pLastResp)) {
  //   //DB_Printf("SMSDeleteAll Save setting fail. Resp: %s\n", pLastResp);
  //   return false;
  // }

  return true;
};

//*************************************************************
/**
    @brief Read SMS, the oldest one
    @param[out] pHasSMS If there is a new SMS
    @param[out] pSMSIndex Index of the SMS
    @param[out] pSentNum sent number
    @param[out] pContent SMS content
    @param[out] pLastResp The last received response
    @return TRUE done success
*/
bool ATMOBILE_ReadSMS(bool *pHasSms, int32_t *pSMSIndex, char *pSentNum, char *pContent, char *pLastResp) {
  return SMSReadOldest(pHasSms, pSMSIndex, pSentNum, pContent, pLastResp);
};

//*************************************************************
/**
    @brief Delete all SMS in storage
    @param[out] pLastResp The last received response
    @return TRUE done success
*/
bool ATMOBILE_DeleteAllSMS(char *pLastResp) {
  return SMSDeleteAll(true, pLastResp);
};
//*************************************************************
/**
    @brief Join data network 
    @param[out] pResp The full received response
    @return TRUE done success
*/
bool ATMOBILE_TurnOnGPRSAndPDP(char *pResp) {
  bool isAttach;

  if (!ATUBLOX_GPRSIsAttach(&isAttach, pResp)) {
    __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "Get GPRS stt failed response: %s\n", pResp);
    return false;
  }
  if (!isAttach) {
    if (!ATUBLOX_GPRSSetAttach(true, pResp)) {
      __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "Set GPRS failed response: %s\n", pResp);
      return false;
    }
    if (!ATUBLOX_GPRSIsAttach(&isAttach, pResp)) {
      __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "Check GPRS stt failed response: %s\n", pResp);
      return false;
    }
  }
  nrf_delay_ms(500);
  if (!ATUBLOX_GPRSSetPDP(GPRS_PROFILE_ID, true, pResp)) {
    __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "Set PDP failed response: %s\n", pResp);
    return false;
  }

  return true;
};

//*************************************************************
/**
    @brief Join data network 
    @param[out] pResp The full received response
    @return TRUE done success
*/
bool ATMOBILE_TurnOffGPRSAndPDP(char *pResp) {
  bool isAttach;
  //__LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "Before PDP response: %s\n", pResp);
  ATUBLOX_GPRSSetPDP(GPRS_PROFILE_ID, false, pResp); // turn off PDP
  //__LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "After PDP response: %s\n", pResp);
  if (!ATUBLOX_GPRSIsAttach(&isAttach, pResp)) {
    //__LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "Get GPRS stt failed response: %s\n", pResp);
    return false;
  } //check attach status
  //__LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "After get GPRS response: %s\n", pResp);
  if (isAttach) {
    //DB_Puts("Detach GPRS...\n");
    if (!ATUBLOX_GPRSSetAttach(false, pResp)) {
      return false;
    }
  }
  return true;
};

//*************************************************************
/**
    @brief Config server parameters
    @param[in] pIP IP of the server
    @param[in] port Port of the server
    @return TRUE done success
*/
void ATMOBILE_SetRemoteServer(char *pIP, uint32_t port) {
  strcpy(sServerDomain, pIP);
  sServerPort = port;
};

//*************************************************************
/**
    @brief Upload data to server
    @param[in] pSendData Data to upload to server
    @param[out] pServerResponse The response from server
    @param[out] pResp The full received response
    @return TRUE done success
*/
bool ATMOBILE_UploadData(char *pSendData, char *pResp) {
  bool result;
  strcpy(_uri, pSendData);

  __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "Data is: %s\n", _uri);
  if (!ATUBLOX_HTTPResetParameters(HTTP_PROFILE_ID, pResp)) {
    __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "HTTP reset failed, resp %s\r\n", pResp);
    return false;
  }
//  if (!ATUBLOX_HTTPSetCustomHeader(HTTP_PROFILE_ID, 0, (char *)"Content-Type", (char *)"application/json", pResp)) {
//    __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "HTTP set header failed, resp %s\r\n", pResp);
//    return false;
//  }
  if (!ATUBLOX_HTTPSetDomain(HTTP_PROFILE_ID, sServerDomain, pResp)) {
    __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "HTTP set domain failed, resp %s\r\n", pResp);
    return false;
  }
  if (!ATUBLOX_HTTPSetRemotePort(HTTP_PROFILE_ID, sServerPort, pResp)) {
    __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "HTTP set port %d failed, resp %s\r\n", sServerPort, pResp);
    return false;
  }
  if (!ATUBLOX_HTTPGetRequest(HTTP_PROFILE_ID, _uri, (char *)FILE_SAVE_RESPONSE, &result, pResp)) {
    __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "HTTP GET request failed, resp %s\r\n", pResp);
    return false;
  }
  if (!result) {
    __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "HTTP server response failed, resp %s\r\n", pResp);
    return false;
  }
  return true;
};

//*************************************************************
/**
    @brief Perform GNSS init for uniTraKa project
    @param[out] pLastResp The last received response
    @return TRUE done success
*/
bool ATMOBILE_InitGNSSParameters(bool forNMEA, char *pLastResp) {
  if (!GPIOSetMode(GNSS_POWER_EN_PIN, GNSS_POWER_EN_PIN_TYPE, pLastResp)) {
    //DB_Printf("GNSS set PWR_EN pin fail. Resp: %s\n", pLastResp);
  }
  if (!GPIOSetMode(GNSS_DATA_READY_PIN, GNSS_DATA_READY_PIN_TYPE, pLastResp)) {
    //DB_Printf("GNSS set Data ready pin FAIL. Resp %s\n", pLastResp);
    return false;
  }
  if (!GPIOSetMode(GNSS_RTC_SHARE_PIN, GNSS_RTC_SHARE_PIN_TYPE, pLastResp)) {
    //DB_Printf("GNSS set RTC share pin FAIL. Resp %s\n", pLastResp);
    return false;
  }

  if (!GNSSSetURC(false, pLastResp)) {
    //DB_Printf("GNSS set URC FAIL. Resp %s\n", pLastResp);
    return false;
  }
  // 48 is bitmask for combining two GNSS IO config:
  // 16 is data ready function, 32 is RTC sharing function => 16|32 = 48
  if (!GNSSSetProfile(AT_UGPRF_PARA, pLastResp)) {
    //DB_Printf("GNSS set profile FAIL. Resp %s\n", pLastResp);
    return false;
  }

  //config more for later NMEA access
  if (forNMEA) {
    if (!GNSSSetRMC(true, pLastResp)) {
      //DB_Printf("GNSS set RMC FAIL. Resp %s\n", pLastResp);
      return false;
    }
    if (!TurnOnGNSSModule(pLastResp)) {
      //DB_Printf("GNSS turn on FAIL. Resp %s\n", pLastResp);
      return false;
    }
  }

  /*
	// TODO: Set GNSS sensor
	if (! GNSSConfigData(AT_ULOC_PARA_SENSOR, AT_ULOC_PARA_RESP, AT_ULOC_PARA_TIMEOUT / 1000, AT_ULOC_PARA_ACCURACY, pLastResp)){
		//DB_Printf("GNSS config data FAIL. Resp %s\n", pLastResp);	
		// fail
    //return false;
	}
	if (! GNSSSetFixData(false, pLastResp)){
    //DB_Printf("GNSS set fix data FAIL. Resp %s\n", pLastResp);	
    //return false;
  }
  
  if (! GNSSSetGeoPos(true, pLastResp)){
    //DB_Printf("GNSS set geo pos FAIL. Resp %s\n", pLastResp);	
    //return false;
  }
  if (! GNSSSetNumOfSatellites(false, pLastResp)){
    //DB_Printf("GNSS set num of satellite FAIL. Resp %s\n", pLastResp);	
    //return false;
  }

  if (! GNSSSetVTG(false, pLastResp)){
    ////DB_Printf("GNSS set VTG FAIL. Resp %s\n", pLastResp);	
    return false;
  }
  if (! GNSSSetSatelliteInfo(false, pLastResp)){
		////DB_Printf("GNSS set satellite info FAIL. Resp %s\n", pLastResp);	
    return false;
	}
 */
  return true;
};

//*************************************************************
/**
    @brief Turn on GNSS receiver module of SARA
    @param[out] pResp The full received response
    @return TRUE done success
*/
bool ATMOBILE_TurnOnGNSSModule(char *pResp) {
  // 69 is bitmask for combining three GNSS types:
  // 1 is GPS, 4 is Galileo, 64 is GLONASS => 1|4|64 = 69
  return GNSSSetPower(true, 0, AT_UGPS_ON_PARA, pResp); //on, no aid, GPS + Gallileo + Glonass
};

//*************************************************************
/**
    @brief Turn off GNSS module
    @param[out] pResp The full received response
    @return TRUE done success
*/
bool ATMOBILE_TurnOffGNSSModule(char *pResp) {
  return GNSSSetPower(false, 0, 0, pResp);
};

//*************************************************************
/**
    @brief Read data from GNSS module for uniTraKa project
    @param[out] pNMEAData The GNSS NMEA data
    @param[out] pResp The full received response
    @return TRUE done success
*/
// bool ATMOBILE_ReadNMEAData(char *pNMEAData, char *pLastResp) {

//   poll_timeout_c readTimeout(DEFAULT_READ_TIMEOUT);
//   uint32_t startRead = millis();
//   char *p;
//   if (!GNSSGetGeoPos(pLastResp)) {
//     return false;
//   }
//   p = strstr(pLastResp, "$GPGLL");
//   if (p == NULL) {
//     return false;
//   }
//   p = strtok(p, "\r\n");
//   strcpy(pNMEAData, p);
//   if (readPeriod - (millis() - startRead) > 60000) {
//     return TurnOffGNSSModule(pLastResp);
//   }
//   return true;
// };