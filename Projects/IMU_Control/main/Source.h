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
    int16_t yaw[4];
    int16_t pitch[4];
    int16_t roll[4];
};

void initServer(AsyncWebServer* server, ParamsStruct* params);

uint8_t i2c_scanner();

void initIMU(uint8_t IMU_ADDRESS);

void writeByte(uint8_t IMU_ADDRESS, uint8_t REGISTER_ADDRESS, uint8_t VALUE);

uint8_t readByte(uint8_t IMU_ADDRESS, uint8_t REGISTER_ADDRESS);

uint16_t readBytes(uint8_t IMU_ADDRESS, uint8_t REGISTER_ADDRESS);

int16_t getAxis(uint8_t IMU_ADDRESS, uint8_t REGISTER_ADDRESS);

int16_t convertEuler(int16_t euler_angle_reading, int16_t min_angle_reading, int16_t max_angle_reading, int16_t min_output_value, int16_t max_output_value);
#ifdef _cplusplus
}
#endif

#endif

