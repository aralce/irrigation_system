#include "Control_unit_factory.h"

Irrigation_manager* Control_unit_factory::instantiate_control_unit(){
	if( _control_unit != nullptr)
		return _control_unit;

	init_io();
	std::array<Sensor*, MAX_SENSORS> sensor_array{instantiate_switch_sensors(true)};
	std::array<Actuator*, MAX_ACTUATORS> actuator_array{instantiate_actuators(sensor_array)};
	std::array<Irrigation_zone*, MAX_ZONES> irrigation_array{instantiate_irrigation_zones(actuator_array)};
	_control_unit = instantiate_irrigation_manager(irrigation_array);
	return _control_unit;
}

/* ====[Private Functions]=========================================================== */
void Control_unit_factory::init_io(){
	//Inits sensor inputs:
	  /* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	for(auto i=0; i<MAX_SENSORS; ++i){
		GPIO_InitStruct.Pin = io_sensor[i].pin;
		HAL_GPIO_Init(io_sensor[i].port, &GPIO_InitStruct);
	}

	//Inits actuator outputs:
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	for(auto i=0; i<MAX_ACTUATORS; ++i){
		GPIO_InitStruct.Pin = io_electrovalve[i].pin;
		HAL_GPIO_Init(io_electrovalve[i].port, &GPIO_InitStruct);
	}

}


std::array<Sensor*, MAX_SENSORS> Control_unit_factory::instantiate_switch_sensors(bool are_normal_open){
	std::array<Sensor*, MAX_SENSORS> sensor_array;
	//The last sensor is the sensor pump.
	for(auto i=0; i<MAX_SENSORS; ++i)
		sensor_array[i] = new Sensor_switch(io_sensor[i].port, io_sensor[i].pin, are_normal_open);
	return sensor_array;
}


std::array<Actuator*, MAX_ACTUATORS> Control_unit_factory::instantiate_actuators(std::array<Sensor*, MAX_ACTUATORS> sensor_array){
	std::array<Actuator*, MAX_ZONES+1> actuator_array;
	//The last actuator is the pump. The others are electro-valves ordered by number.
	for(auto i=0; i<MAX_ACTUATORS; ++i)
		actuator_array[i] = new Actuator_ON_OFF(io_electrovalve[i].port, io_electrovalve[i].pin, sensor_array[i]);
	return actuator_array;
}


std::array<Irrigation_zone*, MAX_ZONES> Control_unit_factory::instantiate_irrigation_zones(std::array<Actuator*, MAX_ACTUATORS> actuator_array){ //the last actuator is the pump
	std::array<Irrigation_zone*, MAX_ZONES> irrigation_array;
	for(auto i=0; i<MAX_ZONES; ++i)
		irrigation_array[i] =  new Irrigation_zone(actuator_array[i], actuator_array[MAX_ACTUATORS-1]);
	return irrigation_array;
}


Irrigation_manager* Control_unit_factory::instantiate_irrigation_manager(std::array<Irrigation_zone*, MAX_ZONES> irrigation_array){
	Calendar_routine *calendar_array[MAX_ZONES];
	for(auto i=0; i<MAX_ZONES; ++i)
		calendar_array[i] = new Calendar_routine_annual;
	Irrigation_manager *irr_manager = new Irrigation_manager(irrigation_array.data(), calendar_array);
	return irr_manager;
}

