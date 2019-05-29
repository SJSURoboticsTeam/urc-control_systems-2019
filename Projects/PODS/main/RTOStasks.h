#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/event_groups.h"


#ifndef RTOS_TASKS_H_
#define RTOS_TASKS_H_

#ifdef _cplusplus
extern "C" {
#endif


extern "C" void vGygerTask(void *pvParameters);// reads from interupt queue and calculataed cpm

extern "C" void vLidTask(void *pvParameters);// allows mission control to toggle lid manually

extern "C" void vServoTask(void *pvParameters);//allows for specifing the angle of individual servos
#ifdef _cplusplus
}
#endif
#endif
