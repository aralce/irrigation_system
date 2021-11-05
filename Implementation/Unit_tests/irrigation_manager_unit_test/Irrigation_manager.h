/**
 * Irrigation_manager.h
*/
#ifndef IRRIGATION_MANAGER_H
#define IRRIGATION_MANAGER_H
#include <ctime>
#include "Irrigation_manager_config.h"
#include "mock-irrigation_zone.h"
#include "mock-calendar_routine.h"

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
private:
    Irrigation_zone* _zones[MAX_ZONES];
    Calendar_routine* _calendar[MAX_ZONES];
};

#endif