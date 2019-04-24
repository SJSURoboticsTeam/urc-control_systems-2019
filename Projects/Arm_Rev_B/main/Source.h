#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/event_groups.h"
#include "constants.h"
#include <stdlib.h>
#include <stdio.h>

#ifndef PROTOPROJECT_H_
#define PROTOPROJECT_H_

#ifdef _cplusplus
extern "C" {
#endif

struct ParamsStruct {
	double RotundaTarget = -180.0; //range of -180 to 180
	double ElbowTarget = kElbowStartPos;
	double ShoulderDuration_ms = 0;
	double WristPitch = 90;
	double WristRoll = 90;   
	SemaphoreHandle_t xWristPitchSemaphore;
	SemaphoreHandle_t xWristRollSemaphore;

	//Raul's Stuff
	int current_direction = 0;
   //PWM Signal between 0-50 to limit voltage up to 12V
   uint32_t actuator_speed = 0;
   uint32_t update_speed = 0;
};

void initServer(AsyncWebServer* server, ParamsStruct* params);

bool initEEPROM();

int EEPROMCount(int addr);

void initClaw();

bool openClaw();

bool closeClaw();

// Current = (Target * α) + (Current * (1 - α))
double ExpMovingAvg(double Current, double Target, double Alpha);

double fmap(double x, double in_min, double in_max, double out_min, double out_max);

#ifdef _cplusplus
}
#endif

#endif

