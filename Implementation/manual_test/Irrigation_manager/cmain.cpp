
#include "cmain.h"
#include "Actuator_manual_test.h"
#include "cmsis_os.h"
#include "Sensor.h"
#include "Sensor_manual_test.h"
#include "irrigation_zone.h"
#include "irrigation_zone_manual_test.h"
#include "Irrigation_manager_manual_test.h"

#include "fatfs.h"
#include "ffconf.h"
#include <cstring>
#include "RTC.h"

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
	irrigation_manager_manual_test_init();
}

void cmain()
{
	irrigation_manager_manual_test_run_2();
}



