/**
 * Irrigation_manager.h
*/
#ifndef IRRIGATION_MANAGER_H
#define IRRIGATION_MANAGER_H
#include <ctime>
#include "Irrigation_manager_config.h"
#include "irrigation_zone.h"
#include "Calendar_routine.h"
#include "RTC.h"

class Irrigation_manager{
public:
    //Copy operations deleted for use of internal pointers.
    Irrigation_manager() = delete;
    Irrigation_manager(Irrigation_manager& ) = delete;
    Irrigation_manager& operator=(Irrigation_manager&) = delete;
    Irrigation_manager(Irrigation_zone* zone[], Calendar_routine* calendar[]);
    //Member functions
    bool irrigate(uint8_t zone_from_0, bool must_irrigate);
    bool is_healthy(uint8_t* zone_with_error);
    bool add_event(uint8_t zone_from_0, const tm start_time, const uint32_t duration_in_minutes);
    bool remove_event(uint8_t zone_from_0, const tm time_in_event);
    void process_events(RTC_class& rtc);
    bool get_next_event(uint8_t zone_from_0, std::pair<tm, uint32_t>& event_to_return);
    bool set_get_event(uint8_t zone_from_0, tm time_to_set_index);
    bool reset_get_event(uint8_t zone_from_0);
private:
    Irrigation_zone* _zones[MAX_ZONES];
    Calendar_routine* _calendar[MAX_ZONES];
};

#endif
