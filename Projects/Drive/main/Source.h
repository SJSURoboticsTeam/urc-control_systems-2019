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
    char mode[10];
    int wheel_1_speed;
    int wheel_1_heading;
    int wheel_2_speed;
    int wheel_2_heading;
    int wheel_3_speed;
    int wheel_3_heading;
    bool brake;
};

enum DriveMode {
    CRAB,
    CAR,
    SPIN,
    DEBUG
};

void initServer(AsyncWebServer* server, ParamsStruct* params);

bool initEEPROM();

int EEPROMCount(int addr);

#ifdef _cplusplus
}
#endif

#endif