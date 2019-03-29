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

extern "C" void vCountTask(void *pvPArameters);

#ifdef _cplusplus
}
#endif
#endif
