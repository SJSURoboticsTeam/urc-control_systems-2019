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
    float yaw[4];
    float pitch[4];
    float roll[4];
};

void initServer(AsyncWebServer* server, ParamsStruct* params);

uint8_t i2c_scanner();

bool i2cScanAndInit(uint8_t imu_address);

void initIMU(uint8_t IMU_ADDRESS, uint8_t MODE);

void writeByte(uint8_t IMU_ADDRESS, uint8_t REGISTER_ADDRESS, uint8_t VALUE);

uint8_t readByte(uint8_t IMU_ADDRESS, uint8_t REGISTER_ADDRESS);

int16_t getAxis(uint8_t IMU_ADDRESS, uint8_t REGISTER_ADDRESS);

double calculatePitch(double xAxis, double yAxis, double zAxis);

double calculateRoll(double xAxis, double yAxis, double zAxis);
#ifdef _cplusplus
}
#endif

#endif

