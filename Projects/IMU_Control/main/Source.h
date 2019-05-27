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
    int32_t yaw[4];
    int32_t pitch[4];
    int32_t roll[4];
};

void initServer(AsyncWebServer* server, ParamsStruct* params);

//BNO055 Functions

uint8_t i2c_scanner();

bool i2cScanAndInit(uint8_t imu_address);

void initIMU(uint8_t IMU_ADDRESS, uint8_t MODE);

void writeByte(uint8_t IMU_ADDRESS, uint8_t REGISTER_ADDRESS, uint8_t VALUE);

uint8_t readByte(uint8_t IMU_ADDRESS, uint8_t REGISTER_ADDRESS);

int16_t getAxis(uint8_t IMU_ADDRESS, uint8_t REGISTER_ADDRESS);

//MPU6050 Functions

int32_t calculatePitch(int32_t accel_x, int32_t accel_y, int32_t accel_z);

int32_t calculateRoll(int32_t accel_x, int32_t accel_y, int32_t accel_z);

imu::Vector<3> scanMPU6050(uint8_t IMU_ADDRESS);

imu::Vector<3> scanAccel(uint8_t IMU_ADDRESS);

imu::Vector<3> scanGyro(uint8_t IMU_ADDRESS);

void initMPU6050(uint8_t IMU_ADDRESS);

#endif
