/**
 * Sensor.h
 * Sensor is an interface to control any type of sensor.
 * Each sensor has its own class.
*/

#ifndef SENSOR_H
#define SENSOR_H

extern "C"{
	#include "stm32f4xx_hal.h"
}


class sensor
{
public:
	virtual uint32_t read() = 0;
	virtual int get_instances_num() = 0;
	virtual ~sensor() = default;
};


class sensor_switch : public sensor
{
public:
  explicit sensor_switch(GPIO_TypeDef *gpio, uint16_t pin) : _gpio(gpio), _pin(pin) { ++_instances_number; }
  sensor_switch() = delete;
  int get_instances_num(){ return _instances_number; }
  uint32_t read();

private:
  	GPIO_TypeDef* _gpio = nullptr;
    uint16_t _pin = 0;
    static  int _instances_number = 0;
};


#endif //SENSOR_H
