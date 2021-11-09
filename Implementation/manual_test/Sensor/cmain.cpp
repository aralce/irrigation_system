
#include "cmain.h"

#include "Sensor.h"
#include "cmsis_os.h"
#include "Sensor_manual_test.h"

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
	sensor_switch_test_init();
}

void cmain()
{
	sensor_switch_test_run();
}



