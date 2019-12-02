#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <stdlib.h>
#include <stdio.h>
#include "Adafruit_BNO055.h"

#ifndef PROTOPROJECT_H_
#define PROTOPROJECT_H_

#ifdef _cplusplus
extern "C" {
#endif

struct ParamsStruct {
    char name[40];
    uint32_t fpi_pin;
};

void initServer(AsyncWebServer* server, ParamsStruct* params);

#endif
