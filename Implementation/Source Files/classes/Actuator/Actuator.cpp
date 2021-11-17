/**
 * Actuator.cpp
 * Actuator is an interface to control any type of actuator.
 * Each actuator has its own class.
*/
#include "Actuator.h"

void Actuator_ON_OFF::set( bool set_ON )
{
	_is_ON = set_ON;
	HAL_GPIO_WritePin(_gpio, _pin, set_ON ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

bool Actuator_ON_OFF::remove_sensor()
{
	if(_has_sensor)
	{
		delete _sensor_ptr;
		_sensor_ptr = nullptr;
		_has_sensor = false;
		return false;
	}
	else
		return true;
};
