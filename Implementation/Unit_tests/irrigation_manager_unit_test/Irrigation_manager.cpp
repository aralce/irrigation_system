#include "Irrigation_manager.h"

Irrigation_manager::Irrigation_manager(Irrigation_zone* zones[]){
    for(auto i=0; i<MAX_ZONES; ++i)
        _zones[i] = zones[i];
}