#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/event_groups.h"
extern "C" {
#include "Wifi.h"
#include "Structure.h"
}

#include "Source.h"
// #include "Arm_I2C.h"
#include "common_init.h"
#include "IMU.h"
#include "Magnet_Encoder.h"
#include "Servo.h"

#ifndef RTOS_TASKS_H_
#define RTOS_TASKS_H_

#ifdef _cplusplus
extern "C"{
#endif

/*extern "C"*/ void vReadDataTask(void *pvParameters);

/*extern "C"*/ void vSendDataTask(void *pvParameters);

void vI2CTask(void *pvParameters);

// Servo Tasks

void vRotundaTask(void *pvParameters);

void vElbowTask(void *pvParameters);

void vCamTask(void *pvParameters);

void vIMUTask(void *pvParameters);

void vBIMUTask(void *pvParameters);

// Motor Tasks

void vShoulderTask(void *pvParameters);

void vWristPitchTask(void *pvParameters);

void vWristRotationTask(void *pvParameters);

void vClawTask(void *pvParameters);

// Testing Tasks
void vMimicFilming(void *pvParameters);

void vMotorTask(void *pvParameters);

void vServoTask(void *pvParameters);

void vMagRawTest(void *pvParameters);

void vImuIdRead(void *pvParameters);

void vServoTestTask(void *pvParameters);

void vMotorTestTask(void *pvParameters);

void vQuadEncTestTask(void *pvParameters);

void vAdcTask(void *pvParameters);

#ifdef _cplusplus
}
#endif

#endif 
