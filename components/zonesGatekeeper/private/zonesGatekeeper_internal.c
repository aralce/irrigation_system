/*
 * zonesGatekeeper_internal.c
 *
 *  Created on: 3 mar. 2021
 *      Author: Ariel
 */

#include "zonesGatekeeper_internal.h"
#include "cmsis_os.h"
#include "../../config/configuration.h"


/*Public variables --------------------------------------------------------------------------------------------*/

/*tskSensorsGatekeeper */
osThreadId_t tskZonesGatekeeperHandle;
const osThreadAttr_t tskZonesGatekeeper_attributes = {
  .name = "tskZonesGatekeeper",
  .priority = (osPriority_t) osPriorityAboveNormal,
  .stack_size = 1024 * 4
};

/*Queues for INPUT and OUTPUT data*/
/*qSensorsGatekeeperIN */
osMessageQueueId_t qZonesGatekeeperINHandle;
const osMessageQueueAttr_t qZonesGatekeeperIN_attributes = {
  .name = "qZonesGatekeeperIN"
};
/*qSensorsGetekeeperOUT */
osMessageQueueId_t qZonesGatekeeperOUTHandle;
const osMessageQueueAttr_t qZonesGatekeeperOUT_attributes = {
  .name = "qSensorsGatekeeperOUT"
};



/*Private typedefs ----------------------------------------------------------------------------------------------*/

typedef struct {
  uint32_t ID; /*zone identifier*/
  char name[MAX_NAME_SIZE + 1];
  uint32_t *activationMap;
  uint32_t selenoidValveID;
}struct_zone;

static uint32_t activationMap; /* |bit 31 to 7 reserved | bit 6: zone 6 | bit 5: zone 5 | bit 4: zone 4 | bit 3: zone 3 | bit 2: zone 2 |
   	   	   	   	   	   	   	      | bit 1: zone 1 | bit 0: zone 0 |*/



/*Private variables ---------------------------------------------------------------------------------------------*/

/*List of zones */
static struct_zone zones[MAX_ZONES_ID + 1];

/*Receiving orders structure*/
static struct{
  enum_zone_operationType operationType;
  uint32_t ID;
  void *data;
} IN_struct;

static struct{
  enum_zone_operationType operationType;
  uint32_t ID;
  void *data;
  enum_zonesOrderError errorType;
} OUT_struct;


/*Private function declaration ----------------------------------------------------------------------------------*/
static enum_zoneState setZoneState( uint32_t ID, enum_zone_state state );



/*Public function definition -----------------------------------------------------------------------------------*/

void zonesGatekeeper_init()
{
  uint32_t zoneNumber = 0;
  char tmp[17] = ""; /*helps to load the sensor number on the sensor name.*/

  /* creation of tskZonesGatekeeper */
    tskZonesGatekeeperHandle = osThreadNew(zonesGatekeeper_task, NULL, &tskZonesGatekeeper_attributes);

  /* Create the queue(s) */
    /* creation of qSensorsGatekeeperIN */
    qZonesGatekeeperINHandle = osMessageQueueNew (32, sizeof(void *), &qZonesGatekeeperIN_attributes);
    /* creation of qSensorsGetekeeperOUT */
    qZonesGatekeeperOUTHandle = osMessageQueueNew (16, sizeof(void *), &qZonesGetekeeperOUT_attributes);


  /*Initializes the sensor structures*/
    for(zoneNumber = 0; zoneNumber <=MAX_ZONE_ID; zoneNumber++)
    {
      zone[zoneNumber].ID = zoneNumber;

      /*loads the sensor name*/
      strcpy( (char* volatile) zone[zoneNumber].name, "Zone ");
      itoa( zoneNumber, tmp, 10 );
      strcat( (char* volatile) zone[zoneNumber].name, (const char* volatile) tmp );



      /*sets the driver function for the sensor*/
      setZoneState(zoneNumber, OFF );

    }
}

/*----------------------------------------------------------------------------------------------------------------*/
/*--------------------------------THIS CODE HAS THE ZONESGATEKEEPER CORE TASK-----------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/

void zonesGatekeeper_task(void* parameters)
{
  static const char *zoneName = NULL; /*to return the sensor name of the sensor used*/


  /*task infinite loop*/
  for(;;)
  {
    osMessageQueueGet(qZonesGatekeeperINHandle, &IN_struct, 0, osWaitForever ); /*waits until an order arrives*/

    /*processes the command*/
    switch(IN_struct.operationType)
    {
      case getName:
        /*executes the order*/
        if( IN_struct.ID <= MAX_ZONE_ID ) /*checks the ID*/
        {
          zoneName = (const char *) zone[IN_struct.ID].name;
          OUT_struct.errorType = OK;
        }
        else
        {
          zoneName = NULL; /*if there is an error*/
          OUT_struct.errorType = INVALID_ID;
        }

        /*loads the data onto the output structures*/
        OUT_struct.operationType = IN_struct.operationType;
        OUT_struct.ID = IN_struct.ID;
        OUT_struct.data = (void *) zoneName;
        break;


      case setName:
        zoneName = (const char *)IN_struct.data;

        /*executes the order*/
        if( IN_struct.ID <= MAX_SENSOR_ID ) /*the nameSize was checked on sensorsGatekeeper_public*/
        {
          if( strcmp(zoneName, "") != 0 ) /*sensorsGatekeeper_public sends an empty string when the name is not valid*/
          {
            strcpy( sensor[IN_struct.ID].name, sensorName);
            OUT_struct.errorType = OK;
          }
          else
          {
            sensorName = NULL; /*if there is an error*/
            OUT_struct.errorType = INVALID_NAME;
          }
        }
        else
        {
          sensorName = NULL; /*if there is an error*/
          OUT_struct.errorType = INVALID_ID;
        }

        /*loads the data onto the output structures*/
        OUT_struct.operationType = IN_struct.operationType;
        OUT_struct.ID = IN_struct.ID;
        OUT_struct.data = (void *) sensorName;
        break;

      case getState:
        if( IN_struct.ID <= MAX_SENSOR_ID ) /*checks the ID*/
        {
          sensorType = sensor[IN_struct.ID].type;
          OUT_struct.errorType = OK;
        }
        else
        {
          sensorType = sensorType_ENUM_END; /*if there is an error*/
          OUT_struct.errorType = INVALID_ID;
        }

        /*load the data onto the output structures*/
        OUT_struct.operationType = IN_struct.operationType;
        OUT_struct.ID = IN_struct.ID;
        OUT_struct.data = (void *) sensorType;
        break;


      case setState:
        sensorType = (enum_sensorType) IN_struct.data;

        /*executes the order*/
        if( IN_struct.ID <= MAX_SENSOR_ID ) /*checks the ID*/
        {
          if(sensorType < sensorType_ENUM_END) /*checks if the sensorType is valid*/
          {
            sensor[IN_struct.ID].type = sensorType;
            OUT_struct.errorType = OK;

            /*sets the corresponding measure function*/
            takeMeasureFromSensor[IN_struct.ID] = driverArray[(uint32_t) sensorType];
          }
          else
            OUT_struct.errorType = INVALID_SENSOR_TYPE;
        }
        else
          OUT_struct.errorType = INVALID_ID;

        /*load the data onto the output structures*/
        OUT_struct.operationType = IN_struct.operationType;
        OUT_struct.ID = IN_struct.ID;
        OUT_struct.data = (void *) sensorType;
        break;


      default:
        break;

    }
  }

  /*the code never reaches this point*/

}


/*Used by the timers to take the measure from the sensor*/
void sensorsGatekeeper_takeMeasure( void* sensorID )
{

  /*checks the sensorID*/
  if( (uint32_t)sensorID <= MAX_SENSOR_ID )
  {
    sensor[(uint32_t)sensorID].measure.value = takeMeasureFromSensor[(uint32_t)sensorID]((uint32_t)sensorID);
    /*sensor measure.Timestamp: TBD*/

    /*loads the data onto the output structures*/
    OUT_struct.operationType = getMeasure;
    OUT_struct.ID = IN_struct.ID;
    OUT_struct.data = (void *) sensor[(uint32_t)sensorID].measure.value;

    /*sends the data to the datalogger*/
    if( osMessageQueuePut(qSensorsGatekeeperOUTHandle, &OUT_struct, 0, 500) != osOK)
      /*Error Message*/;

  }
}


/*Private function definitions ---------------------------------------------------------------------------------------*/

static osStatus sensorsGatekeeper_setTimerPeriod( uint32_t sensorID ) /*returns the operationStatus*/
{
  osStatus operationStatus = osOK;

  /*checks the sensorID*/
  if( sensorID <= MAX_SENSOR_ID )
  {
    /*if measure interval is greater than 0, sets the interval, else stops the measures*/
    if( sensor[sensorID].measureInterval > 0 )
    {
    	operationStatus = osTimerStart( timer_sensorHandle[sensorID], HAL_secondsToTicks(sensor[sensorID].measureInterval) );
    }
    else
    {
      if( osTimerIsRunning( timer_sensorHandle[sensorID] ) )
        operationStatus = osTimerStop(timer_sensorHandle[sensorID] );
    }
  }

  return operationStatus;

}

