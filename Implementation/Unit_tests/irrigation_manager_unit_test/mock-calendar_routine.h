/**
 * Mock for Calendar routine
*/
#ifndef MOCK_CALENDAR_ROUTINE_H
#define MOCK_CALENDAR_ROUTINE_H
#include "gmock/gmock.h"
#include <utility>
#include <ctime>

class Calendar_routine{
public:
    virtual ~Calendar_routine(){}
    virtual bool add_event(const tm& start_time, const uint32_t duration_in_minutes) = 0;
    virtual bool remove_event(const tm& time_in_event) = 0;
    virtual bool is_event_active(const tm& time_in_event) = 0;
    virtual bool get_next_event(std::pair<tm, uint32_t>& event_to_return) = 0;
    virtual bool set_get_event(const tm& time_to_set_index) = 0;
    virtual void reset_get_event() = 0;
};

class MockCalendar_routine: public Calendar_routine{
public:
    MOCK_METHOD(bool, add_event, (const tm& start_time, const uint32_t duration_in_minutes), (override));
    MOCK_METHOD(bool, remove_event, (const tm& time_in_event), (override));
    MOCK_METHOD(bool, is_event_active, (const tm& time_in_event), (override));
    MOCK_METHOD(bool, get_next_event, ((std::pair<tm, uint32_t>&) event_to_return), (override));
    MOCK_METHOD(bool, set_get_event, (const tm& time_to_set_index), (override));
    MOCK_METHOD(void, reset_get_event, (), (override));
};

#endif
