#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/event_groups.h"
#include "Servo.hpp"

extern "C" {
#include "Wifi.h"
#include "Structure.h"
}


#ifndef RTOS_TASKS_H_
#define RTOS_TASKS_H_

#ifdef _cplusplus
extern "C"{
#endif

extern "C" void vDriveTask(void *pvParameters);
extern "C" void vReadTask(void *pvParameters);
extern "C" void vWriteTask(void *pvParameters);
extern "C" void vBrakeTask(void *pvParameters);


#ifdef _cplusplus
}
#endif

#endif
