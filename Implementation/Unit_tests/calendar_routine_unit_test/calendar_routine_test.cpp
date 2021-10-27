#include <gtest/gtest.h>
#include "Calendar_routine.h"

/*************************
 *  Calendar Routine Test
 * ***********************
 *  The main requirements are:
 * 1- Construct a calendar routine without data.
 * 2- Add a routine with a start time and a duration in minutes. Check if it is active in a valid time.
 * 3- Add a routine with a start time and a duration in minutes. Check if it is inactive in an invalid time.
 * 4- Erase a routine with an instant included in the event to erase.
 * 5- If a user tries to add a routine twice, the routine is added only once.
 * 6- If a user adds a routine wi
 *
 * ****************************************************************************
 * Author: Ariel Cerfoglia
 * Email: ariel.cerfoglia@gmail.com
 * ****************************************************************************
*/ 

class CalendarRoutineTest: public ::testing::Test {
protected:
    void SetUp() override
    {
        start_time.tm_hour = 10;
        start_time.tm_year = 2021 - 1900;
        start_time.tm_mon = 9;              
    }
    tm start_time{};
    uint32_t duration_in_minutes = 60; 
};

/*STARTING TESTS*/
//1- Construct a calendar routine without data.
TEST_F(CalendarRoutineTest, calendarRoutine_store_anual_routines) {
   std::unique_ptr<Calendar_routine> calendar{new Calendar_routine_annual}; 
}


//2- Add a routine with a start time and a duration in minutes. Check if it is active in a valid time.
TEST_F(CalendarRoutineTest, calendarRoutine_add_routine_checkEvent) {
    std::unique_ptr<Calendar_routine> calendar{new Calendar_routine_annual}; 
    bool success = calendar->add_event( start_time, duration_in_minutes);
    ASSERT_EQ(true, success);

    tm time_to_check = start_time;
    bool is_active = calendar->is_event_active( time_to_check );
    ASSERT_EQ(true, is_active);
}


//3- Add a routine with a start time and a duration in minutes. Check if it is inactive in an invalid time.
TEST_F(CalendarRoutineTest, calendarRoutine_add_routine_check_no_Event) {
    std::unique_ptr<Calendar_routine> calendar{new Calendar_routine_annual}; 
   
    bool success = calendar->add_event( start_time, duration_in_minutes);
    ASSERT_EQ(true, success);

    tm time_to_check = start_time;
    time_to_check.tm_mon = 8;
    bool is_active = calendar->is_event_active( time_to_check );
    ASSERT_EQ(false, is_active);
}

//4- Erase a routine with an instant included in the event to erase.
TEST_F(CalendarRoutineTest, calendarRoutine_erase_routine_check)
{
    std::unique_ptr<Calendar_routine> calendar{new Calendar_routine_annual};
    calendar->add_event(start_time, duration_in_minutes);
    ASSERT_EQ( true, calendar->is_event_active(start_time));

    calendar->remove_event(start_time);
    ASSERT_EQ( false, calendar->is_event_active(start_time));
}