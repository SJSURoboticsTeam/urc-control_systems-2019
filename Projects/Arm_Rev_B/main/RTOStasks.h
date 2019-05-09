#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/event_groups.h"

#ifndef RTOS_TASKS_H_
#define RTOS_TASKS_H_

#ifdef _cplusplus
extern "C" {
#endif

extern "C" void vCountTask(void *pvParameters);

extern "C" void vRotundaTask(void *pvParameters);

extern "C" void vElbowTask(void *pvParameters);

extern "C" void vShoulderTask(void *pvParameters);

extern "C" void vDiffGearboxTask(void *pvParameters);

extern "C" void vClawTask(void *pvPArameters);

extern "C" void vI2CScannerTask(void *pvParameters);

extern "C" void vReadAxisTask(void *pvParameters);

extern "C" void vMPU6050Task(void *pvParameters);

#ifdef _cplusplus
}
#endif
#endif
