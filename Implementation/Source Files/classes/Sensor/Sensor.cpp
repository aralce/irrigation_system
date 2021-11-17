/**
 * Sensor.h
 * Sensor is an interface to control any type of sensor.
 * Each sensor has its own class.
*/
#include "Sensor.h"

#include <limits>

int Sensor_switch::_instances_number = 0;

uint32_t Sensor_switch::read()
{
	bool raw_read = _is_normal_open ? !HAL_GPIO_ReadPin(_gpio, _pin) : HAL_GPIO_ReadPin(_gpio, _pin);
	return raw_read ? std::numeric_limits<uint32_t>::max() : 0;
}
