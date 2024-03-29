#include "irrigation_zone.h"
#include <limits>
int Irrigation_zone::_zones_irrigating = 0;

/*Alias*/
static constexpr bool ON  = true;
static constexpr bool OFF = false;
static constexpr int NONE = 0;

/*Function Definition*/
bool Irrigation_zone::is_irrigating(){ 
    if(_electrovalve == nullptr || _pump == nullptr)
        return false;
    return _electrovalve->is_ON(); 
}

void Irrigation_zone::irrigate(bool must_irrigate)
{
    if( _electrovalve == nullptr || _pump == nullptr)
        return;
    if(must_irrigate && _electrovalve->is_ON() == false){   
        _electrovalve->set(ON);
        ++_zones_irrigating;
    }
    else if(must_irrigate == false && _electrovalve->is_ON()){   
        _electrovalve->set(OFF);
        --_zones_irrigating;
    }   
    if(must_irrigate == true && _pump->is_ON() == false) 
        _pump->set(ON);
    else if( must_irrigate == false && _zones_irrigating == NONE)
        _pump->set(OFF);
}

bool Irrigation_zone::is_sensing_health()
{
    if( _electrovalve == nullptr || _pump == nullptr)
        return false;
    if( _electrovalve->has_sensor() && _pump->has_sensor() )
        return true;
    return false;
}

bool Irrigation_zone::is_healthy()
{
    if( _electrovalve == nullptr || _pump == nullptr)
        return false;
    constexpr uint32_t max = std::numeric_limits<uint32_t>::max();
    if( is_sensing_health() == false )
        return true;   
    uint32_t electrovalve_read = _electrovalve->read_sensor();
    uint32_t pump_read = _pump->read_sensor();
    if( pump_read == max &&  electrovalve_read == max )
        return true;
    else if( pump_read - electrovalve_read <= HEALTHY_OFFSET )
        return true;
    
    return false;
}