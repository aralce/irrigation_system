/*
 * zonesGatekeeper_internal.h
 *
 *  Created on: 3 mar. 2021
 *      Author: Ariel
 */

#ifndef ZONESGATEKEEPER_PRIVATE_ZONESGATEKEEPER_INTERNAL_H_
#define ZONESGATEKEEPER_PRIVATE_ZONESGATEKEEPER_INTERNAL_H_

/* Includes -------------------------------------------------------------------------------------------------*/
#include "../../config/configuration.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <cmsis_os.h>
#include <string.h>

/* Typedefs -------------------------------------------------------------------------------------------------*/

typedef enum
{
  withoutOperation,
  getName,
  setName,
  getState,
  setState
}enum_zone_operationType;

typedef enum
{
  ON,
  OFF
}enum_zone_state;

typedef enum
{
  OK,
  INVALID_ID,
  INVALID_NAME,
}enum_zonesOrderError;



/*Public variables --------------------------------------------------------------------------------------------*/


/*tskZonesGatekeeper*/
extern osThreadId_t tskZonesGatekeeperHandle;
extern const osThreadAttr_t tskZonesGatekeeper_attributes;

/*Queues for INPUT and OUTPUT data*/
/*qSensorsGatekeeperIN */
extern osMessageQueueId_t qZonesGatekeeperINHandle;
extern const osMessageQueueAttr_t qZonesGatekeeperIN_attributes;
/*qSensorsGetekeeperOUT */
extern osMessageQueueId_t qZonesGatekeeperOUTHandle;
extern const osMessageQueueAttr_t qZonesGatekeeperOUT_attributes;




/*Public functions --------------------------------------------------------------------------------------------*/

void zonesGatekeeper_init( );

void zonesGatekeeper_task( void* parameters);


#endif /* ZONESGATEKEEPER_PRIVATE_ZONESGATEKEEPER_INTERNAL_H_ */
