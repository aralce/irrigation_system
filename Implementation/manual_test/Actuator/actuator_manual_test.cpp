#include "Actuator_manual_test.h"
#include "cmsis_os.h"
#include "cmsis_os.h"
#include "stm32f4xx_hal.h"
#include "Actuator.h"

#define COUNT_OF(x) sizeof(x)/sizeof(x[0])

static Sensor *sensor;
static Actuator *actuator;
extern UART_HandleTypeDef huart1;


void actuator_manual_test_init_with_sensor()
{
	/*GPIO Initialization*/
	GPIO_InitTypeDef GPIO_init_struct = {0};
	GPIO_init_struct.Pin = GPIO_PIN_2;
	GPIO_init_struct.Mode = GPIO_MODE_INPUT;
	GPIO_init_struct.Pull = GPIO_PULLUP;
	//Initializes GPIO for sensor in GP2 as input with pull-up
	HAL_GPIO_Init(GPIOG, &GPIO_init_struct);

	//Initializes GPIO for actuator in LED4(PG14) has been initialized by MX default configuration.

	/*Initialization of sensor and actuator*/
	sensor = new Sensor_switch(GPIOG, GPIO_PIN_2, true);
	actuator = new Actuator_ON_OFF(GPIOG, GPIO_PIN_14, sensor);
}

void actuator_manual_test_init_without_sensor()
{
	/*GPIO Initialization*/
	GPIO_InitTypeDef GPIO_init_struct = {0};
	GPIO_init_struct.Pin = GPIO_PIN_2;
	GPIO_init_struct.Mode = GPIO_MODE_INPUT;
	GPIO_init_struct.Pull = GPIO_PULLUP;
	//Initializes GPIO for sensor in GP2 as input with pull-up
	HAL_GPIO_Init(GPIOG, &GPIO_init_struct);

	//Initializes GPIO for actuator in LED4(PG14) has been initialized by MX default configuration.

	/*Initialization of actuator*/
	actuator = new Actuator_ON_OFF(GPIOG, GPIO_PIN_14);
}

void actuator_manual_test_run()
{
	uint8_t yes[7] = "Yes \n";
	uint8_t  no[7] = "No  \n";
	uint8_t  on[7] = "ON  \n";
	uint8_t off[7] = "OFF \n";

	/*3-Shows the actuator state on UART(PA9)*/
	{
	uint8_t string[] = "\nThe actuator has sensor: ";
	HAL_UART_Transmit(&huart1, string, COUNT_OF(string) - 1, 500/*ms*/ );
	uint8_t *response = actuator->has_sensor() ? yes : no ;
	HAL_UART_Transmit(&huart1, response, 6, 500/*ms*/ );
	uint8_t string2[] = "The actuator state is: ";
	HAL_UART_Transmit(&huart1, string2, COUNT_OF(string2) - 1, 500/*ms*/ );
	response = actuator->is_ON() ? on : off;
	HAL_UART_Transmit(&huart1, response, 6, 500/*ms*/ );
	}

	/*4-Read the sensor state on LED3*/
	if (actuator->has_sensor())
		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, actuator->read_sensor() ? GPIO_PIN_SET : GPIO_PIN_RESET);

	/*6- Wait one second*/
	osDelay(1000);

	/*5-Activate actuator*/
	actuator->set(true);

	/*6-Shows the actuator state on UART(PA9)*/
	{
	uint8_t string[] = "\nThe actuator has sensor: ";
	HAL_UART_Transmit(&huart1, string, COUNT_OF(string) - 1, 500/*ms*/ );
	uint8_t *response = actuator->has_sensor() ? yes : no ;
	HAL_UART_Transmit(&huart1, response, 6, 500/*ms*/ );
	uint8_t string2[] = "The actuator state is: ";
	HAL_UART_Transmit(&huart1, string2, COUNT_OF(string2) - 1, 500/*ms*/ );
	response = actuator->is_ON() ? on : off;
	HAL_UART_Transmit(&huart1, response, 6, 500/*ms*/ );
	}

	/*7-Wait one second*/
	osDelay(1000);

	/*8-Deactivate actuator*/
	actuator->set(false);

}
