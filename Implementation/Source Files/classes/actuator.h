/**
 * Actuator.h
 * Actuator is an interface to control any type of actuator.
 * Each actuator has its own class.
*/
#ifndef ACTUATOR_H
#define ACTUATOR_H

#include "sensor.h"
#include <type_traits>
#include <iostream>

extern "C"{
	#include "stm32f4xx_hal.h"
}


class actuator
{
public:
	virtual void set( bool set_ON ) = 0;
	virtual bool is_ON() = 0;
	virtual bool has_sensor() = 0;
	virtual uint32_t read_sensor() = 0;
	virtual ~actuator() = default;
};


class actuator_ON_OFF : actuator
{
public:
	/*Constructors*/
	explicit actuator_ON_OFF(GPIO_TypeDef *gpio, uint16_t pin ) :
			_gpio(gpio), _pin(pin), _has_sensor(false), _sensor_ptr(nullptr) { set(false); }
	explicit actuator_ON_OFF(GPIO_TypeDef *gpio, uint16_t pin, sensor *sensor_ptr) :
			_gpio(gpio), _pin(pin), _has_sensor(true), _sensor_ptr(sensor_ptr) { set(false); }
	actuator_ON_OFF() = delete;

	/*Main functions*/
	void set( bool set_ON );
	bool is_ON(){ return _is_ON; }
	bool has_sensor(){ return _has_sensor; }
	uint32_t read_sensor(){ return _has_sensor ? _sensor_ptr->read() : 0; }

	/*Secondary functions*/
	bool set_sensor(sensor *sensor_ptr){ _sensor_ptr = sensor_ptr; return _has_sensor = true; }
	bool remove_sensor(){ if(_has_sensor){ _sensor_ptr = nullptr; return !(_has_sensor = false); } };

private:
  	GPIO_TypeDef* _gpio = nullptr;
    uint16_t _pin;
    bool _has_sensor;
    sensor *_sensor_ptr = nullptr;
    bool _is_ON = false;
};

#endif //ACTUATOR_H
