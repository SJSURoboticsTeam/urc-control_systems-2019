#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef PROTOPROJECT_H_
#define PROTOPROJECT_H_

#ifdef _cplusplus
extern "C" {
#endif

struct ParamsStruct {
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

//void stopClaw();

#ifdef _cplusplus
}
#endif

#endif

