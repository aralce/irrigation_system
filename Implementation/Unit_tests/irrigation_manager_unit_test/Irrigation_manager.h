/**
 * Irrigation_manager.h
*/
#ifndef IRRIGATION_MANAGER_H
#define IRRIGATION_MANAGER_H
#include "mock-irrigation_zone.h"

constexpr int MAX_ZONES = 8;

class Irrigation_manager{
public:
    //Copy operations deleted for use of internal pointers.
    Irrigation_manager() = delete;
    Irrigation_manager(Irrigation_manager& ) = delete;
    Irrigation_manager& operator=(Irrigation_manager&) = delete;
    Irrigation_manager(Irrigation_zone* zone[]);
    //Member functions
    void irrigate(uint8_t zone_from_0, bool must_irrigate){ _zones[zone_from_0]->irrigate(must_irrigate); }
private:
    Irrigation_zone* _zones[MAX_ZONES];
};

#endif