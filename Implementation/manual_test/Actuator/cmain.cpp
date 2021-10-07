
#include "cmain.h"

#include "../../classes/sensor.h"
#include "cmsis_os.h"

#include "../Sensor/Sensor_manual_test.h"
#include "Actuator_manual_test.h"

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
	actuator_manual_test_init_with_sensor();
}

void cmain()
{
	actuator_manual_test_run();
}



