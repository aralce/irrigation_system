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
![Main Subsystems](https://user-images.githubusercontent.com/72839552/133090483-dd62eda0-fdfd-4727-8200-6367c05dd504.jpg)  
Each subsystem is working in parallel to each other.

----------------------------------------------
## 3. SPECIFIC REQUERIMENTS
#### 3.1 External interfaces
#### 3.1.1 User interfaces
#### 3.1.2 Hardware interfaces
#### 3.1.3 Software interfaces
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
###### 3.2.5.1 *Requeriment:* The user can indicate how many liters per minute each valve pulls
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
###### 3.2.12.2 *Description:* On the calendar the user can select the day and starting hour. The time of irrigation is expresed as minutes or as liters.
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

![Main Subsystems](https://user-images.githubusercontent.com/72839552/133090483-dd62eda0-fdfd-4727-8200-6367c05dd504.jpg)

The Control Unit is the system core, so its simplified composition is shown here.
Each irrigation zone has an electrovalve to irrigate and a unique pump which is shared by all irigation zones.
Electrovalve and pump can have a sensor. The irrigation zone use these measures to determine the healthy of the pipe.

The Power Unit is responsible to sense the principal power source and determine if it's necessary to switch to the secondary power source.

The Communication Unit is responsible to bring and send data from and to server. Also, must to share with the control unit the healthy of connection.

----------------------------------------------

#### 3.4.1 Control Unit
##### The responsibilities of the control unit class are:
##### - Manage irrigation routines.
##### - Manage manual irrigation.
##### - Communicate system status.
##### - Set system on low power state if is requested by power unit.

##### The class diagram of the control unit is:
![Control Unit](https://user-images.githubusercontent.com/72839552/133093298-c60630d2-4a3b-4570-b667-7e1229b7cf4f.jpg) 

#### 3.4.2 Control unit factory
##### It's responsible to instantiate the components of control unit. It allows the system to change sensors types on run-time.
#### 3.4.3 System status holder
##### It's responsible to store system variables related to system health.
#### 3.4.4 Irrigation manager
##### It's responsible to manage irrigation operations. It controls the irrigation zones directly and schedules the irrigation routines.
#### 3.4.5 Irrigation storage manager
##### It stores the irrigation routines with a policies such as monthly or weekly. It manages the store and read data way.
#### 3.4.6 Timer
##### It's a timer to schedule time events.

#### 3.4.7 Irrigation Zone
##### It takes the control of the pipes related to one irrigation zone.
![Irrigation Zone](https://user-images.githubusercontent.com/72839552/133100735-80415533-05a4-4e51-8cad-9dbce53af31c.jpg)



#### 3.4.8 Actuator
#### 3.4.9 Sensor


----------------------------------------------


