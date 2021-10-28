#include <gtest/gtest.h>
#include "Calendar_routine.h"

/*************************
 *  Calendar Routine Test
 * ***********************
 *  The main requirements are:
 * 
 * 1- Construct a calendar routine without data.
 * //Adding events:
 * 2.1- Add an event with a start time and a duration in minutes. Check if it is active in a valid time.
 * 2.2- Add an event with a start time and a duration in minutes. Check if it is inactive in an invalid time.
 * 2.3- If a user tries to add an event twice, the event is added only once.
 * 2.4- If a user adds an event that shares time with another. The event is changed to merge both.
 * 2.5- If a user adds an event that shares time with another event and such acts as a bridge with other events, 
 * then all events involved are merged as one.
 * 2.6- if MAX_EVENTS_ALLOW has been reached, add a event has no effect.
 * 
 * //Erasing events:
 * 3.1- Erase an event with an instant included in the event to erase.
 * 3.2- Erase an event on an empty list has no effect.
 * 3.3- Erase the same event twice, only erase the routine 1 time. The second attempt has no effect.
 * 
 * //Returning events:
 * 4.1- if it is an empty routine. Returns {0,0}
 * 4.1- Return an event, then return the next one.
 * 4.2- If Return the last event, the next returned event is the first one.
 * 4.3- Reset the returning event to start receiving from the first element.
 * 4.4- Return from an event above or equal to a specific.
 * 4.5- If was solicited a returned event above a specific time, call to return event returns the next event.
 * 4.7- If was solicited a returned event above a specific time and the condition cannot meet, then the return value is {0,0} and the index holds 
 * 
 * ****************************************************************************
 * Author: Ariel Cerfoglia
 * Email: ariel.cerfoglia@gmail.com
 * ****************************************************************************
*/ 

class CalendarRoutineTest: public ::testing::Test {
protected:
    void SetUp() override {
        start_time.tm_hour = 10;
        start_time.tm_year = 2021 - 1900;
        start_time.tm_mon = 9;              
    }
    tm start_time{};
    uint32_t duration_in_minutes = 60; 
};

/*STARTING TESTS*/
//1- Construct a calendar routine without data.
TEST_F(CalendarRoutineTest, construct_anual_routines) {
   std::unique_ptr<Calendar_routine> calendar{new Calendar_routine_annual}; 
}


/////////////
//Adding routine
//2.1- Add a routine with a start time and a duration in minutes. Check if it is active in a valid time.
TEST_F(CalendarRoutineTest, add_event_and_check_it) {
    std::unique_ptr<Calendar_routine> calendar{new Calendar_routine_annual}; 
    ASSERT_TRUE(calendar->add_event(start_time, duration_in_minutes));
    tm time_to_check = start_time;
    ASSERT_TRUE(calendar->is_event_active(time_to_check));
}

//2.2- Add a routine with a start time and a duration in minutes. Check if it is inactive in an invalid time.
TEST_F(CalendarRoutineTest, add_event_check_other) {
    std::unique_ptr<Calendar_routine> calendar{new Calendar_routine_annual}; 
    ASSERT_TRUE(calendar->add_event(start_time, duration_in_minutes));
    tm time_to_check = start_time;
    time_to_check.tm_mon -= 1;
    ASSERT_FALSE(calendar->is_event_active(time_to_check));
}

//2.3- If a user tries to add a routine twice, the routine is added only once.
TEST_F(CalendarRoutineTest, add_same_event_twice) {
    std::unique_ptr<Calendar_routine> calendar{new Calendar_routine_annual};
    auto calendar_ptr = calendar.get();
    ASSERT_EQ( 0, Calendar_debug::get_events_quantity(calendar_ptr));  
    ASSERT_TRUE(calendar->add_event( start_time, duration_in_minutes));
    ASSERT_EQ( 1, Calendar_debug::get_events_quantity(calendar_ptr));
    ASSERT_FALSE(calendar->add_event( start_time, duration_in_minutes));
    ASSERT_EQ( 1, Calendar_debug::get_events_quantity(calendar_ptr));    
}

//2.4- If a user adds a routine that shares time with another. The routine is changed to merge both.
TEST_F(CalendarRoutineTest, add_one_event_to_merge_two) {
    std::unique_ptr<Calendar_routine> calendar{new Calendar_routine_annual};
    auto calendar_ptr = calendar.get();
    duration_in_minutes = 60;
    ASSERT_TRUE(calendar->add_event( start_time, duration_in_minutes));
    ASSERT_EQ( 1, Calendar_debug::get_events_quantity(calendar_ptr));
    start_time.tm_hour += 1;
    duration_in_minutes = 120;
    ASSERT_TRUE(calendar->add_event( start_time, duration_in_minutes));
    ASSERT_EQ( 1, Calendar_debug::get_events_quantity(calendar_ptr));   
}

//2.5- If a user adds a routine that shares time with another routine and such acts as a bridge with other routines, 
//then all routines involved are merged as one.
TEST_F(CalendarRoutineTest, add_one_event_to_merge_three) {
    std::unique_ptr<Calendar_routine> calendar{new Calendar_routine_annual};
    auto calendar_ptr = calendar.get();
    duration_in_minutes = 60;
    ASSERT_TRUE(calendar->add_event( start_time, duration_in_minutes));
    ASSERT_EQ( 1, Calendar_debug::get_events_quantity(calendar_ptr));
    start_time.tm_hour += 2;
    ASSERT_TRUE(calendar->add_event( start_time, duration_in_minutes));
    ASSERT_EQ( 2, Calendar_debug::get_events_quantity(calendar_ptr));
    start_time.tm_hour -= 1; 
    //the 3rd event is adyacent to 1st and 2nd event, so the event must merge.
    ASSERT_TRUE(calendar->add_event( start_time, duration_in_minutes));
    ASSERT_EQ( 1, Calendar_debug::get_events_quantity(calendar_ptr));
}

//2.6- If MAX_EVENTS_ALLOW has been reached, add a event has no effect.
TEST_F(CalendarRoutineTest, add_MAX_events) {
    std::unique_ptr<Calendar_routine> calendar{new Calendar_routine_annual};
    auto calendar_ptr = calendar.get();
    for(auto i = 0; i<MAX_EVENTS_ALLOW; ++i){
        ASSERT_TRUE(calendar->add_event(start_time, duration_in_minutes));
        ASSERT_EQ(i+1, Calendar_debug::get_events_quantity(calendar_ptr));
        start_time.tm_hour += 2;
        if(start_time.tm_hour>= 23)
            ++start_time.tm_mday;
        if(start_time.tm_mday>26)
            ++start_time.tm_mon;
    }
    for(auto i = 0; i<MAX_EVENTS_ALLOW; ++i){
        ASSERT_FALSE(calendar->add_event(start_time, duration_in_minutes));
        ASSERT_EQ(MAX_EVENTS_ALLOW, Calendar_debug::get_events_quantity(calendar_ptr));
    }
}

////////////
//Erase routine
//3.1- Erase a routine with an instant included in the event to erase.
TEST_F(CalendarRoutineTest, erase_event_check) {
    std::unique_ptr<Calendar_routine> calendar{new Calendar_routine_annual};
    ASSERT_TRUE(calendar->add_event(start_time, duration_in_minutes));
    ASSERT_TRUE(calendar->is_event_active(start_time));
    ASSERT_TRUE(calendar->remove_event(start_time));
    ASSERT_FALSE(calendar->is_event_active(start_time));
}

//3.2- Erase a routine on an empty list has no effect.
TEST_F(CalendarRoutineTest, erase_empty) {
    std::unique_ptr<Calendar_routine> calendar{new Calendar_routine_annual};
    Calendar_routine* calendar_ptr = calendar.get();
    ASSERT_EQ(0, Calendar_debug::get_events_quantity(calendar_ptr));
    ASSERT_FALSE(calendar->remove_event(start_time));
    ASSERT_EQ(0, Calendar_debug::get_events_quantity(calendar_ptr));
}

//3.3- Erase the same event twice, only erase the routine 1 time. The second attempt has no effect.
TEST_F(CalendarRoutineTest, erase_same_event_twice) {
    std::unique_ptr<Calendar_routine> calendar{new Calendar_routine_annual};
    Calendar_routine* calendar_ptr = calendar.get();
    ASSERT_TRUE(calendar->add_event(start_time, duration_in_minutes));
    ASSERT_EQ(1, Calendar_debug::get_events_quantity(calendar_ptr));
    start_time.tm_mon++;
    ASSERT_TRUE(calendar->add_event(start_time, duration_in_minutes));
    ASSERT_EQ(2, Calendar_debug::get_events_quantity(calendar_ptr));
    ASSERT_TRUE(calendar->remove_event(start_time));
    ASSERT_EQ(1, Calendar_debug::get_events_quantity(calendar_ptr));
    ASSERT_FALSE(calendar->remove_event(start_time));
    ASSERT_EQ(1, Calendar_debug::get_events_quantity(calendar_ptr));
}
