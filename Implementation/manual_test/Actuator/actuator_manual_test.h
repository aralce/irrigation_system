/**Sensor_manual_test.h
 * This file contain the manual tests for the sensor class.
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

#ifndef ACTUATOR_MANUAL_TEST_H
#define ACTUATOR_MANUAL_TEST_H

/**Actuator Class test description.
 * Initialization:
 * 1) Add a sensor to the actuator(PG2).
 * 2) Attach a gpio to actuator(LED4).
 * Run stage:
 * 3) Show the actuator state on UART(PA9).
 * 4) Read the sensor state on LED3.
 * 5) Wait one second
 * 6) Activate the actuator
 * 7) Show the actuator state on UART (PA9).
 * 8) Wait one second.
 * 9) Deactivate the actuator.
 * */
/**How to run test
 * 1) Call one of actuator_manual_test_init functions in init_cmain() in cmain.cpp
 * 2) Call function actuator_manual_test_run() in cmain() in cmain.cpp
 * 3) Check the PA9 port and GND with a logic analyzer to see UART at 115200.
 * 4) Check that connecting PG2 port to GND turn on the green led. PG2 is configured as a normal_open sensor and the led indicate when sensor is active.
 * 5) Check that actuator is active with the LED4 is ON.
 */
void actuator_manual_test_init_with_sensor();
void actuator_manual_test_init_without_sensor();
void actuator_manual_test_run();


#endif //ACTUATOR_MANUAL_TEST_H
