/**Actuator_manual_test.h
 * This file contain the manual tests for the actuator class.
 *
 * FAQ:
 * How do I configure project to run test?
 * 1) You must create a project in stm32CubeIde with MXCube. Select the STM32F429 discovery evaluation kit.
 * 2) You may use the default configuration for this board.
 * 3) Change the name of "main.c" to "main.cpp"
 * 4) Copy manual_test folder and classes folder to the project.
 * 5) You should check that manual_test folder and classes folder is not excluded from build.
 * 5) Copy cmain.cpp to core/Src and cmain.h to core/Inc.
 * 6) Copy the function test to run in cmain() in cmain.cpp and follow the steps described on the selected test.
 * 7) Add the init_cmain() in the initialization part of the main.
 * 8) Add the cmain() in the main task function.
 * */

#ifndef IRRIGATION_MANAGER_H
#define IRRIGATION_MANAGER_H

/**
 * Irrigation Manager Class tests:
 * -Test of irrigate function. The system need to irrigate and stop to irrigate an irrigation zone.
 * -Test of zones healthy. The system need to check the system is healthy.
 * -Add event and wait for the zone activation.
 *
 *Description:
 * There are essentially 2 tests: 1- Manual irrigation and zones healthy   2- Add event and event execution.
 * Each of these tests has an initialization and a run function.
 *
 * Initialization:
 * 1) Instantiate irrigation zones
 * 2) Instantiate calendar routines
 * 3) Instantiate irrigation zone manager.
 * 4) Add event.
 *
 * Run stage(1- Manual irrigation and zones healthy):
 * 1) Irrigate and check the state of PUMP and electrovalve by UART. //The PUMP and irrigation_zone outpus can be seen on io_map.cpp
 * 2) Check the healthy of irrigation zone by UART.
 * 3) Every 2 seconds the zone is being deactivated and activated.
 *
 * Run stage(2- Add event and event execution)
 * 1) Process events.
 * 2) If the event is active the zone is active
 * 3) If the event is not active the zone neither.
 * */

//The initialization only has a sensor to test the healthy of system.
void irrigation_manager_manual_test_init();
void irrigation_manager_manual_test_run_1();
void irrigation_manager_manual_test_run_2();

#endif //IRRIGATION_MANAGER_H
