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
#include <memory>

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
    friend class Calendar_debug;
    std::forward_list<std::array<uint32_t, 2>> _events_list{};
    int _routines_quantity = 0;
};

class Calendar_debug
{
public:
    static int get_routines_quantity(Calendar_routine *calendar){
        Calendar_routine_annual *calendar_annual_ptr = static_cast<Calendar_routine_annual *>(calendar);
        return calendar_annual_ptr->_routines_quantity;
    }
};
#endif //CALENDAR_ROUTINE_H