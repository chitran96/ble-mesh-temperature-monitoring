/**
 @file systmr.cpp
 @version 1.0
 @date 2018-3-27
 @brief System timer API
 @attention The library use app_timer (RTC1)
*/

#include "systmr.h"


///////////////////////////////////////////////////////////////////////////////
static uint32_t       _baseTimerCounter;

///////////////////////////////////////////////////////////////////////////////
//*************************************************************
/**
    @brief Reinit and start system timer
    @return TRUE done success
    @attention Haven't restart timer counter to zero yet
*/
bool SYSTMR_Reinit(void){
  // nothing to do, but need to call app_timer_init first in main loop
  return true;
}


//*************************************************************
/**
    @brief Get system timer value in second
    @return value
*/
uint32_t SYSTMR_Second(void){
  return SYSTMR_GetNumTick() / SYSTMR_NUM_TICK_PER_SEC;
}

//*************************************************************
/**
    @brief Get number of elapsed ticks
    @return Number of ticks
*/
uint32_t SYSTMR_GetNumTick(void){
  return _baseTimerCounter + app_timer_cnt_get();
}