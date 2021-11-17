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


class Sensor
{
public:
	virtual uint32_t read() = 0;
	virtual ~Sensor() = default;
};


class Sensor_switch : public Sensor
{
public:
  explicit Sensor_switch(GPIO_TypeDef *gpio, uint16_t pin, bool is_normal_open) :
  	  	  _gpio(gpio), _pin(pin), _is_normal_open(is_normal_open) { ++_instances_number; }
  Sensor_switch() = delete;
  ~Sensor_switch(){ --_instances_number; }
  uint32_t read() override;
  static int _instances_number;

private:
  	GPIO_TypeDef* _gpio = nullptr;
    uint16_t _pin = 0;
    bool _is_normal_open;
};


#endif //SENSOR_H
