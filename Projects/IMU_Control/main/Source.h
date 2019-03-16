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
    char name[40];
};

void initServer(AsyncWebServer* server, ParamsStruct* params);

uint8_t i2c_scanner();

void initIMU(uint8_t IMU_ADDRESS);

void writeByte(uint8_t IMU_ADDRESS, uint8_t REGISTER_ADDRESS, uint8_t VALUE);

uint8_t readByte(uint8_t IMU_ADDRESS, uint8_t REGISTER_ADDRESS);

uint16_t readBytes(uint8_t IMU_ADDRESS, uint8_t REGISTER_ADDRESS);

int16_t getAxis(uint8_t IMU_ADDRESS, uint8_t REGISTER_ADDRESS);

#ifdef _cplusplus
}
#endif

#endif

