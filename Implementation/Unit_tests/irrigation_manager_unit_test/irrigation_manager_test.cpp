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
 * 1- Construct a irrigation manager with irrigation zones and calendar.
 * 
 * //Manual control of Irrigation Zones:
 * 2.1- Set an irrigation zone to irrigate. Then stop to irrigate the zone.
 * 2.2- Set and stop an invalid irrigation zone has no effect. The system notify the failure.
 * 2.3- Set and stop an not initiated zone has no effect. The system notify the failure.
 * 
 * //Check irrigation zones health:
 * 3.1- If there is any irrigation zone with malfunctions. The system notify the failure
 * and return number of irrigation zone.
 * 3.2- When is asked for health on irrigation zones, if a zone is not initiated on constructor, the 
 * that zone is not accounted for the system health.
 *  
 * //Add and remove of Events:
 * 4.1- Add an event to an irrigation zone. Returns success
 * 4.2- Add an event to an invalid irrigation zone. Returns failure.
 * 4.3- Add an event from a not initiated calendar. Has no effect and returns failure.
 * 4.4- Remove an event from an irrigation zone. Returns success
 * 4.5- Remove an event from an invalid irrigation zone. Returns failure.
 * 4.6- Remove an event from a not initiated calendar. Has no effect and returns failure.
 *  
 * //Process events:
 * 5.1- If there is no event active, all irrigation zones must be inactive.
 * 5.2- If there is an event active in one irrigation zone, that irrigation zone turns on.
 * 5.3- If there is an event in more than one irrigation zone, those irrigation zones are active.
 * 5.4- If there is no event active and irrigation zones are irrigating, then irrigating zones are inactive.
 * 5.5- If there is a not initialized zone. The calendar for that zone is omitted.
 * 5.6- If there is a not initialized calendar. The actions are omitted for that calendar.
 *  
 * //Return events
 * 6.1 Returns the next event active from the desired zone.
 * 6.2 Return  the next event active from an invalid zone, returns failure and has no effect.
 * 6.3 Return the next event from a not initialized calendar, returns failure and has no effect.
 * 6.4 In a zone sets the next event to be return from a desired start_time.
 * 6.5 Try to set the next event on an invalid zone, returns failure and has no effect.
 * 6.6 Set the next event on a not initialized calendar, returns failure and has no effect.
 * 6.7 In a zone restarts the index to get the next event from the first event.
 * 6.8 Restart the index to get the next event from an invalid zone, returns failure and has no effect.
 * 6.9 Restart the index to het the next event in a not initiated calendar. Returns failure and has no effect.
 * 
 * ****************************************************************************
 * Author: Ariel Cerfoglia
 * Email: ariel.cerfoglia@gmail.com
 * ****************************************************************************
*/ 

class IrrigationZonesManagerTest: public ::testing::Test{
protected:
   void SetUp() override{
      for(int i=0; i<MAX_ZONES; ++i){
         zones[i] = new MockIrrigation_zone;
         calendar[i] = new MockCalendar_routine;
      }
   }
   void TearDown() override{
      for(int i=0; i<MAX_ZONES; ++i){
         delete zones[i];
         delete calendar[i];
      }
   }
   Irrigation_zone *zones[MAX_ZONES];
   Calendar_routine *calendar[MAX_ZONES];
};

/*STARTING TESTS*/
//1- Construct a calendar routine without data.
TEST_F(IrrigationZonesManagerTest, construct_with_irrigation_zones){
   Irrigation_manager irr_manager(zones, calendar);
}

///////////////////////////////////
//Manual control of Irrigation Zones:
//2.1- Set an irrigation zone to irrigate. Then stop to irrigate the zone.
TEST_F(IrrigationZonesManagerTest, irrigate_zone){
   Irrigation_manager irr_manager(zones, calendar);  
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
   Irrigation_manager irr_manager(zones, calendar);
   ASSERT_FALSE(irr_manager.irrigate(MAX_ZONES + 1, true));
   ASSERT_FALSE(irr_manager.irrigate(MAX_ZONES + 1, false));
}

//2.3- Set and stop an not initiated zone has no effect. The system notify the failure.
TEST_F(IrrigationZonesManagerTest, irrigate_not_initiated_zone){
   constexpr int ZONE_TO_TEST = 1;
   zones[ZONE_TO_TEST] = nullptr;
   Irrigation_manager irr_manager(zones, calendar);
   ASSERT_FALSE(irr_manager.irrigate(ZONE_TO_TEST, true));
   ASSERT_FALSE(irr_manager.irrigate(ZONE_TO_TEST, false));
}

////////////////////////////
//Check irrigation zones health:
//3.1- If there is any irrigation zone with malfunctions. The system notify the failure
//and return number of irrigation zone.
TEST_F(IrrigationZonesManagerTest, check_zone_health){
   Irrigation_manager irr_manager(zones, calendar);
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

 //3.2- When is asked for health on irrigation zones, if a zone is not initiated on constructor, the 
 //that zone is not accounted for the system health.
TEST_F(IrrigationZonesManagerTest, check_zone_health_with_not_initiated_zone){
   zones[1] = nullptr;
   Irrigation_manager irr_manager(zones, calendar);
   constexpr int zone_to_test = 2;
   uint8_t zone_with_error;
   using ::testing::Return;
   for(auto i=0; i<MAX_ZONES; ++i){
      if(i != zone_to_test && zones[i] != nullptr)
         EXPECT_CALL(*dynamic_cast<MockIrrigation_zone*>(zones[i]), is_healthy())
            .WillOnce(Return(true));
      else if(zones[i] != nullptr){
         EXPECT_CALL(*dynamic_cast<MockIrrigation_zone*>(zones[i]), is_healthy())
            .WillOnce(Return(false));
         break; //when a zone with failure is found, the for needs to break
      }
   }
   ASSERT_FALSE(irr_manager.is_healthy(&zone_with_error));
   ASSERT_EQ(zone_to_test, zone_with_error);
}

///////////////////////
//Add and remove of Events:
//4.1- Add an event to an irrigation zone. Returns success
TEST_F(IrrigationZonesManagerTest, add_event){
   Irrigation_manager irr_manager(zones, calendar);
   tm event_time;
   event_time.tm_year = 2021-1900;
   event_time.tm_mon = 7;
   event_time.tm_hour = 2;
   uint32_t duration_in_minutes = 60;
   using ::testing::Return;
   for(auto i=0; i<MAX_ZONES; ++i){
      EXPECT_CALL(*dynamic_cast<MockCalendar_routine*>(calendar[i]), add_event)
         .WillOnce(Return(true));
      ASSERT_TRUE(irr_manager.add_event(i ,event_time, duration_in_minutes));
      event_time.tm_hour +=2; //change the time between events.
   }
}

//4.2- Add an event to an invalid irrigation zone. Returns failure.
TEST_F(IrrigationZonesManagerTest, add_event_in_invalid_zone){
   Irrigation_manager irr_manager(zones, calendar);
   tm event_time;
   event_time.tm_year = 2021-1900;
   event_time.tm_mon = 7;
   event_time.tm_hour = 2;
   uint32_t duration_in_minutes = 60;
   ASSERT_FALSE(irr_manager.add_event(MAX_ZONES + 2, event_time, duration_in_minutes));
}

//4.3- Add an event from a not initiated calendar. Has no effect and returns failure.
TEST_F(IrrigationZonesManagerTest, add_event_in_not_initiated_calendar){
   constexpr int INDEX_TO_TEST = 1;
   calendar[INDEX_TO_TEST] = nullptr;
   Irrigation_manager irr_manager(zones, calendar);
   tm event_time;
   uint32_t duration_in_minutes = 60;
   ASSERT_FALSE(irr_manager.add_event(INDEX_TO_TEST, event_time, duration_in_minutes));
}

//4.4- Remove an event from an irrigation zone. Returns success
TEST_F(IrrigationZonesManagerTest, remove_event){
   Irrigation_manager irr_manager(zones, calendar);
   tm event_time;
   event_time.tm_year = 2021-1900;
   event_time.tm_mon = 7;
   event_time.tm_hour = 2;
   using ::testing::Return;
   for(auto i=0; i<MAX_ZONES; ++i){
      EXPECT_CALL(*dynamic_cast<MockCalendar_routine*>(calendar[i]), remove_event)
         .WillOnce(Return(true));
      ASSERT_TRUE(irr_manager.remove_event(i, event_time));
   }
}

//4.5- Remove an event from an invalid irrigation zone. Returns failure.
TEST_F(IrrigationZonesManagerTest, remove_event_in_invalid_zone){
   Irrigation_manager irr_manager(zones, calendar);
   tm event_time;
   event_time.tm_year = 2021-1900;
   event_time.tm_mon = 7;
   event_time.tm_hour = 2;
   ASSERT_FALSE(irr_manager.remove_event(MAX_ZONES + 4, event_time));
}

//4.6- Remove an event from a not initiated calendar. Has no effect and returns failure.
TEST_F(IrrigationZonesManagerTest, remove_event_in_not_initiated_calendar){
   constexpr int INDEX_TO_TEST = 1;
   calendar[INDEX_TO_TEST] = nullptr;
   Irrigation_manager irr_manager(zones, calendar);
   tm event_time;
   ASSERT_FALSE(irr_manager.remove_event(INDEX_TO_TEST, event_time));
}

///////////
//Process events:
//5.1- If there is no event active, all irrigation zones must be inactive.
TEST_F(IrrigationZonesManagerTest, process_no_event_active){
   Irrigation_manager irr_manager(zones, calendar);
   MockRTC rtc;
   using ::testing::Return;
   EXPECT_CALL(rtc, get_time)
      .Times(1);
   for(auto i=0; i<MAX_ZONES; ++i){
      using ::testing::Return;
      EXPECT_CALL(*dynamic_cast<MockCalendar_routine*>(calendar[i]), is_event_active)
         .WillOnce(Return(false));
      EXPECT_CALL(*dynamic_cast<MockIrrigation_zone*>(zones[i]), is_irrigating)
         .WillOnce(Return(false));
   }
   irr_manager.process_events(rtc);
}
//5.2- If there is an event active in one irrigation zone, that irrigation zone turns on.
TEST_F(IrrigationZonesManagerTest, event_active_in_one_irrigation_zone){
   Irrigation_manager irr_manager(zones, calendar);
   MockRTC rtc;
   constexpr uint8_t ZONE_ACTIVE = 3;
   using ::testing::Return;
   EXPECT_CALL(rtc, get_time)
      .Times(1);
   for(auto i=0; i<MAX_ZONES; ++i){
      using ::testing::Return;
      if(i != ZONE_ACTIVE){
         EXPECT_CALL(*dynamic_cast<MockCalendar_routine*>(calendar[i]), is_event_active)
            .WillOnce(Return(false));
         EXPECT_CALL(*dynamic_cast<MockIrrigation_zone*>(zones[i]), is_irrigating)
            .WillOnce(Return(false));
      }else{
         EXPECT_CALL(*dynamic_cast<MockCalendar_routine*>(calendar[i]), is_event_active)
            .WillOnce(Return(true));
         EXPECT_CALL(*dynamic_cast<MockIrrigation_zone*>(zones[i]), is_irrigating)
            .WillOnce(Return(false));
         EXPECT_CALL(*dynamic_cast<MockIrrigation_zone*>(zones[i]), irrigate(true));
      }
   }
   irr_manager.process_events(rtc);
}

//5.3- If there is an event in more than one irrigation zone, those irrigation zones are active.
TEST_F(IrrigationZonesManagerTest, event_active_in_several_irrigation_zone){
   Irrigation_manager irr_manager(zones, calendar);
   MockRTC rtc;
   using ::testing::Return;
   EXPECT_CALL(rtc, get_time)
      .Times(1);
   for(auto i=0; i<MAX_ZONES; ++i){
      using ::testing::Return;
      EXPECT_CALL(*dynamic_cast<MockCalendar_routine*>(calendar[i]), is_event_active)
         .WillOnce(Return(true));
      EXPECT_CALL(*dynamic_cast<MockIrrigation_zone*>(zones[i]), is_irrigating)
         .WillOnce(Return(false));
      EXPECT_CALL(*dynamic_cast<MockIrrigation_zone*>(zones[i]), irrigate(true));
   }
   irr_manager.process_events(rtc);
}

//5.4- If there is no event active and irrigation zones are irrigating, then irrigating zones are inactive.}
TEST_F(IrrigationZonesManagerTest, events_inactive_irrigation_zones_active){
   Irrigation_manager irr_manager(zones, calendar);
   MockRTC rtc;
   using ::testing::Return;
   EXPECT_CALL(rtc, get_time)
      .Times(1);
   for(auto i=0; i<MAX_ZONES; ++i){
      using ::testing::Return;
      EXPECT_CALL(*dynamic_cast<MockCalendar_routine*>(calendar[i]), is_event_active)
         .WillOnce(Return(false));
      EXPECT_CALL(*dynamic_cast<MockIrrigation_zone*>(zones[i]), is_irrigating)
         .WillOnce(Return(true));
      EXPECT_CALL(*dynamic_cast<MockIrrigation_zone*>(zones[i]), irrigate(false));
   }
   irr_manager.process_events(rtc);
}

 //5.5- If there is a not initialized zone. The calendar for that zone is omitted.
TEST_F(IrrigationZonesManagerTest, process_event_zone_is_not_initialized){
   constexpr int INDEX_TO_TEST = 1;
   zones[INDEX_TO_TEST] = nullptr;
   Irrigation_manager irr_manager(zones, calendar);
   MockRTC rtc;
   using ::testing::Return;
   EXPECT_CALL(rtc, get_time)
      .Times(1);
   for(auto i=0; i<MAX_ZONES; ++i){
      using ::testing::Return;
      if(i != INDEX_TO_TEST){
         EXPECT_CALL(*dynamic_cast<MockCalendar_routine*>(calendar[i]), is_event_active)
            .WillOnce(Return(false));
         EXPECT_CALL(*dynamic_cast<MockIrrigation_zone*>(zones[i]), is_irrigating)
            .WillOnce(Return(false));
      }
   }
   irr_manager.process_events(rtc);
}


//5.6- If there is a not initialized calendar. The actions are omitted for that calendar.
TEST_F(IrrigationZonesManagerTest, process_event_calendar_is_not_initialized){
   constexpr int INDEX_TO_TEST = 1;
   calendar[INDEX_TO_TEST] = nullptr;
   Irrigation_manager irr_manager(zones, calendar);
   MockRTC rtc;
   using ::testing::Return;
   EXPECT_CALL(rtc, get_time)
      .Times(1);
   for(auto i=0; i<MAX_ZONES; ++i){
      using ::testing::Return;
      if(i != INDEX_TO_TEST){
         EXPECT_CALL(*dynamic_cast<MockCalendar_routine*>(calendar[i]), is_event_active)
            .WillOnce(Return(false));
         EXPECT_CALL(*dynamic_cast<MockIrrigation_zone*>(zones[i]), is_irrigating)
            .WillOnce(Return(false));
      }
   }
   irr_manager.process_events(rtc);
}


/////////
//Return events
//6.1 Returns the next event active from the desired zone.
TEST_F(IrrigationZonesManagerTest, return_next_event){
   Irrigation_manager irr_manager(zones, calendar);
   std::pair<tm, uint32_t> event_element;
   for(auto i=0; i<MAX_ZONES; ++i){
      using ::testing::Return;
      EXPECT_CALL(*dynamic_cast<MockCalendar_routine*>(calendar[i]), get_next_event)
         .WillOnce(Return(true));
      ASSERT_TRUE(irr_manager.get_next_event(i, event_element));     
   }
}

//6.2 Return  the next event active from an invalid zone, returns failure and has no effect.
TEST_F(IrrigationZonesManagerTest, return_next_event_in_invalid_zone){
   Irrigation_manager irr_manager(zones, calendar);
   std::pair<tm, uint32_t> event_element;
   ASSERT_FALSE(irr_manager.get_next_event(MAX_ZONES, event_element));
}

//6.3 Return the next event from a not initialized calendar, returns failure and has no effect.
TEST_F(IrrigationZonesManagerTest, return_next_event_in_not_initiated_calendar){
   constexpr int INDEX_TO_TEST = 1;
   calendar[INDEX_TO_TEST] = nullptr;
   Irrigation_manager irr_manager(zones, calendar);
   std::pair<tm, uint32_t> event_element;
   ASSERT_FALSE(irr_manager.get_next_event(INDEX_TO_TEST, event_element));
}

//6.4 In a zone sets the next event to be return from a desired start_time.
TEST_F(IrrigationZonesManagerTest, set_next_event_custom_time){
   Irrigation_manager irr_manager(zones, calendar);
   tm time_to_set_index;
   for(auto i=0; i<MAX_ZONES; ++i){
      using ::testing::Return;
      EXPECT_CALL(*dynamic_cast<MockCalendar_routine*>(calendar[i]), set_get_event)
         .WillOnce(Return(true));
      ASSERT_TRUE(irr_manager.set_get_event(i, time_to_set_index));
   }
}

//6.5 Try to set the next event on an invalid zone, returns failure and has no effect.
TEST_F(IrrigationZonesManagerTest, set_next_event_custom_time_in_invalid_zone){
   Irrigation_manager irr_manager(zones, calendar);
   tm time_to_set_index;
   ASSERT_FALSE(irr_manager.set_get_event(MAX_ZONES, time_to_set_index));
}

//6.6 Set the next event on a not initialized calendar, returns failure and has no effect.
TEST_F(IrrigationZonesManagerTest, set_next_event_custom_time_in_not_initiated_calendar){
   constexpr int INDEX_TO_TEST = 1;
   calendar[INDEX_TO_TEST] = nullptr;
   Irrigation_manager irr_manager(zones, calendar);
   tm time_to_set_index;
   ASSERT_FALSE(irr_manager.set_get_event(INDEX_TO_TEST, time_to_set_index));
}

//6.7 In a zone restarts the index to get the next event from the first event.
TEST_F(IrrigationZonesManagerTest, reset_get_next_event){
   Irrigation_manager irr_manager(zones, calendar);
   for(auto i=0; i<MAX_ZONES; ++i){
      using ::testing::Return;
      ASSERT_TRUE(irr_manager.reset_get_event(i));
   }
}

//6.8 Restart the index to get the next event from an invalid zone, returns failure and has no effect.
TEST_F(IrrigationZonesManagerTest, reset_get_next_event_in_invalid_zone){
   Irrigation_manager irr_manager(zones, calendar);
   ASSERT_FALSE(irr_manager.reset_get_event(MAX_ZONES));
}

//6.9 Restart the index to het the next event in a not initiated calendar. Returns failure and has no effect.
TEST_F(IrrigationZonesManagerTest, reset_get_next_event_in_not_initiated_calendar){
   constexpr int INDEX_TO_TEST = 1;
   calendar[INDEX_TO_TEST] = nullptr;
   Irrigation_manager irr_manager(zones, calendar);
   ASSERT_FALSE(irr_manager.reset_get_event(INDEX_TO_TEST));
}