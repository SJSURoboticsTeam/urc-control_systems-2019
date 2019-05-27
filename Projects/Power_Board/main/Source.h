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
    uint32_t ch1_isns;
    uint32_t ch2_isns;
    uint32_t ch3_isns;
    uint32_t ch4_isns;
};

void initServer(AsyncWebServer* server, ParamsStruct* params);

uint8_t i2c_scanner();

#endif
