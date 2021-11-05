#include "Irrigation_manager.h"

Irrigation_manager::Irrigation_manager(Irrigation_zone* zones[], Calendar_routine* calendar[]){
    for(auto i=0; i<MAX_ZONES; ++i){
        _zones[i] = zones[i];
        _calendar[i] = calendar[i];
    }
}

bool Irrigation_manager::irrigate(uint8_t zone_from_0, bool must_irrigate){
    if(zone_from_0 >= MAX_ZONES)
        return false;
    _zones[zone_from_0]->irrigate(must_irrigate);
    return true;
}

bool Irrigation_manager::is_healthy(uint8_t* zone_with_error = nullptr){
    bool are_zones_ok = true;
    for(auto i=0; i<MAX_ZONES; ++i){
        are_zones_ok &= _zones[i]->is_healthy();
        if(!are_zones_ok){
            if(zone_with_error != nullptr) 
                *zone_with_error = i;
            break;
        }
    }
    return are_zones_ok;
}

bool Irrigation_manager::add_event(uint8_t zone_from_0, const tm start_time, const uint32_t duration_in_minutes){
    if(zone_from_0 >= MAX_ZONES)
        return false;
    return _calendar[zone_from_0]->add_event(start_time, duration_in_minutes);
}