# Monitoring and control irrigation system  
  
## Software Requirement Specifications  
Version: v1.0  
Date: September 3, 2021.  
Author: Cerfoglia Ariel  

## Revisions
| Date       |  Description         | Author          |
| ------     | -------------------- | --------------- |
| 09/03/2021 |  1.0 Initial version |Cerfoglia Ariel  |

## Content 
### 1. INTRODUCTION
#### 1.1. Purpose
### 2. OVERALL DESCRIPTION  
### 3. SPECIFIC REQUERIMENTS
#### 3.1 External interfaces
##### 3.1.1 User interfaces
##### 3.1.2 Hardware interfaces
##### 3.1.3 Software interfaces
##### 3.1.4 Communication protocols and interfaces
#### 3.2 Functional requeriments
#### 3.3 Use cases
#### 3.4 Classes / Objects
----------------------------------------------

## 1.Introduction  
  This project is a final project to achieve an embedded system specialist degree.
### 1.1 Purpose
  This document indicates the requeriments for a system of control and monitoring irrigation, part of an internet of things node.  
  The design criteria and arquitecture is exposed to show and guide the development process.  

## 2.Overall description  
The main responsabilities of the control and monitoring system are:
- Be capable of comunicatte througth a LoRaWAN net.
- Manage an auxiliary source subsystem when the 220/110 Vac is not available.
- Manage presion sensors.
- Manage electrovalves and a pump.
- Warning about failures.
  ![system](https://user-images.githubusercontent.com/72839552/132056941-11d8951f-46ed-4417-aa22-e17dcdcfbfd4.PNG)
  
The system composition is:  
![Main Subsystems](https://user-images.githubusercontent.com/72839552/133168090-f4cc3a4b-6ab9-40e3-bf07-922584a07708.jpg)
 
Each subsystem is working in parallel to each other.

----------------------------------------------
## 3. SPECIFIC REQUERIMENTS
#### 3.1 External interfaces
#### 3.1.1 User interfaces
#### 3.1.2 Hardware interfaces
#### 3.1.3 Software interfaces  
- FreeRTOS: RTOS.  
- STM32CubeIde: Main IDE.  
- Google Test: Unit test framework for C++.  

#### 3.1.4 Communication protocols and interfaces
----------------------------------------------
### 3.2 Functional requeriments
##### 3.2.1 SENSORS-FR1
###### 3.2.1.1 *Requeriment:* The system have a maximun of 8 presion sensors.
##### 3.2.2 SENSORS-FR2
###### 3.2.2.1 *Requeriment:* The posible configuration for each sensor is: NO-SENSOR, SWITCH, 4-20mA.
##### 3.2.3 SENSORS-FR3
###### 3.2.3.1 *Requeriment:* Each sensor can be configured independently from others.
##### 3.2.4 ELECTROVALVE-FR1
###### 3.2.4.1 *Requeriment:* The system have a maximun of 8 electrovalves.
##### 3.2.5 ELECTROVALVE-FR2
###### 3.2.5.1 *Requeriment:* 
##### 3.2.6 ELECTROVALVE-FR3
###### 3.2.6.1 *Requeriment:* Only one electrovalve can be open each time. 
###### 3.2.6.2 *Description:* The sequense of transition is: valve_1 is open, valve_2 is opened, valve_1 is closed.
##### 3.2.7 PUMP-FR1
###### 3.2.7.1 *Requeriment:* The pump is activated only if there is an electrovalve open.
##### 3.2.8 PUMP-FR2
###### 3.2.8.1 *Requeriment:* Before close all electrovalves, the pump is desactivated.
##### 3.2.9 POWER_SYSTEM-FR1
###### 3.2.9.1 *Requeriment:* If voltage <= 0,7*220Vac then the primary power system is desactivated and secondary power system is activated.
##### 3.2.10 POWER_SYSTEM-FR2
###### 3.2.10.1 *Requeriment:* If voltage >= 0,8*220Vac then the secondary power system is activated and primary power system is activated.
##### 3.2.11 POWER_SYSTEM-FR3
###### 3.2.11.1 *Requeriment:* If secondary power system is active then the system is low consumption.
###### 3.2.11.2 *Description:* In low consumption only alarm system and communication system are active. Secondary power system can be active for a week.
##### 3.2.12 CONTROL_UNIT-FR1
###### 3.2.12.1 *Requeriment:* For each electrovalve the system can irrigate the irrigation zone with a monthly calendar.
###### 3.2.12.2 *Description:* On the calendar the user can select the day and starting hour. The time of irrigation is expresed as minutes.
##### 3.2.13 CONTROL_UNIT-FR2
###### 3.2.12.1 *Requeriment:* The user can manually irrigate an irrigation zone ONLY if there is no connection with server. The manual irrigation has priority over calendar irrigation.
##### 3.2.14 CONTROL_UNIT-FR3
###### 3.2.14.1 *Requeriment:* On rainy days the system have to cancel the irrigation orders for that day.
##### 3.2.15 COMMUNICATION_UNIT-FR1
###### 3.2.15.1 *Requeriment:* The communication unit must send orders and configurations from server to system. The success or failure of orders and configurations must be sent from system to server.
##### 3.2.16 COMMUNICATION_UNIT-FR2
###### 3.2.16.1 *Requeriment:* The communication unit must send the system state from server to system.
----------------------------------------------

### 3.3 Use cases
----------------------------------------------
### 3.4 Classes / Objects
The diagram of the whole system is:  

![Main Subsystems](https://user-images.githubusercontent.com/72839552/133168102-09984a9c-5dea-42c5-9d08-149271a51b08.jpg)


The Control Unit is the system core. It is responsible of execute the systems routines.

The Power Unit is responsible to sense the principal power source and determine if it's necessary to switch to the secondary power source.

The Communication Unit is responsible to bring and send data from and to server. Also, must to share with the control unit the healthy of connection.

----------------------------------------------

#### 3.4.1 Control Unit
Requeriments
- Facade to access system status holder, irrigation manager and control unit factory.

##### The class diagram of the control unit is:  
![Control Unit](https://user-images.githubusercontent.com/72839552/133838288-4bffdcb6-3e85-4439-9567-0670cb3daac3.jpg)

#### 3.4.2 Scheduler
Requeriments
- Schedule control unit operations.
- Manage irrigation routines.
- Manage manual irrigation.
- Communicate system status.
- Set system on low power state if is requested by power unit.
##### 3.4.2.1 Attributes: N/A
##### 3.4.2.2 Member functions: N/A 

#### 3.4.3 Control unit factory
Requeriments
- Instantiate control unit components.  
- Allow change sensor on run-time.  

#### 3.4.4 System status holder  
Requeriments  
- Keep track on system status: zones healthy, power source, communication status.  

##### 3.4.4.1 Attributes: N/A
##### 3.4.4.2 Member functions: N/A

#### 3.4.5 Irrigation manager
Requeriments  
- Allow manual control of irrigation zones.  
- Checks state of irrigation zones.  
- Execute irrigation routines.  
- Manage routines.  

##### 3.4.5.1 Attributes: 
- routine::iterator: foward_iterator returning pairs<time_t start_time, uint32_t duration_minutes>

##### 3.4.5.2 Member functions:  
- void irrigate( uint8_t zone_from_1, bool must_irrigate )
- bool is_irrigating( uint8_t zone_from_1 )
- void process_routine()
- bool add_routine( uint8_t zone_from_1, time_t start_time, uint32_t duration_minutes ): Returns success or failure.
- bool clean_routine( uint8_t zone_from_1, time_t time_in_routine ): Returns success or failure.
- routine::iterator get_routines( uint8_t zone_from_1 )  

#### 3.4.6 Calendar routine
Requeriments
- Track a periodic programmed event. The periodicity policy is selected on instantiation.
- Allow to add programmed events and remove them.
- Indicates if a programmed event is active.  

##### 3.4.6.1 Attributes: N/A  
##### 3.4.6.2 Member functions: 
- bool add_event( time_t start_time, uint32_t minutes_duration ): Returns success or failure.
- bool clean_event( time_t date_inside_event ). Returns succes or failure.
- void clean_all()
- void is_event_active()


#### 3.4.7 Storage Manager  
- Manage storage in non-volatile memmory.  
##### 3.4.7.1 Attributes: N/A  
##### 3.4.7.2 Member functions:  

#### 3.4.8 Clock
Requeriments
- Give time
- Set time  

##### 3.4.8.1 Attributes: N/A  
##### 3.4.8.2 Member functions:  
- time_t get_time()
- void set_time( time_t actual_time )

#### 3.4.9 Irrigation Zone
![Irrigation Zone](https://user-images.githubusercontent.com/72839552/135307235-cac14ff7-a2c1-4bf9-91b7-f767d2379526.jpg)

Requeriments
- Irrigate Zone
- Indicates pipes healthy

##### 3.4.9.1 Attributes: N/A  
##### 3.4.9.2 Member functions:  
- void irrigate( bool must_irrigate )
- bool is_irrigating()
- bool is_sensing_health()
- bool is_healthy()

#### 3.4.10 Actuator
Requeriments:
- Use a gpio.
- Control actuator state.
- Query actuator state.
- Manage sensor.
##### 3.4.10.1 Attributes: N/A

##### 3.4.10.2 Member functions:  
  - void set( bool set_ON )
  - bool is_ON()  
  - bool has_sensor()
  - uint32_t read_sensor()


#### 3.4.11 Sensor_SI  
![Sensor](https://user-images.githubusercontent.com/72839552/134406251-e9eb02a1-eb6b-4c8b-90b6-2c73a001b29a.jpg)


Requeriments:
- Returns measure value.  
- Returns measure unit.
- Returns sensor instances.
    
##### 3.4.11.1 Attributes:  N/A  
##### 3.4.11.2 Member functions:  
  - uint32_t read(): In case of switch, return value is numeric_limits::max(uint32_t).
  - int get_instances_num()
  - string get_unit(): unit is lower case.

#### 3.4.12 Communication Unit  
![Communication Unit](https://user-images.githubusercontent.com/72839552/133290935-5664190c-5a3e-4ce4-b1e7-3812647f24a9.jpg)  

#### 3.4.13 Communication Factory  
Requeriments
- Construct communication components.  

#### 3.4.14 Communication Driver  
Requeriments  
- Manage specific communication protocol.  
- Send data.  
- Receive data.  
#### 3.4.15 Unpackager  
Requeriments  
- Prepares arriving data to be used by system.
#### 3.4.16 Packager  
Requeriments  
- Prepares sent data to be used by server.  

#### 3.4.17 Parser  
Requeriments  
- Translate data from a format to another.

----------------------------------------------


