/**
 * Actuator.h
 * Actuator is an interface to control any type of actuator.
 * Each actuator has its own class.
*/
#ifndef ACTUATOR_H
#define ACTUATOR_H

#include <type_traits>
#include <iostream>
#include "Sensor.h"

extern "C"{
	#include "stm32f4xx_hal.h"
}


class Actuator
{
public:
	virtual void set( bool set_ON ) = 0;
	virtual bool is_ON() = 0;
	virtual bool has_sensor() = 0;
	virtual uint32_t read_sensor() = 0;
	virtual ~Actuator() = default;
};


class Actuator_ON_OFF : public Actuator
{
public:
	/*Constructors*/
	explicit Actuator_ON_OFF(GPIO_TypeDef *gpio, uint16_t pin ) :
			_gpio(gpio), _pin(pin), _has_sensor(false), _sensor_ptr(nullptr) { set(false); }
	explicit Actuator_ON_OFF(GPIO_TypeDef *gpio, uint16_t pin, Sensor *sensor_ptr) :
			_gpio(gpio), _pin(pin), _has_sensor(true), _sensor_ptr(sensor_ptr) { set(false); }
	Actuator_ON_OFF() = delete;

	/*Main functions*/
	void set( bool set_ON ) override;
	bool is_ON() override{ return _is_ON; }
	bool has_sensor() override{ return _has_sensor; }
	uint32_t read_sensor() override{ return _has_sensor ? _sensor_ptr->read() : 0; }

	/*Secondary functions*/
	bool set_sensor(Sensor *sensor_ptr){ _sensor_ptr = sensor_ptr; return _has_sensor = true; }
	bool remove_sensor();

private:
  	GPIO_TypeDef* _gpio = nullptr;
    uint16_t _pin;
    bool _has_sensor;
    Sensor *_sensor_ptr = nullptr;
    bool _is_ON = false;
};

#endif //ACTUATOR_H
