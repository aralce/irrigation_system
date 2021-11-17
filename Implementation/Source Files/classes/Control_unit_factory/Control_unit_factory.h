/**
 * Control_unit_factory.h
 * This file is in charge of instantiate each part of the Control unit.
 * Also, It has the function to change sensor type in runtime.
 * */
#ifndef CONTROL_FACTORY_UNIT_H
#define CONTROL_FACTORY_UNIT_H

#include "stm32f429xx.h"
#include "irrigation_manager.h"
#include "io_map.h"
#include <array>

//By the moment the control_unit_factory return only a reference to an Irrigation_manager... later it will return a reference to control unit class.
class Control_unit_factory{
public:
	Irrigation_manager* instantiate_control_unit();
private:
	void init_io();
	std::array<Sensor*, MAX_SENSORS> instantiate_switch_sensors(bool are_normal_open);
	std::array<Actuator*, MAX_ACTUATORS> instantiate_actuators(std::array<Sensor*, MAX_SENSORS> sensor_array);
	std::array<Irrigation_zone*, MAX_ZONES> instantiate_irrigation_zones(std::array<Actuator*, MAX_ACTUATORS> actuator_array); //the last actuator is the pump
	Irrigation_manager* instantiate_irrigation_manager(std::array<Irrigation_zone*, MAX_ZONES> irrigation_array);
	Irrigation_manager* _control_unit = nullptr;
};

#endif //CONTROL_FACTORY_UNIT_H
