/**
 * Sensor.h
 * Sensor is an interface to control any type of sensor.
 * Each sensor has its own class.
*/
#include "sensor.h"
#include <limits>

uint32_t sensor_switch::read()
{
	return HAL_GPIO_ReadPin(_gpio, _pin) ? std::numeric_limits<uint32_t>::max() : 0;

}
