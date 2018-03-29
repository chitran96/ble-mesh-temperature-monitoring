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
#include "poll_timeout_c.h"
#include "arduino_port.h"

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

#define MOBI_APN							          ("m-wap")
#define MOBI_USERNAME						        ("mms")
#define MOBI_PASSWORD						        ("mms")
#define VINA_APN                        ("m3-world")
#define VINA_USERNAME                   ("mms")
#define VINA_PASSWORD                   ("mms")
#define VIETTEL_APN											("v-internet")
#define VIETTEL_USERNAME								("")
#define VIETTEL_PASSWORD								("")


#define SERVER_DOMAIN_DEFAULT						 ("34.230.4.191")
#define SERVER_PORT_DEFAULT 	      	    (7000)

#define FILE_SAVE_RESPONSE				      ("get.ffs")


typedef enum{
  ULOC_TYPE_GNSS=0,
  ULOC_TYPE_CELLLOCATE
} uloc_type_t;


class at_mobile_c: public at_ublox_c {

private:
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

public:
	/* revised */ at_mobile_c(void);
	
	bool SetReadPeriod(uint32_t newPeriod);
	uint32_t GetReadPeriod(void);

	/* revised */ bool MOBIReinit(char* pLastResp);
	/* revised */ bool MOBIReadIdentify(char* pLastResp);
	/* revised */ bool MOBIRegisterNetwork(char* pLastResp);
  bool MOBIDeRegisterNetwork(char* pLastResp);
  bool MOBIWaitRegisterNetwork(char* pLastResp);
	/* revised */ bool MOBISMSReinit(char* pLastResp);

  bool ReadSMS(bool* pHasSms, int32_t* pSMSIndex, char* pSentNum, char* pContent, char* pLastResp);
  bool DeleteAllSMS(char* pLastResp);

  void SetRemoteServer(char* pIP, uint32_t port);
  /* revised */ bool TurnOnGPRSAndPDP(char* pResp);
	/* revised */ bool TurnOffGPRSAndPDP(char* pResp);
	/* revised */ bool UploadData(uloc_type_t ulocType, char* pSendData, char* pServerResponse, char* pResp);

	bool InitGNSSParameters(bool forNMEA, char* pLastResp);
	bool TurnOnGNSSModule(char* pResp);
	bool TurnOffGNSSModule(char* pResp);
	bool ReadNMEAData(char* pNMEAData, char* pLastResp);
}; /* end class */


#endif /*  __AT_UBLOX_C_H */

