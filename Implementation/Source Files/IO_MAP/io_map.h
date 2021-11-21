/**
 * INPUT and OUTPUT map
 * */
#ifndef IO_MAP_H
#define IO_MAP_H

#include "stm32f4xx_hal.h"
#include "Irrigation_manager_config.h"

constexpr int MAX_SENSORS = MAX_ZONES + 1;
constexpr int MAX_ACTUATORS = MAX_SENSORS;

typedef struct{
	GPIO_TypeDef* port;
	uint16_t pin;
} io_t;

/**
 * SENSOR INPUTS
 * the sensor in [MAX_SENSORS-1] is the pump sensor
 * */
extern io_t io_sensor[MAX_SENSORS];

/**
 * ACTUATOR OUTPUTS
 * The actuator in [MAX_ACTUATORS-1] is the pump actuator.
 * The actuators before [MAX_ACTUATORS-1] are the electro-valves
 * */
extern io_t io_electrovalve[MAX_ACTUATORS];

#endif //IO_MAP
