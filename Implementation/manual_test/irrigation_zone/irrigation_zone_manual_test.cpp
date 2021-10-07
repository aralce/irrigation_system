#include "irrigation_zone_manual_test.h"

#include "cmsis_os.h"
#include "stm32f4xx_hal.h"
#include "../../classes/Sensor.h"
#include "../../classes/Actuator.h"
#include "../../classes/irrigation_zone.h"

extern UART_HandleTypeDef huart1;

/*==== [Constants] ===============================================================*/
//GPIOs
static constexpr uint16_t LD3_PIN = GPIO_PIN_13;
static constexpr uint16_t LD4_PIN = GPIO_PIN_14;
//Delay
static constexpr int TEST_DELAY_MS = 1000;

/*==== [Private variables] =======================================================*/
static Irrigation_zone *irr = nullptr;
static Irrigation_zone *irr2 = nullptr;


/*==== [Function definitions] ====================================================*/
void irrigation_zone_manual_test_init()
{
	Actuator *electrovalve = new Actuator_ON_OFF(GPIOG, LD3_PIN);
	Actuator *pump = new Actuator_ON_OFF(GPIOG, LD4_PIN);
	pump->set(false);
	irr = new Irrigation_zone(electrovalve, pump);

	/*Configure GPIO pin : PG2 */
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin   = GPIO_PIN_2;
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

	Actuator *electrovalve2 = new Actuator_ON_OFF(GPIOG, GPIO_PIN_2);

	irr2 = new Irrigation_zone(electrovalve2, pump);

}

void irrigation_zone_manual_test_run()
{
	osDelay(TEST_DELAY_MS);
	irr2->irrigate(false);
	/*Test 1) To activate a zone when is the unique zone and the pump is off.*/
	irr->irrigate(true);
	{
	uint8_t string[] = "\nThe number of irrigation zones active is: ";
	HAL_UART_Transmit(&huart1, string, sizeof(string)/sizeof(string[0]), 500/*ms*/ );
	uint8_t sensor_num[2] = {(uint8_t)Debug::get_nzones_irrigating(), (uint8_t)'\n'};
	HAL_UART_Transmit(&huart1, sensor_num, sizeof(sensor_num)/sizeof(sensor_num[0]), 500/*ms*/ );
	}


	osDelay(TEST_DELAY_MS);
	/*Test 2) To deactivate a zone when the zone is unique and the pump is on.*/
	irr->irrigate(false);
	{
	uint8_t string[] = "\nThe number of irrigation zones active is: ";
	HAL_UART_Transmit(&huart1, string, sizeof(string)/sizeof(string[0]), 500/*ms*/ );
	uint8_t sensor_num[2] = {(uint8_t)Debug::get_nzones_irrigating(), (uint8_t)'\n'};
	HAL_UART_Transmit(&huart1, sensor_num, sizeof(sensor_num)/sizeof(sensor_num[0]), 500/*ms*/ );
	}

	osDelay(TEST_DELAY_MS);
	irr->irrigate(true);
	irr2->irrigate(true);
	{
	uint8_t string[] = "\nThe number of irrigation zones active is: ";
	HAL_UART_Transmit(&huart1, string, sizeof(string)/sizeof(string[0]), 500/*ms*/ );
	uint8_t sensor_num[2] = {(uint8_t)Debug::get_nzones_irrigating(), (uint8_t)'\n'};
	HAL_UART_Transmit(&huart1, sensor_num, sizeof(sensor_num)/sizeof(sensor_num[0]), 500/*ms*/ );
	}

	/*Test 3) To deactivate a zone when there are two zones activated.*/
	irr->irrigate(false);
}


