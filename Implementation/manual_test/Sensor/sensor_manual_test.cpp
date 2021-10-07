#include "Sensor_manual_test.h"

#include "cmsis_os.h"
#include "stm32f4xx_hal.h"
#include "../../classes/Sensor.h"

static Sensor *sensor[2];
extern UART_HandleTypeDef huart1;

void sensor_switch_test_init()
{
	/*GPIO Initialization*/
	GPIO_InitTypeDef GPIO_init_struct = {0};
	GPIO_init_struct.Pin = GPIO_PIN_2;
	GPIO_init_struct.Mode = GPIO_MODE_INPUT;
	GPIO_init_struct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOG, &GPIO_init_struct);
	GPIO_init_struct.Pin = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOG, &GPIO_init_struct);

	/*Initialization of sensor*/
	sensor[0] = new Sensor_switch(GPIOG, GPIO_PIN_2, true);
	sensor[1] = new Sensor_switch(GPIOG, GPIO_PIN_3, false);
}

void sensor_switch_test_run()
{
	/*Read sensors and show if a sensor is active on LED3 and LED4*/
	bool isHigh = sensor[0]->read();
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, isHigh ? GPIO_PIN_SET : GPIO_PIN_RESET ); //LED3-SENSOR0(PG2)
	isHigh = sensor[1]->read();
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, isHigh ? GPIO_PIN_SET : GPIO_PIN_RESET ); //LED4-SENSOR1(PG3)

	{
	uint8_t string[] = "\nThe number of sensors is: ";
	HAL_UART_Transmit(&huart1, string, sizeof(string)/sizeof(string[0]), 500/*ms*/ );
	uint8_t sensor_num[2] = {(uint8_t)Sensor_switch::_instances_number, (uint8_t)'\n'};
	HAL_UART_Transmit(&huart1, sensor_num, sizeof(sensor_num)/sizeof(sensor_num[0]), 500/*ms*/ );
	}

	/*Delete 1 sensor and show the number of sensors on UART*/
	osDelay(500);
	if(Sensor_switch::_instances_number)
		delete sensor[0];
	{
	uint8_t string[] = "\nThe number of sensors is: ";
	HAL_UART_Transmit(&huart1, string, sizeof(string)/sizeof(string[0]), 500/*ms*/ );
	uint8_t sensor_num[2] = {(uint8_t)Sensor_switch::_instances_number , (uint8_t)'\n'};
	HAL_UART_Transmit(&huart1, sensor_num, sizeof(sensor_num)/sizeof(sensor_num[0]), 500/*ms*/ );
	}

	osDelay(500);
	/*Delete 1 sensor and show the number of sensors on UART*/
	if(Sensor_switch::_instances_number)
		delete sensor[1];
	{
	uint8_t string[] = "\nThe number of sensors is: ";
	HAL_UART_Transmit(&huart1, string, sizeof(string)/sizeof(string[0]), 500/*ms*/ );
	uint8_t sensor_num[2] = {(uint8_t)Sensor_switch::_instances_number , (uint8_t)'\n'};
	HAL_UART_Transmit(&huart1, sensor_num, sizeof(sensor_num)/sizeof(sensor_num[0]), 500/*ms*/ );
	}

	/*Initialization of sensor*/
	sensor[0] = new Sensor_switch(GPIOG, GPIO_PIN_2, true);
	sensor[1] = new Sensor_switch(GPIOG, GPIO_PIN_3, false);
	osDelay(500);
}
