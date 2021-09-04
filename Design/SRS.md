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

### 3.3 Use cases
### 3.4 Classes / Objects
