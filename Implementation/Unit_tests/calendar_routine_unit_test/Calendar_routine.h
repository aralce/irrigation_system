/**
 * Calendar_routine.h
 */
#ifndef CALENDAR_ROUTINE_H
#define CALENDAR_ROUTINE_H

#include <cstdint>
#include <ctime>
#include <forward_list>
#include <array>
#include <iostream>

class Calendar_routine
{
public:
    virtual ~Calendar_routine(){}
    virtual bool add_event(const tm start_time, const uint32_t duration_in_minutes) = 0;
    virtual bool is_event_active(const tm time_in_event) const = 0;
    virtual bool remove_event(const tm start_time) = 0;
};

class Calendar_routine_annual : public Calendar_routine
{
public:
    Calendar_routine_annual(){}
    bool add_event(const tm start_time, const uint32_t duration_in_minutes) override;
    bool is_event_active(const tm time_in_event) const override;
    bool remove_event(const tm time_in_event);

private:
    using calendar_pair = std::array<uint32_t, 2>;
    using calendar_iter = std::forward_list<calendar_pair>::const_iterator;
    std::forward_list<calendar_pair> _events_list{};
};

#endif //CALENDAR_ROUTINE_H