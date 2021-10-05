#include "irrigation_zone.h"
int Irrigation_zone::_zones_irrigating = 0;

/*Alias*/
static constexpr bool ON  = true;
static constexpr bool OFF = false;
static constexpr int ONLY_THIS = 1;

/*Function Definition*/
bool Irrigation_zone::is_sensing_health()
{
    if( _electrovalve.has_sensor() && _pump.has_sensor() )
        return true;
    else
        return false;
}

void Irrigation_zone::irrigate(bool must_irrigate)
{
    if(must_irrigate && _electrovalve.is_ON() == false) 
    {   
        _electrovalve.set(ON);
        ++_zones_irrigating;
    }
    else if(must_irrigate == false && _electrovalve.is_ON())
    {   
        _electrovalve.set(OFF);
        --_zones_irrigating;
    }
    
    if (_pump.is_ON() == false) 
        _pump.set(ON);
    else if( must_irrigate == false && _zones_irrigating == ONLY_THIS)
        _pump.set(OFF);
}