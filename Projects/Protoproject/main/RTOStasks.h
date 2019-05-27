#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/event_groups.h"


#ifndef RTOS_TASKS_H_
#define RTOS_TASKS_H_

#ifdef _cplusplus
extern "C" {
#endif

extern "C" void vSayHelloTask(void *pvParameters);

/*
    Enables camera gimbal pitch movement to interpret manual inputs 
    from mission control in order to change the gimbal's direction.

*/

extern "C" void vPitchTask(void *pvParameters);

/*
    This task demonstrates how to read and write from EEPROM,
    which is non-voltaile memory that we can use to store data on the 
    ESP.
*/

extern "C" void vMPU6050Task(void *pvParameters);

/*
    This task reads IMU positional data from the MPU6050
*/

extern "C" void vCountTask(void *pvPArameters);

#ifdef _cplusplus
}
#endif
#endif
