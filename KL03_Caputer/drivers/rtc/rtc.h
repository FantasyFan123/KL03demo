/*
 * File:		rtc.h
 * Purpose:     Provide common rtc functions
 *
 * Notes:
 */
#include "common.h"
#ifndef __RTC_H__
#define __RTC_H__

extern uint32_t g_ulCurrentTimeCount;

/********************************************************************/

void rtc_init(uint32 seconds, uint32 alarm, uint8 c_interval, uint8 c_value, uint8 interrupt);      
void rtc_isr(void);
void rtc_second_isr(void); 
void rtc_reg_report (void);
void rtc_reset( void );

uint32_t rtc_updated_timer( void );
/********************************************************************/

#endif /* __RTC_H__ */
