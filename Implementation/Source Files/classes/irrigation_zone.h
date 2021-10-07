#include "actuator.h"
/**Irrigation Zone (IS NOT THREAD SAFE)
 * 
 * The irrigation zone is responsible to control the field irrigation.
 * A irrigation zone is associated with one electrovalve and a shared pump.
 * 
*/
#include "irrigation_zone_config.h"

class Irrigation_zone
{
public:
    explicit Irrigation_zone(Actuator *electrovalve, Actuator *pump) : _electrovalve(electrovalve), _pump(pump) {}
    Irrigation_zone() = delete;
    bool is_irrigating(){ return _electrovalve->is_ON(); }
    void irrigate(bool must_irrigate);
    bool is_sensing_health();
    bool is_healthy();
private:
    Actuator *_electrovalve;
    Actuator *_pump;
    static int _zones_irrigating;
    friend class Debug;
};

class Debug
{
public:
    static Actuator* get_electrovalve(Irrigation_zone *irr){ return irr->_electrovalve;}
    static Actuator* get_pump(Irrigation_zone *irr){ return irr->_pump;}
    static int get_nzones_irrigating(){ return Irrigation_zone::_zones_irrigating; }
};
