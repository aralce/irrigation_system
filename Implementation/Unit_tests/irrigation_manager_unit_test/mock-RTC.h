/**
 *  RTC mock for STM32
 */
#ifndef MOCK_RTC_H
#define MOCK_RTC_H
#include "gmock/gmock.h"
#include <ctime>

class RTC{
public:
    virtual ~RTC(){}
    virtual void get_time(tm& time) = 0;
};

class MockRTC : public RTC{
public:
    MOCK_METHOD(void, get_time, (tm& time), (override));
};

#endif