#include "Calendar_routine.h"
#include <algorithm>
#include <iterator>
//#define NDEBUG
#include <cassert>

/* ==== [Definitions] ======================================================= */
using calendar_array = std::array<uint32_t, 2>;
using calendar_iter = std::forward_list<calendar_array>::iterator;
using calendar_list = std::forward_list<calendar_array>;
constexpr int SHIFT_WORD = 16;
constexpr int START_TIME_ELEMENT = 0;
constexpr int DURATION_ELEMENT = 1;
constexpr int ELEMENT = 1;
constexpr int MINUTES_IN_HOUR = 60; 
constexpr int MINUTES_IN_DAY  = 24*MINUTES_IN_HOUR;
typedef enum{
    ERROR = 0,
    IS_REPEATED = 0,
    NO_EVENT_TO_MERGE = 0,
    MAX_EVENTS_REACH = 0,
    INVALID_DURATION = 0,
    OK
}return_status_t;

/* ==== [Private functions declaration] ====================================== */
static bool             insert_event(calendar_list& events_list, calendar_iter& list_iter, calendar_array& list_element);
static calendar_iter    get_event_iter(calendar_list& events_list, const tm time_in_event);
static calendar_iter    get_event_iter(calendar_list& events_list, const calendar_array& list_element);
static calendar_iter    get_previous_element(calendar_list& events_list, calendar_iter list_iter);
static bool             merge_events(Calendar_routine_annual* calendar, calendar_list& events_list, calendar_array& list_element);
static uint32_t         tm_to_internal(const tm start_time);
static tm               internal_to_tm(const uint32_t start_time);
static bool             list_comp_lesser(calendar_array list_element, uint32_t comp_val);

/* ==== [Public functions definition] ================================================== */
bool Calendar_routine_annual::add_event(const tm start_time, const uint32_t duration_in_minutes) {
    using namespace std;
    try{
        if(_events_quantity >= MAX_EVENTS_ALLOW) 
            return MAX_EVENTS_REACH;
        if(duration_in_minutes == 0) 
            return INVALID_DURATION; 
        uint32_t converted_start_time = tm_to_internal(start_time);
        calendar_array list_element{ converted_start_time, duration_in_minutes};
        //Checks if the routine is not repited.
        auto list_iter = find(_events_list.begin(), _events_list.end(), list_element);
        if(list_iter != _events_list.end()) 
            return IS_REPEATED;
        //Checks if the routine must be merge
        auto last_event_quantity = _events_quantity;
        for(auto i=0; i<last_event_quantity; ++i){
            if( merge_events(this, _events_list, list_element) == NO_EVENT_TO_MERGE)
                break;
        }
        //Look where to insert the routine in the list. The list is sorted by start_time.
        auto iter_matching_condition = lower_bound(_events_list.begin(), _events_list.end(), converted_start_time, list_comp_lesser);
        if(_events_list.empty())
            _events_list.push_front(move(list_element));
        else
            insert_event(_events_list, iter_matching_condition, list_element);
        ++_events_quantity;
        return OK;
    }
    catch(...) {return ERROR;}   
}

bool Calendar_routine_annual::is_event_active(const tm time_in_event) {
    return get_event_iter(_events_list, time_in_event) != _events_list.end() ? true : false; 
}

bool Calendar_routine_annual::get_next_event(std::pair<tm, uint32_t>& event_to_return) {
    if(_events_quantity <= 0){
        event_to_return.second = 0;
        return false;
    }
    //check _return_iter integrity
    if( std::distance(_events_list.begin(), _return_iter) == std::distance(_events_list.begin(), _events_list.end()))
        _return_iter = _events_list.begin();
    calendar_array event = *_return_iter;
    if(_return_iter != _events_list.end())
        ++_return_iter;
    else
        _return_iter = std::next(_events_list.begin(), 0);
    //make the return value
    tm start_time = internal_to_tm(event[START_TIME_ELEMENT]); 
    event_to_return.first = std::move(start_time); 
    event_to_return.second = event[DURATION_ELEMENT];  
    return true;
}

void Calendar_routine_annual::set_get_event(const tm& time_to_set_index ){
    uint32_t converted_time = tm_to_internal(time_to_set_index);
    auto iter = std::lower_bound(_events_list.begin(), _events_list.end(), converted_time, list_comp_lesser);
    if(iter != _events_list.end())
        _return_iter = iter;
}

bool Calendar_routine_annual::remove_event(const tm time_in_event) {
    auto list_element = get_event_iter(_events_list, time_in_event);
    return remove_event(list_element);
}

bool Calendar_routine_annual::remove_event(calendar_iter element) {
    if(element == _events_list.end() || element == _events_list.before_begin())
        return false;
    else if( element == _events_list.begin())
        _events_list.pop_front();
    else
        _events_list.erase_after(get_previous_element(_events_list, element));
    --_events_quantity;
    return true;        
}

/* ==== [Private functions] ================================================== */
/**
 * 
*/
static bool insert_event(calendar_list& events_list, calendar_iter& list_iter, calendar_array& list_element) {
    auto iter_to_insert = get_previous_element(events_list, list_iter);
    events_list.insert_after(iter_to_insert, move(list_element));
    return true;
}
/**
 * Looks for the event with time_in_event. Returns the first event found in the events_list.
 * If no envents match, returns events_list.end()
*/
//TODO: IMPORTANT!! THIS FUNCTION CAN BE OPTIMIZED TO START FROM THE LAST QUERY ITER
static calendar_iter get_event_iter(calendar_list& events_list, const tm time_in_event) {
     uint32_t converted_time = tm_to_internal(time_in_event);
     calendar_array list_element{converted_time, 0};
     return get_event_iter(events_list, list_element);     
}

/**
 * 
*/
static calendar_iter get_event_iter(calendar_list& events_list, const calendar_array& list_element){
     uint32_t start_time_A = list_element[START_TIME_ELEMENT];
     uint32_t final_time_A = start_time_A + list_element[DURATION_ELEMENT];
     auto list_iter = events_list.begin();
     //checks if there is any intersection with other elements.
     for(; list_iter!= events_list.end(); ++list_iter) {
         uint32_t start_time_B = (*list_iter)[START_TIME_ELEMENT];
         uint32_t final_time_B = start_time_B + (*list_iter)[DURATION_ELEMENT];   
         //Cases without intersection:
         //| =============================== [Time axis] ============================================== |  
         //|(start_time_A)------(final_time_A)----------------------------------------------------------|
         //|-------------------------------------(start_time_B)-------------------------(final_time_B)--|
         //| =============================== [Time axis] ============================================== |
         //|-------------------------------------(start_time_A)-------------------------(final_time_A)--|
         //|(start_time_B)------(final_time_B)----------------------------------------------------------|
         bool there_is_intersection = !(final_time_A < start_time_B || final_time_B < start_time_A);
         if(there_is_intersection)
            return list_iter;
     }
     return events_list.end();
}

/**
 * Returns an Iterator to the previous element in the event list based on the supplied element.
*/
static calendar_iter get_previous_element(calendar_list &events_list, calendar_iter list_iter) {
    auto items = distance(events_list.before_begin(), list_iter) - ELEMENT;
    calendar_iter previous_element = events_list.before_begin();
    advance(previous_element, items);
    return previous_element;
}

/**
 * 
*/
static bool merge_events(Calendar_routine_annual* calendar, calendar_list& events_list, calendar_array& list_element) {
    //get iter that has a time in common with other event, in which intersectionAB != {0}  |event A (| intersectionAB |) event B|
    auto iter_of_list_match = get_event_iter(events_list, list_element);
    if(iter_of_list_match == events_list.end())
        return false;       
    //process list_element to be unionAB
    uint32_t start_time_A = list_element[START_TIME_ELEMENT];
    uint32_t final_time_A = start_time_A +  list_element[DURATION_ELEMENT];
    uint32_t start_time_B = (*iter_of_list_match)[START_TIME_ELEMENT];
    uint32_t final_time_B = start_time_B + (*iter_of_list_match)[DURATION_ELEMENT];
    if(start_time_A > start_time_B){
        start_time_A = start_time_B;
        list_element[START_TIME_ELEMENT] = start_time_A;
    }
    if(final_time_A < final_time_B)
        final_time_A = final_time_B;   
    assert(final_time_A - start_time_A >= 0);
    list_element[DURATION_ELEMENT] = final_time_A - start_time_A;   
    //insert element
    insert_event(events_list, iter_of_list_match, list_element);
    //erase eventB from list
    calendar->remove_event(iter_of_list_match);
    return true;
}

/**
 * tm_to_internal: 
 * Converts the start_time used for add_event and return a value to be stored in class
 * 
 * @param start_time: tm value to be converted.
 * @return converted_value: The value has the following format: 
 *  -Bits 0-to-15: Starting time of the month expressed in minutes.(e.g The day 2 at 12hs would be 2*24*60 + 12*60 == 3600)
 *  -Bits 16-to-19: Number of month starting from 0.(e.g December == 11).
 *  
*/
static uint32_t tm_to_internal(const tm start_time) {  
    uint32_t converted_time = start_time.tm_mon;
    //convert to internal
    converted_time <<= SHIFT_WORD;
    converted_time &= 0xFFFF0000;
    converted_time += (24*60*start_time.tm_mday + 60*start_time.tm_hour + start_time.tm_min) & 0x0000FFFF;
    return converted_time;
}

/**
 * 
*/
static tm internal_to_tm(const uint32_t start_time){
    tm time_to_return = {0};
    time_to_return.tm_mon = start_time >> SHIFT_WORD;
    uint32_t total_minutes = start_time & 0x0000FFFF;
    time_to_return.tm_min = total_minutes%MINUTES_IN_HOUR;
    time_to_return.tm_hour = (total_minutes/MINUTES_IN_HOUR)%MINUTES_IN_DAY;
    time_to_return.tm_mday = total_minutes/MINUTES_IN_DAY;
    return std::move(time_to_return);
}

/** 
 * Function to use with STL algorithms
 */
static bool list_comp_lesser(calendar_array list_element, uint32_t comp_val) {
    if(list_element[START_TIME_ELEMENT] < comp_val)
        return true;
    else
        return false;
}