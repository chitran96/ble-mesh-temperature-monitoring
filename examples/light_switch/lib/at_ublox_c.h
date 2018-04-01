/**
 @file at_low_c.h
 @version 1.0
 @date 2017-12-12
 @brief Define API to communicate with a UBlox SARA-U201
 @attention The maximum length of the command line is 1024 characters
 @author Bui Van Hieu <vanhieubk@gmail.com>
*/

#ifndef __AT_UBLOX_C_H
#define __AT_UBLOX_C_H

#include "at_low_c.h"
#include <string.h>

#define NUM_SUPPORT_AT_CMD			(47)
#define MAX_MSG_LENGTH					(400)
#define MAX_GET_RESPONSE_LENGTH	(700)

typedef enum{
	AT_UBLOX_ATE = 0, 				// turn on turn off echo
	/* General commands */	
	AT_UBLOX_CGMR = 1,        // read firmware version
	AT_UBLOX_CGSN = 2,        // read IMEI
	AT_UBLOX_CIMI = 29,       // read IMSI
	AT_UBLOX_CCID = 31,       // read CCID
	AT_UBLOX_COPS = 44,       // select operator
	AT_UBLOX_CREG = 45,       // network registration status
	/* SMS commands */
	AT_UBLOX_CSMS = 3,        // read supported message service, set to 1
	AT_UBLOX_CPMS = 4,        // set recv msg storage
	AT_UBLOX_CNMI = 5,				// set recv msg notify
	AT_UBLOX_CMGR = 6,        // read msg
	AT_UBLOX_CMGD = 7,        // delete msg
	AT_UBLOX_CMGF = 30,       // set text mode
	AT_UBLOX_CMGL = 32,       // list msg
	AT_UBLOX_CSDH = 46,       // config msg header
	/* GPIO commands */
	AT_UBLOX_UGPIOC = 8,      // GPIO config
	AT_UBLOX_UGPIOR = 9,      // read GPIO
	AT_UBLOX_UGPIOW = 10,     // set GPIO
	/* DNS commands */
	AT_UBLOX_UDNSRN = 11, 
	AT_UBLOX_UDYNDNS = 12,
	/* GPRS commands */
	AT_UBLOX_CGATT = 33,      // check GPRS attachment
	AT_UBLOX_UPSD = 34,       // config APN
	AT_UBLOX_UPSDA = 35,      // set PDP context
	AT_UBLOX_USOCR = 36,      // set socket
	AT_UBLOX_USOCO = 37,      // open TCP
	AT_UBLOX_USOWR = 38,      // send data
	AT_UBLOX_USOCL = 39,			// close TCP
	/* HTTP commands */
	AT_UBLOX_UHTTP = 40,      // set HTTP parameters
	AT_UBLOX_UHTTPC = 41,     // send HTTP request
	/* Flash commands */
	AT_UBLOX_URDFILE = 42,    // read file stored in flash
	AT_UBLOX_UDELFILE = 43,   // delete file stored in flash
	/* GNSS commands */
	AT_UBLOX_UGPS = 13,       // GPS power management
	AT_UBLOX_UGIND = 14,      // URC mode
	AT_UBLOX_UGPRF = 15,      // config GNSS profile
	AT_UBLOX_UGUBX = 16,
	AT_UBLOX_UGTMR = 17,
	AT_UBLOX_UGZDA = 18,
	AT_UBLOX_UGGGA = 19,      // fix data
	AT_UBLOX_UGGLL = 20,      // geographic position
	AT_UBLOX_UGGSV = 21,      // number of GNSS satellites in view
	AT_UBLOX_UGRMC = 22,      // recommended minimum GNSS data
	AT_UBLOX_UGVTG = 23,      // course over ground and ground speed
	AT_UBLOX_UGGSA = 24,      // satellite information
	AT_UBLOX_ULOC = 25,       // config ULOC
	AT_UBLOX_UTIME = 26,
	AT_UBLOX_ULOCIND = 27,
	AT_UBLOX_ULOCGNSS = 28,   // config GNSS sensor
	MAX_AT_INDEX = NUM_SUPPORT_AT_CMD
} at_ublox_cmd_t;

typedef struct {
	char code[15];
	uint32_t timeout;
} at_cmd_info_t;

/* general commands */
bool ATUBLOX_PingTest(char* pResp);
bool ATUBLOX_SetEcho(bool isEnable, char* pResp);
bool ATUBLOX_ReadFirmware(char* pFirmVer, char* pResp);
bool ATUBLOX_ReadIMEI(char* pIMEI, char* pResp);
bool ATUBLOX_ReadIMSI(char* pIMSI, char* pResp);
bool ATUBLOX_ReadSIMID(char* pSIMID, char* pResp);
bool SetPowerMode(uint8_t powerMode, int32_t awakeTime, char* pResp);
bool SwitchOff(char* pResp);

bool ATUBLOX_SetOperator(uint8_t mode, char* pResp);
bool ATUBLOX_ReadNetworkRegistrationStatus(uint8_t* stt, char* pResp);
bool ATUBLOX_ReadNetworkOperator(char* pInfo, char* pResp);

/* SMS commands ..... */
bool ATUBLOX_SMSSetTextMode(bool isEnable, char* pResp);
bool ATUBLOX_SMSSetShowHeader(bool isEnable, char* pResp);
bool ATUBLOX_SMSSelectService(bool isPhase2Plus, char* pResp);
bool ATUBLOX_SMSSetNewIndication(uint32_t indicateMode, char* pResp);
bool ATUBLOX_SMSSetWaitingIndicaition(bool isEnable, char* pResp);
bool ATUBLOX_SMSSetPreferredStorage(bool onSIM, char* pResp);
bool ATUBLOX_SMSSaveSetting(char* pResp);
bool ATUBLOX_SMSReadAt(uint8_t index, char* pSentNum, char* pContent, char* pResp);

bool ATUBLOX_SMSReadOldest(bool* hasSms, int32_t* pIndex, char* pSentNum, char* pContent, char* pResp);

bool ATUBLOX_SMSDeleteAt(int32_t index, char* pResp);
bool ATUBLOX_SMSDeleteAll(bool includeUnread, char* pResp);

/* GPIO commands */
bool ATUBLOX_GPIOSetMode(uint16_t pinIndex, uint16_t pinMode, char* pResp);
bool ATUBLOX_GPIORead(uint16_t pinIndex, bool* pReadVal, char* pResp);
bool ATUBLOX_GPIOSet(uint16_t pinIndex, bool setVal, char* pResp);

/* DNS commands */
bool ATUBLOX_DNSResolveURL(char* pURL, char* pResolvedIP, char* pResp);
bool ATUBLOX_DNSSetDynamicUpdate(bool isEnable, char* pResp);

/* GPRS commands */
bool ATUBLOX_GPRSSetAttach(bool isAttach, char* pResp);
bool ATUBLOX_GPRSIsAttach(bool* pResult, char* pResp);

bool ATUBLOX_GPRSSetAuthentication(uint8_t profileId, uint8_t type, char* pResp);
bool ATUBLOX_GPRSSetAPN(uint8_t profileId, char* pAPN, char* pResp);
bool ATUBLOX_GPRSSetUsername(uint8_t profileId, char* pUserName, char* pResp);
bool ATUBLOX_GPRSSetPassword(uint8_t profileId, char* pPassword, char* pResp);


bool ATUBLOX_GPRSSetPDP(uint8_t profileId, bool isActive, char* pResp);

/* HTTP commands */
bool ATUBLOX_HTTPResetParameters(uint8_t httpID, char* pResp);
bool ATUBLOX_HTTPSetDomain(uint8_t httpID, char* domain, char* pResp);
bool ATUBLOX_HTTPSetRemotePort(uint8_t httpID, uint32_t port, char* pResp);
bool ATUBLOX_HTTPSetUsername(uint8_t httpID, char* pUsername, char* pResp);
bool ATUBLOX_HTTPSetPassword(uint8_t httpID, char* pPassword, char* pResp);
bool ATUBLOX_HTTPSetAuthentication(uint8_t httpID, bool isEnable, char* pResp);
bool ATUBLOX_HTTPSetCustomHeader(uint8_t httpID, uint8_t headerIdx, char* headerName, char* headerVal, char* pResp);
bool ATUBLOX_HTTPGetRequest(uint8_t httpID, char* pUri, char* pSaveFileName, bool* pResult, char* pResp); 
/* dummy implement. need change later */ bool HTTPParseResponse(char* fileName, char* pResp);

/* Flash commmands */
bool ATUBLOX_FlashReadFile(char* fileName, char* pResp);
bool ATUBLOX_FlashDelFile(char* fileName, char* pResp);

/* GNSS commands */
bool ATUBLOX_GNSSSetPower(bool isOn, uint8_t aidMode, uint32_t GNSSSystem, char* pResp);
bool ATUBLOX_GNSSSetURC(bool isEnable, char* pResp);
bool ATUBLOX_GNSSSetProfile(uint32_t code, char* pResp);
bool ATUBLOX_GNSSSetSensor(void);
bool ATUBLOX_GNSSConfigData(uint8_t sensor, uint8_t responseType, uint32_t timeout, uint16_t accuracy, char* pResp);
bool ATUBLOX_GNSSSetFixData(bool isEnable, char* pResp);
bool ATUBLOX_GNSSGetFixData(char* pResp);
bool ATUBLOX_GNSSSetGeoPos(bool isEnable, char* pResp);
bool ATUBLOX_GNSSGetGeoPos(char* pResp);
bool ATUBLOX_GNSSSetNumOfSatellites(bool isEnable, char* pResp);
bool ATUBLOX_GNSSGetNumOfSatellites(char* pResp);
bool ATUBLOX_GNSSSetRMC(bool isEnable, char* pResp);
bool ATUBLOX_GNSSGetRMC(char* pResp);
bool ATUBLOX_GNSSSetVTG(bool isEnable, char* pResp);
bool ATUBLOX_GNSSGetVTG(char* pResp);
bool ATUBLOX_GNSSSetSatelliteInfo(bool isEnable, char* pResp);
bool ATUBLOX_GNSSGetSatelliteInfo(char* pResp);

bool ATUBLOX_GNSSGetULOCData(bool byGNSS, uint32_t timeOutInSec, uint32_t accuracyInMet, char* pData, char* pResp);

/* general read commands */
bool ATUBLOX_Read(char* pATCmd, uint32_t timeOut, char* pWait, char* pResp);
bool ATUBLOX_ReadNoQuestionMark(char* pATCmd, uint32_t timeOut, char* pWait, char* pResp);


#endif /*  __AT_UBLOX_C_H */

