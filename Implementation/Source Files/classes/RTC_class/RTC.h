/**
 * RTC.h
 * */
#ifndef RTC_H
#define RTC_H
#include <ctime>
#include "stm32f4xx_hal.h"


class RTC_class{
public:
	RTC_class(RTC_HandleTypeDef& hrtc) : _hrtc(hrtc) {}
	void get_time(tm& time);
private:
	RTC_HandleTypeDef& _hrtc;
};

#endif //RTC__H
