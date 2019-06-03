#ifndef RTOS_TASKS_H_
#define RTOS_TASKS_H_

#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/event_groups.h"

#ifdef _cplusplus
extern "C" {
#endif

extern "C" void vGPSTask(void *pvParameters);

#ifdef _cplusplus
}
#endif
#endif