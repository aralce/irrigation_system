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
- Manage electro valves and a pump.
- Warning about failures.
  ![system](https://user-images.githubusercontent.com/72839552/132056941-11d8951f-46ed-4417-aa22-e17dcdcfbfd4.PNG)  
  
## 3. SPECIFIC REQUERIMENTS
#### 3.1 External interfaces
#### 3.1.1 User interfaces
#### 3.1.2 Hardware interfaces
#### 3.1.3 Software interfaces
#### 3.1.4 Communication protocols and interfaces

### 3.2 Functional requeriments
##### 3.2.1 SENSORS-FR1
###### 3.2.1.1 *Requeriment:* The system have a maximun of 8 presion sensors.
##### 3.2.2 SENSORS-FR2
###### 3.2.2.1 *Requeriment:* The posible configuration for each sensor is: NO-SENSOR, SWITCH, 0-20mA
##### 3.2.3 SENSORS-FR3
###### 3.2.3.1 *Requeriment:* Each sensor can be configured independently from others.
##### 3.2.4 ELECTRO-VALVE-FR1
###### 3.2.4.1 *Requeriment:* The system have a maximun of 8 electro-valves
##### 3.2.5 ELECTRO-VALVE-FR2
###### 3.2.5.1 *Requeriment:* The user can indicate how many liters per minute each valve pulls
##### 3.2.6 ELECTRO-VALVE-FR3
###### 3.2.6.1 *Requeriment:* Only one electro-valve can be open each time. 
###### 3.2.6.2 *Description:* The sequense of transition is: valve_1 is open, valve_2 is opened, valve_1 is closed.
##### 3.2.7 PUMP-FR1
###### 3.2.7.1 *Requeriment:* The pump is activated only if there is an electro-valve open.
##### 3.2.8 PUMP-FR2
###### 3.2.8.1 *Requeriment:* Before close all electro-valves, the pump is desactivated.
##### 3.2.9 POWER_SYSTEM-FR1
###### 3.2.9.1 *Requeriment:* If voltage <= 0,7*220Vac then the primary power system is desactivated and secondary power system is activated.
##### 3.2.10 POWER_SYSTEM-FR2
###### 3.2.10.1 *Requeriment:* If voltage >= 0,8*220Vac then the secondary power system is activated and primary power system is activated.
##### 3.2.11 POWER_SYSTEM-FR3
###### 3.2.11.1 *Requeriment:* If secondary power system is active then the system is low consumption.
###### 3.2.11.2 *Description:* In low consumption only alarm system and communication system are active. Secondary power system can be active for a week.
##### 3.2.12 CONTROL_UNIT-FR1
###### 3.2.12.1 *Requeriment:* For each electro-valve the system can irrigate the irrigation zone with a weekly calendar.
###### 3.2.12.2 *Description:* On the calendar the user can select the day and starting hour. The time of irrigation is expresed as minutes or as liters.
##### 3.2.13 CONTROL_UNIT-FR2
###### 3.2.12.1 *Requeriment:* The user can manually irrigate an irrigation zone ONLY if there is no connection with server. The manual irrigation has priority over calendar irrigation.

### 3.3 Use cases
### 3.4 Classes / Objects
