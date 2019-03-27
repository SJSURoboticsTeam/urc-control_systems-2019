#ifndef RTOS_TASKS_H_
#define RTOS_TASKS_H_

#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/event_groups.h"

#ifdef _cplusplus
extern "C" {
#endif


/*extern "C" void vModeTaskHandler(void *pvParameters);

extern "C" void vDebugTask(void *pvPArameters);

extern "C" void vDriveTask(void *pvParameters);

extern "C" void vCrabTask(void *pvParameters);

extern "C" void vSpinTask(void *pvParameters);*/

extern "C" void vMoveTask(void *pvParameters);

#ifdef _cplusplus
}
#endif
#endif