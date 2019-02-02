#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <stdlib.h>
#include <stdio.h>
#include "Servo_Control.hpp"
#include "constants.h"

#ifndef PROTOPROJECT_H_
#define PROTOPROJECT_H_

#ifdef _cplusplus
extern "C" {
#endif

struct ParamsStruct {
    char name[20]; // test parameter
    int mode; // arm or manual control mode
    double pitch_value; // y axis value of the gimbal
};

void initServer(AsyncWebServer* server, ParamsStruct* params);

bool initEEPROM();

int EEPROMCount(int addr);

void hello_world(char* name);

void initGimbal();

void manualMovePitch(double percent);

void sweepMovePitch();

#ifdef _cplusplus
}
#endif

#endif

