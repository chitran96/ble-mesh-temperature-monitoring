/**
@file at_mobile_c.h
@version 1.0
@date 2017-12-14
@brief Define API to control a mobile module which supports AT command
@attention The maximum length of the command line is 1024 characters
@author Bui Van Hieu <vanhieubk@gmail.com>
*/

#ifndef __AT_MOBILE_C_H
#define __AT_MOBILE_C_H

#include "at_ublox_c.h"

#define MAX_IMEI_LENGTH						      (32u)
#define MAX_IMSI_LENGTH						      (32u)
#define MAX_SIMID_LENGTH					      (32u)
#define MAX_APN_LENGTH						      (32u)
#define MAX_USERNAME_LENGTH             (32u)
#define MAX_PASSWORD_LENGTH             (32u)
#define MAX_DOMAIN_LENGTH               (32u)
#define MAX_URI_LENGTH                  (300)

#define AT_SYNC_TIMES					          (6)

#define TIMEOUT_WAIT_REGISTER			      (30000)

#define AT_CPOS_PARA						        (0)
#define AT_UGPRF_PARA						        (48)
#define AT_UGPS_ON_PARA						      (69)
#define AT_ULOC_PARA_SENSOR				      (1)
#define AT_ULOC_PARA_RESP					      (1)
#define AT_ULOC_PARA_TIMEOUT				    (10000)
#define AT_ULOC_PARA_ACCURACY			      (100)

#define GNSS_POWER_EN_PIN               (23)
#define GNSS_POWER_EN_PIN_TYPE          (3)     /**! GNSS supply enable */
#define GNSS_DATA_READY_PIN				      (24)
#define GNSS_DATA_READY_PIN_TYPE			  (4)
#define GNSS_RTC_SHARE_PIN				      (25)
#define GNSS_RTC_SHARE_PIN_TYPE				  (5)

#define DEFAULT_READ_TIMEOUT			      (30000)

#define GPRS_PROFILE_ID						      (0)
#define GPRS_AUTH_TYPE									(3) // automatic
#define GPRS_APN_STREAM                 ("stream.co.uk")
#define GPRS_USERNAME_STREAM            ("streamip")
#define GPRS_PASSWORD_STREAM            ("streamip")

#define HTTP_PROFILE_ID						      (0)

#define MOBI_APN                        ("m-wap")
#define MOBI_USERNAME			("mms")
#define MOBI_PASSWORD			("mms")
#define VINA_APN                        ("m3-world")
#define VINA_USERNAME                   ("mms")
#define VINA_PASSWORD                   ("mms")
#define VIETTEL_APN                     ("v-internet")
#define VIETTEL_USERNAME		("")
#define VIETTEL_PASSWORD		("")


#define SERVER_DOMAIN_DEFAULT		("api.thingspeak.com")
#define SERVER_PORT_DEFAULT 	      	(80)

#define FILE_SAVE_RESPONSE				      ("get.ffs")


typedef enum{
ULOC_TYPE_GNSS=0,
ULOC_TYPE_CELLLOCATE
} uloc_type_t;


void ATMOBILE_Init(void);

bool ATMOBILE_SetReadPeriod(uint32_t newPeriod);
uint32_t ATMOBILE_GetReadPeriod(void);

bool ATMOBILE_MOBIReinit(char* pLastResp);
bool ATMOBILE_MOBIReadIdentify(char* pLastResp);
bool ATMOBILE_MOBIRegisterNetwork(char* pLastResp);
bool ATMOBILE_MOBIDeRegisterNetwork(char* pLastResp);
bool ATMOBILE_MOBIWaitRegisterNetwork(char* pLastResp);
bool ATMOBILE_MOBISMSReinit(char* pLastResp);

bool ATMOBILE_ReadSMS(bool* pHasSms, int32_t* pSMSIndex, char* pSentNum, char* pContent, char* pLastResp);
bool ATMOBILE_DeleteAllSMS(char* pLastResp);

void ATMOBILE_SetRemoteServer(char* pIP, uint32_t port);
bool ATMOBILE_TurnOnGPRSAndPDP(char* pResp);
bool ATMOBILE_TurnOffGPRSAndPDP(char* pResp);
bool ATMOBILE_UploadData(char* pSendData, char* pResp);

bool ATMOBILE_InitGNSSParameters(bool forNMEA, char* pLastResp);
bool ATMOBILE_TurnOnGNSSModule(char* pResp);
bool ATMOBILE_TurnOffGNSSModule(char* pResp);
bool ATMOBILE_ReadNMEAData(char* pNMEAData, char* pLastResp);


#endif /*  __AT_UBLOX_C_H */
