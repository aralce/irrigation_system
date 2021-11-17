/**
 * RTC.cpp
 * */
#include "RTC.h"
#include "cmsis_os.h"

void RTC_class::get_time( tm& time ){
	RTC_TimeTypeDef _sTime = {0};
	RTC_DateTypeDef _sDate = {0};
	HAL_RTC_GetDate(&_hrtc, &_sDate, RTC_FORMAT_BIN);
	HAL_RTC_GetTime(&_hrtc, &_sTime, RTC_FORMAT_BIN);
	time.tm_hour = _sTime.Hours;
	time.tm_min  = _sTime.Minutes;
	time.tm_sec  = _sTime.Seconds;
	time.tm_wday = _sDate.WeekDay - 1;
	time.tm_mon  = _sDate.Month - 1;
	time.tm_mday = _sDate.Date;
	time.tm_year = _sDate.Year + 100;
}
