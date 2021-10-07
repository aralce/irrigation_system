
#include "cmain.h"
#include "../../manual_test/Actuator/Actuator_manual_test.h"
#include "cmsis_os.h"
#include "../../classes/Sensor.h"
#include "../../manual_test/Sensor/Sensor_manual_test.h"
#include "../../classes/irrigation_zone.h"
#include "../../manual_test/irrigation_zone/irrigation_zone_manual_test.h"

void * operator new( size_t size )
{
    return pvPortMalloc(size);
}

void * operator new[]( size_t size )
{
    return pvPortMalloc( size );
}

void operator delete( void * ptr )
{
    vPortFree( ptr );
}

void operator delete[]( void * ptr )
{
    vPortFree( ptr );
}


/*User GPIO Initialization*/
void init_main()
{
	irrigation_zone_manual_test_init();
}

void cmain()
{
	irrigation_zone_manual_test_run();
}



