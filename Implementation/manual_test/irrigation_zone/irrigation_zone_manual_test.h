/**irrigation_zone_manual_test.h
 * This file contain the manual tests for the Irrigation_zone class.
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

#ifndef IRRIGATION_ZONE_MANUAL_TEST
#define IRRIGATION_ZONE_MANUAL_TEST

/**Irrigation zone class test description
 *
 * The manual test will check:
 * 1) To activate a zone when is the unique zone and the pump is off.
 * 		Prerequisites:
 * 			- GPIO is assigned to actuator electro-valve.
 * 			- GPIO is assigned to actuator pump.
 * 			- Actuator electro-valve and pump is assigned to irrigation zone.
 * 		Post-condition:
 * 			- GPIO of actuator electro-valve is HIGH.
 * 			- GPIO of actuator pump is HIGH.
 *
 * 2) To deactivate a zone when the zone is unique and the pump is on.
 * 		Prerequisites:
 * 			- Test 1) has been run.
 *		Post-condition:
 *			- GPIO of actuator electro-valve is LOW.
 *			- GPIO of actuator pump is LOW.
 *
 * 3) To deactivate a zone when there are two zones activated.
 * 		Prerequisites:
 * 			- Two irrigation zones were properly created.
 * 		Post-condition:
 * 		 	- GPIO of actuator electro-valve is LOW.
 * 		 	- GPIO of actuator pump is HIGH.
 *
 * */

/**
 * Test functions:
 * irrigation_zone_manual_test_run() runs the tests in order. With a delay of 1 second between each test.
 * */
void irrigation_zone_manual_test_init();
void irrigation_zone_manual_test_run();

#endif //IRRIGATION_ZONE_MANUAL_TEST
