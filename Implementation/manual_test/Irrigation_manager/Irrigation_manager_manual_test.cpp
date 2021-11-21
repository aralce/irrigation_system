#include "Control_unit_factory.h"
#include <cstring>
#include "cmsis_os.h"

/* ==== [CONSTANTS] ========================================================= */
constexpr int ZONE_TO_TEST = 3;


/* ==== [Private variables] ================================================= */
static Irrigation_manager *irr_manager;
static Control_unit_factory factory;
extern UART_HandleTypeDef huart1;
extern RTC_HandleTypeDef hrtc;
RTC_class *_rtc;

/* ==== [Private Functions] ================================================= */
static void MX_RTC_Init(void);

/* ==== [Public Functions Definition] ======================================= */
void irrigation_manager_manual_test_init(){
	irr_manager = factory.instantiate_control_unit();

	_rtc = new RTC_class(hrtc);
	//Initializes run2 test
	MX_RTC_Init();
	tm start_time;
	start_time.tm_sec = 10;
	start_time.tm_min = 20;
	start_time.tm_hour = 10;
	start_time.tm_mday = 1;
	start_time.tm_mon = 7;
	start_time.tm_year = 121;
	//Sets the event 1 minutes into future
	start_time.tm_min += 1;
	char message[] = "Event initialized OK\r\n";
	if(irr_manager->add_event(ZONE_TO_TEST, start_time, 1) == true)
		HAL_UART_Transmit(&huart1, reinterpret_cast<uint8_t*>(message), strlen(message), 500);

}

void irrigation_manager_manual_test_run_1(){
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

void irrigation_manager_manual_test_run_2(){
	irr_manager->process_events(*_rtc);
	tm actual_time;
	_rtc->get_time(actual_time);
	char message[400];
	sprintf(message, "The time is: %d:%d:%d\r\nThe date is: %d/%d/%d\r\n\r\n", actual_time.tm_hour, actual_time.tm_min, actual_time.tm_sec, actual_time.tm_mday + 1, actual_time.tm_mon + 1, actual_time.tm_year + 1900);
	HAL_UART_Transmit(&huart1, reinterpret_cast<uint8_t*>(message), strlen(message), 500);
	osDelay(2000);
}


/* ====[Private function definition] ============================================ */
void Error_Handler(){}

static void MX_RTC_Init(void){
  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};
  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 124;
  hrtc.Init.SynchPrediv = 7999;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 10;
  sTime.Minutes = 20;
  sTime.Seconds = 10;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_AUGUST;
  sDate.Date = 2;
  sDate.Year = 21;
  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
}
