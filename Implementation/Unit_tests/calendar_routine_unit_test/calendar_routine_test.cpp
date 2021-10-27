#include <gtest/gtest.h>
#include "Calendar_routine.h"

/*************************
 *  Calendar Routine Test
 * ***********************
 *  The main requirements are:
 * 1- Construct a calendar routine without data.
 * //Adding routines:
 * 2.1- Add a routine with a start time and a duration in minutes. Check if it is active in a valid time.
 * 2.2- Add a routine with a start time and a duration in minutes. Check if it is inactive in an invalid time.
 * 2.3- If a user tries to add a routine twice, the routine is added only once.
 * 2.4- If a user adds a routine that share time with another. The routine is changed to the merge of both.
 * 2.5- If a user adds a routine that share time with another and acts as a bridge with others routines. All routines as merge as one.
 * 2.6- if MAX_ROUTINES has been reached, add a routine has no effect.
 * //Erasing routines:
 * 3.1- Erase a routine with an instant included in the event to erase.
 * 3.2- Erase a routine onto a empty list has no effect.
 * 3.3- Erase the same routing twice, only erase the routine 1 time. The second attempt has no effect.
 * 
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
TEST_F(CalendarRoutineTest, store_anual_routines) {
   std::unique_ptr<Calendar_routine> calendar{new Calendar_routine_annual}; 
}


/////////////
//Adding routine
//2.1- Add a routine with a start time and a duration in minutes. Check if it is active in a valid time.
TEST_F(CalendarRoutineTest, add_routine_checkEvent) {
    std::unique_ptr<Calendar_routine> calendar{new Calendar_routine_annual}; 
    bool success = calendar->add_event( start_time, duration_in_minutes);
    ASSERT_EQ(true, success);

    tm time_to_check = start_time;
    bool is_active = calendar->is_event_active( time_to_check );
    ASSERT_EQ(true, is_active);
}

//2.2- Add a routine with a start time and a duration in minutes. Check if it is inactive in an invalid time.
TEST_F(CalendarRoutineTest, add_routine_check_no_Event) {
    std::unique_ptr<Calendar_routine> calendar{new Calendar_routine_annual}; 
   
    bool success = calendar->add_event( start_time, duration_in_minutes);
    ASSERT_EQ(true, success);

    tm time_to_check = start_time;
    time_to_check.tm_mon = 8;
    bool is_active = calendar->is_event_active( time_to_check );
    ASSERT_EQ(false, is_active);
}

//2.3- If a user tries to add a routine twice, the routine is added only once.
TEST_F(CalendarRoutineTest, add_routine_twice)
{
    std::unique_ptr<Calendar_routine> calendar{new Calendar_routine_annual};
    auto calendar_ptr = calendar.get();
    ASSERT_EQ( 0, Calendar_debug::get_routines_quantity(calendar_ptr));
   
    bool success = calendar->add_event( start_time, duration_in_minutes);
    ASSERT_EQ( 1, Calendar_debug::get_routines_quantity(calendar_ptr));
    ASSERT_EQ(true, success);
    success = calendar->add_event( start_time, duration_in_minutes);
    ASSERT_EQ(false, success);
    ASSERT_EQ( 1, Calendar_debug::get_routines_quantity(calendar_ptr));
    
}

////////////
//Erase routine
//3.1- Erase a routine with an instant included in the event to erase.
TEST_F(CalendarRoutineTest, erase_routine_check)
{
    std::unique_ptr<Calendar_routine> calendar{new Calendar_routine_annual};
    calendar->add_event(start_time, duration_in_minutes);
    ASSERT_EQ( true, calendar->is_event_active(start_time));

    calendar->remove_event(start_time);
    ASSERT_EQ( false, calendar->is_event_active(start_time));
}