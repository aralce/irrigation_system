/*
 * zonesGatekeeper.h
 *
 *  Created on: 9 ene. 2021
 *      Author: Ariel
 */

#ifndef ZONESGATEKEEPER_ZONESGATEKEEPER_PUBLIC_H_
#define ZONESGATEKEEPER_ZONESGATEKEEPER_PUBLIC_H_

/* Includes ------------------------------------------------------------------*/
#include "private/zonesGatekeeper_internal.h"



/*Public functions -------------------------------------------------------------*/

bool zonesGatekeeper_getName(uint32_t ID); /*returns True if OK, else False*/
bool zonesGatekeeper_setName(uint32_t ID, char *sensorName); /*returns True if OK, else False. Máx characters 16*/
bool zonesGatekeeper_getState(uint32_t ID); /*returns True if OK, else False*/
bool zonesGatekeeper_setState(uint32_t ID, enum_zone_state state ); /*returns True if OK, else False*/

#endif /* ZONESGATEKEEPER_ZONESGATEKEEPER_PUBLIC_H_ */
