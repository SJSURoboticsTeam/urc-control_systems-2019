#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/event_groups.h"


#ifndef RTOS_TASKS_H_
#define RTOS_TASKS_H_

#ifdef _cplusplus
extern "C" {
#endif

			//Reads from Terminal (Misson Control),
			//and updates a target for the servo to
			//point toward
extern "C" void vUpdateTarget(void *pvPArameters);

			//Reads from an IMU to find where the
			//servo is currently pointing
extern "C" void vIMUTask(void *pvPArameters);

			//Handles adjusting the Servo to point 
			//to the intended target
extern "C" void vServoTask(void *pvParameters);


#ifdef _cplusplus
}
#endif
#endif
