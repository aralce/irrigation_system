#include "io_map.h"


/**
 * SENSOR INPUTS
 * the sensor in [MAX_SENSORS-1] is the pump sensor
 * */
io_t io_sensor[MAX_SENSORS] = { {GPIOE, GPIO_PIN_3 },
						        {GPIOE, GPIO_PIN_4 },
						        {GPIOC, GPIO_PIN_13},
						        {GPIOF, GPIO_PIN_6 },
						        {GPIOA, GPIO_PIN_5 },
						        {GPIOG, GPIO_PIN_2 },
						        {GPIOG, GPIO_PIN_3 },
						        {GPIOC, GPIO_PIN_8 },
							    {GPIOB, GPIO_PIN_7 }
				    		  };

/**
 * ACTUATOR OUTPUTS
 * The actuator in [MAX_ACTUATORS-1] is the pump actuator.
 * The actuators before [MAX_ACTUATORS-1] are the electro-valves
 * */
io_t io_electrovalve[MAX_ACTUATORS] = { {GPIOC , GPIO_PIN_11},
		                                {GPIOC , GPIO_PIN_12},
									    {GPIOD , GPIO_PIN_2 },
									    {GPIOD , GPIO_PIN_4 },
									    {GPIOD , GPIO_PIN_5 },
									    {GPIOD , GPIO_PIN_7 },
									    {GPIOG , GPIO_PIN_9 },
									    {GPIOB , GPIO_PIN_3 },
									    {GPIOB , GPIO_PIN_4 }
					      	  	      };
