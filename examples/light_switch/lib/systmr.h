/**
 @file systmr.h
 @version 1.0
 @date 2018-3-27
 @brief System timer API
 @attention The library use app_timer (RTC1)
*/

#ifndef __SYS_TMR_H
#define __SYS_TMR_H

#include <stdint.h>
#include "app_timer.h"

#define SYSTMR_NUM_MSEC_PER_TICK    (125)
#define SYSTMR_NUM_TICK_PER_SEC     (8)

bool SYSTMR_Reinit(void);
uint32_t SYSTMR_Second(void);
uint32_t SYSTMR_GetNumTick(void);


#endif /*  __SYS_TMR_H */
