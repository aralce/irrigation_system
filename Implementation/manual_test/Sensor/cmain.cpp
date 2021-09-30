
#include "cmain.h"
#include "../../classes/sensor.h"
#include "../../manual_test/sensor_manual_test.h"
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
	sensor_switch_test_init();
}

void cmain()
{
	sensor_switch_test_run();
}



