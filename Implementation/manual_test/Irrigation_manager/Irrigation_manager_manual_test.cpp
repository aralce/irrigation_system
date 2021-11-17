#include "Control_unit_factory.h"
#include <cstring>
#include "cmsis_os.h"

static Irrigation_manager *irr_manager;
static Control_unit_factory factory;

void irrigation_manager_manual_test_init(){
	irr_manager = factory.instantiate_control_unit();
}

void irrigation_manager_manual_test_run_1(){
	extern UART_HandleTypeDef huart1;
	for(auto i=0; i< MAX_ZONES; ++i){
		irr_manager->irrigate(i, true);
		char message[200];
		sprintf(message, "The zone %d is IRRIGATING\r\nNumber of irrigating zones: %d\r\n\r\n", i, Debug::get_nzones_irrigating());
		HAL_UART_Transmit(&huart1, reinterpret_cast<uint8_t*>(message), strlen(message), 500);
		osDelay(2000);
		irr_manager->irrigate(i, false);
		sprintf(message, "The zone %d is NOT IRRIGATING\r\nNumber of irrigating zones: %d\r\n\r\n", i, Debug::get_nzones_irrigating());
		HAL_UART_Transmit(&huart1, reinterpret_cast<uint8_t*>(message), strlen(message), 500);
		osDelay(2000);
	}
}
