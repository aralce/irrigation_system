/*
 * configuration.h
 *
 *  Created on: 23 ene. 2021
 *      Author: ariel
 */

#ifndef CONFIG_CONFIGURATION_H_
#define CONFIG_CONFIGURATION_H_

#define MAX_NAME_SIZE 16 /*maximum number of characters in sensor name and zone name*/

/*SensorGatekeeper Defines ---------------------------------------------------------------------------------------------*/
#define MAX_SENSOR_ID 6  /*maximum valid sensors*/
#define MAX_MEASURE_INTERVAL 60 /*maximum valid measure interval in seconds*/

#define INITIAL_SENSOR_DRIVER noSensor /*type of sensor driver used at the beginning of the system*/


/*ZonesGatekeeper Defines ----------------------------------------------------------------------------------------------*/
#define MAX_ZONE_ID 6 /*maximum valid zones*/


#endif /* CONFIG_CONFIGURATION_H_ */
