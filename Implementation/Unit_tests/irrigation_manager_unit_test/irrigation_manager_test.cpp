#include <gtest/gtest.h>
#include "mock-calendar_routine.h"
#include "mock-irrigation_zone.h"
#include "mock-RTC.h"

/*************************
 *  Irrigation Manager Test
 * ***********************
 *  The main requirements are:
 * 
 * 1- Construct a irrigation manager with irrigation zones.
 * 
 * //Manual control of Irrigation Zones:
 * 2.1- Set an irrigation zone to irrigate.
 * 2.2- Stop to irrigate an irrigation zone that is irrigating.
 * 2.3- Set and stop an invalid irrigation zone has no effect. The system notify the failure.
 * 
 * //Check irrigation zones health:
 * 3.1- If there is any irrigation zone with malfunctions. The system notify the failure
 * and return number of irrigation zone.
 *  
 * //Add and remove of Events:
 * 4.1- Set an event to an irrigation zone.
 * 4.2- Set an event to an invalid irrigation zone. Returns failure.
 * 4.3- Remove an event from an irrigation zone.
 * 4.4- Remove an event from an invalid irrigation zone. Returns failure.
 * 
 * //Process events:
 * 5.1- If there is no event active, all irrigation zones must be inactive.
 * 5.2- If there is an event active in one irrigation zone, that irrigation zone turns on.
 * 5.3- If there is an event in more than one irrigation zone, those irrigation zones are active.
 *
 *  
 * ****************************************************************************
 * Author: Ariel Cerfoglia
 * Email: ariel.cerfoglia@gmail.com
 * ****************************************************************************
*/ 
constexpr int ZONES_NUM = 8;

class IrrigationZonesManagerTest: public ::testing::Test{
protected:
   void SetUp() override{
      for(int i=0; i<ZONES_NUM; ++i)
         zones[i] = new MockIrrigation_zone; 
   }
   void TearDown() override{
      for(int i=0; i<ZONES_NUM; ++i)
         delete zones[i];
   }
   Irrigation_zone *zones[ZONES_NUM];
};

/*STARTING TESTS*/
//1- Construct a calendar routine without data.
TEST_F(IrrigationZonesManagerTest, construct_with_irrigation_zones){
   //Irrigation_Manager irr_manager(Irrigation_zone* zones[]);
}
