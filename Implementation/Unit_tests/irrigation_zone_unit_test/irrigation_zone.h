#include "actuator_mock.h"

class Irrigation_zone
{
public:
    explicit Irrigation_zone(Actuator &electrovalve, Actuator &pump) : _electrovalve(electrovalve), _pump(pump) {}
    Irrigation_zone() = delete;

    bool is_sensing_health();
    bool is_irrigating();
    void irrigate(bool must_irrigate);
private:
    Actuator &_electrovalve;
    Actuator &_pump;
    static int _zones_irrigating;
    friend class Debug;
};

class Debug
{
public:
    static Actuator& get_electrovalve(Irrigation_zone& irr){ return irr._electrovalve;}
    static Actuator& get_pump(Irrigation_zone& irr){ return irr._pump;}
    static int get_nzones_irrigating(){ return Irrigation_zone::_zones_irrigating; }
};
