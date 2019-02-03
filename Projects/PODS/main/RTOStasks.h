#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/event_groups.h"


#ifndef RTOS_TASKS_H_
#define RTOS_TASKS_H_

#ifdef _cplusplus
extern "C" {
#endif


extern "C" void vGygerTask(void *pvParameters);


extern "C" void vTest(void *pvParameters);


#ifdef _cplusplus
}
#endif
#endif
