#include <gtest/gtest.h>
#include "mock-calendar_routine.h"
#include "mock-irrigation_zone.h"
#include "mock-RTC.h"
#include "Irrigation_manager.h"

/*************************
 *  Irrigation Manager Test
 * ***********************
 *  The main requirements are:
 * 
 * 1- Construct a irrigation manager with irrigation zones.
 * 
 * //Manual control of Irrigation Zones:
 * 2.1- Set an irrigation zone to irrigate. Then stop to irrigate the zone.
 * 2.2- Set and stop an invalid irrigation zone has no effect. The system notify the failure.
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

class IrrigationZonesManagerTest: public ::testing::Test{
protected:
   void SetUp() override{
      for(int i=0; i<MAX_ZONES; ++i)
         zones[i] = new MockIrrigation_zone; 
   }
   void TearDown() override{
      for(int i=0; i<MAX_ZONES; ++i)
         delete zones[i];
   }
   Irrigation_zone *zones[MAX_ZONES];
};

/*STARTING TESTS*/
//1- Construct a calendar routine without data.
TEST_F(IrrigationZonesManagerTest, construct_with_irrigation_zones){
   Irrigation_manager irr_manager(zones);
}

///////////////////////////////////
//Manual control of Irrigation Zones:
//2.1- Set an irrigation zone to irrigate. Then stop to irrigate the zone.
TEST_F(IrrigationZonesManagerTest, irrigate_zone){
   Irrigation_manager irr_manager(zones);  
   for(auto i=0; i<MAX_ZONES; ++i){
      EXPECT_CALL(*dynamic_cast<MockIrrigation_zone*>(zones[i]), irrigate(true))
         .Times(1);
      irr_manager.irrigate(i, true);
      EXPECT_CALL(*dynamic_cast<MockIrrigation_zone*>(zones[i]), irrigate(false))
         .Times(1);
      irr_manager.irrigate(i, false);
   }
}

//2.2- Set and stop an invalid irrigation zone has no effect. The system notify the failure.
TEST_F(IrrigationZonesManagerTest, irrigate_invalid_zone){
   Irrigation_manager irr_manager(zones);
   ASSERT_FALSE(irr_manager.irrigate(MAX_ZONES + 1, true));
   ASSERT_FALSE(irr_manager.irrigate(MAX_ZONES + 1, false));
}

////////////////////////////
//Check irrigation zones health:
//3.1- If there is any irrigation zone with malfunctions. The system notify the failure
//and return number of irrigation zone.
TEST_F(IrrigationZonesManagerTest, check_zone_health){
   Irrigation_manager irr_manager(zones);
   constexpr int zone_to_test = 2;
   uint8_t zone_with_error;
   using ::testing::Return;
   for(auto i=0; i<MAX_ZONES; ++i){
      if(i != zone_to_test)
         EXPECT_CALL(*dynamic_cast<MockIrrigation_zone*>(zones[i]), is_healthy())
            .WillOnce(Return(true));
      else{
         EXPECT_CALL(*dynamic_cast<MockIrrigation_zone*>(zones[i]), is_healthy())
            .WillOnce(Return(false));
         break; //when a zone with failure is found, the for needs to break
      }
   }
   ASSERT_FALSE(irr_manager.is_healthy(&zone_with_error));
   ASSERT_EQ(zone_to_test, zone_with_error);
}