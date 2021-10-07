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
 */

#ifndef SENSOR_MANUAL_TEST_H
#define SENSOR_MANUAL_TEST_H


/*Sensor Switch Class:
 * 1- Create a sensor attached to a GPIO as normal_open or normal_close.
 * 2- Read a value from GPIO.
 * 3- Create a sensor increase the _instances_number in sensor.
 * 4- Delete a sensor decrease the _instances_number in sensor.
 * 5- If the sensor is normal_open when the sensor is close is read as High.
 * 6- If the sensor is normal_close when the sensor is open is read as High.
 */

/**How to run test:
 * 1) Call function sensor_switch_test_init in init_cmain() in cmain.cpp
 * 2) Call function sensor_switch_test_run in cmain() in cmain.cpp
 * 3) Check the PA9 port and GND with a logic analyzer to see UART at 115200. The sensor instances are shown there.
 * 4) Check that connecting PG2 port to GND turn on the green led. PG2 is configured as a normal_open sensor and the led indicate when sensor is active.
 * 5) Check that connecting PG3 port to GND turn off the red led. PG3 is configured as a normal_close sensor and the led indicate when sensor is active.
 */
void sensor_switch_test_init();
void sensor_switch_test_run();

#endif //SENSOR_MANUAL_TEST_H
