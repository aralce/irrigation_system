
#include "cmain.h"
#include "../../classes/sensor.h"
#include "../../manual_test/sensor_manual_test.h"
#include "../../manual_test/actuator_manual_test.h"
#include "cmsis_os.h"

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



