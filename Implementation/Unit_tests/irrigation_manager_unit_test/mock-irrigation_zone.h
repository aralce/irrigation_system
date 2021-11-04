/**
 * Mock for Irrigation Zone
*/
#ifndef MOCK_IRRIGATION_ZONE_H
#define MOCK_IRRIGATION_ZONE_H
#include "gmock/gmock.h"

class Irrigation_zone{
public:
    virtual ~Irrigation_zone(){}
    virtual bool is_irrigating() = 0;
    virtual void irrigate(bool must_irrigate) = 0;
    virtual bool is_sensing_health() = 0;
    virtual bool is_healthy() = 0;
};

class MockIrrigation_zone: public Irrigation_zone{
public:
    MOCK_METHOD(bool, is_irrigating, (), (override));
    MOCK_METHOD(void, irrigate, (bool must_irrigate), (override));
    MOCK_METHOD(bool, is_sensing_health, (), (override));
    MOCK_METHOD(bool, is_healthy, (), (override));    
};

#endif
