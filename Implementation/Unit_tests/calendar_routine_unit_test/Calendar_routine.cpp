#include "Calendar_routine.h"
#include <algorithm>
#include <iterator>

/* ==== [Definitions] ======================================================= */
using calendar_pair = std::array<uint32_t, 2>;
using calendar_iter = std::forward_list<calendar_pair>::const_iterator;
using calendar_list = std::forward_list<calendar_pair>;
constexpr int SHIFT_WORD = 16;
constexpr int START_TIME_ELEMENT = 0;
constexpr int DURATION_ELEMENT = 1;
constexpr int ELEMENT = 1;
typedef enum{
    ERROR = 0,
    OK
}system_status_t;

/* ==== [Private function declaration] ====================================== */
static calendar_iter get_event_iter(const calendar_list &events_list, const tm time_in_event);
static calendar_iter get_previous_element(const calendar_list &events_list, calendar_iter element);
static uint32_t tm_to_start_time(const tm start_time);
static bool list_comp_function(calendar_pair list_element, uint32_t comp_val);

/* ==== [Public functions] ================================================== */
bool Calendar_routine_annual::add_event(const tm start_time, const uint32_t duration_in_minutes)
{
    uint32_t converted_start_time = tm_to_start_time(start_time);
    calendar_pair list_element{ converted_start_time, duration_in_minutes};
    bool returnValue = OK;
    //catch exceptions if there is any. Return false if there is an exception
    try{
        //Look where to insert the routine in the list. The list is sorted by start_time.
        using namespace std;
        auto iter_matching_condition = lower_bound(_events_list.begin(), _events_list.end(), converted_start_time, list_comp_function);
        if(_events_list.empty())
            _events_list.push_front(move(list_element));
        else {
            auto iter_to_insert = get_previous_element(_events_list, iter_matching_condition);
            _events_list.insert_after(iter_to_insert, move(list_element));
        }
        return returnValue;
    }
    catch(...)
    {
        returnValue = ERROR;
        return returnValue;
    }
}

bool Calendar_routine_annual::is_event_active(const tm time_in_event) const
{
    return get_event_iter(_events_list, time_in_event) != _events_list.end() ? true : false; 
}

bool Calendar_routine_annual::remove_event(const tm time_in_event)
{
    auto list_element = get_event_iter(_events_list, time_in_event);
    if(list_element == _events_list.begin())
        _events_list.pop_front();
    else if(list_element == _events_list.end())
        return false;
    return true;
    
}

/* ==== [Private functions] ================================================== */
/**
 * Looks for the event with time_in_event. Returns the first event found in the events_list.
 * If no envents match, returns events_list.end()
*/
calendar_iter get_event_iter(const std::forward_list<calendar_pair> &events_list, const tm time_in_event)
{
     uint32_t converted_time = tm_to_start_time(time_in_event);
     auto list_element = events_list.begin();
     for(; list_element!= events_list.end(); ++list_element)
     {
         uint32_t comp_start_time = (*list_element)[START_TIME_ELEMENT];
         uint32_t comp_final_time = comp_start_time + (*list_element)[DURATION_ELEMENT];   
         //query if time is in routine.
         if( converted_time >= comp_start_time && converted_time <= comp_final_time )
            return list_element;
     }
     return events_list.end();
}

/**
 * Returns an Iterator to the previous element in the event list based on the supplied element.
*/
static calendar_iter get_previous_element(const calendar_list &events_list, calendar_iter element)
{
    auto items = distance(events_list.before_begin(), element) - ELEMENT;
    auto previous_element = events_list.before_begin();
    advance(previous_element, items);
    return previous_element;
}
/**
 * tm_to_start_time: 
 * Converts the start_time used for add_event and return a value to be stored in class
 * 
 * @param start_time: tm value to be converted.
 * @return converted_value: The value has the following format: 
 *  -Bits 0-to-15: Starting time of the month expressed in minutes.(e.g The day 2 at 12hs would be 2*24*60 + 12*60 == 3600)
 *  -Bits 16-to-19: Number of month starting from 0.(e.g December == 11).
 *  
*/
static uint32_t tm_to_start_time(const tm start_time)
{  
    uint32_t converted_time = start_time.tm_mon;
    converted_time <<= SHIFT_WORD;
    converted_time &= 0xFFFF0000;
    converted_time += (24*60*start_time.tm_mday + 60*start_time.tm_hour + start_time.tm_min) & 0x0000FFFF;
    return converted_time;
}

/** 
 * Function to use with STL algorithms
 */
static bool list_comp_function(calendar_pair list_element, uint32_t comp_val)
{
    if(list_element[START_TIME_ELEMENT] < comp_val)
        return true;
    else
        return false;
}