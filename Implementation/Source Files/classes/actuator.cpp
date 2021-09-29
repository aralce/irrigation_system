/**
 * Actuator.cpp
 * Actuator is an interface to control any type of actuator.
 * Each actuator has its own class.
*/
#include "actuator.h"

void actuator_ON_OFF::set( bool set_ON )
{
	HAL_GPIO_WritePin(_gpio, _pin, set_ON ? GPIO_PIN_SET : GPIO_PIN_RESET);
}
