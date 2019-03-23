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
    char mode[20]; // debug, manual, or arm control mode based on "debug", "manual", or "arm" keywords 
    int manual_move; // changes the position of the gimbal manual, up, center, or down based on "0", "1", "2", or "3" respectively
    double gimbal_position = 0; // current y axis value of the gimbal
};

typedef enum CommandMoveMode {
    CENTER = 0, 
    UP = 1, 
    DOWN = 2,
};

void initServer(AsyncWebServer* server, ParamsStruct* params);

bool initEEPROM();

int EEPROMCount(int addr);

void hello_world(char* name);

void initGimbal(); // Initialize the servo object of the camera pitch

void initCameraLens(); // Initialize the servo object of the DC Camera Lens

void readIMU(); // Interprets values from the IMU underneath the gimbal

void centerMovePitch(); // Rotates gimbal where the camera aligns all the way to the center

void upMovePitch(int position); // Rotates gimbal to shift the camera upwards

void downMovePitch(int position); // Rotates gimbal to shift the camera downwards

void sweepMovePitch(); // Test function to make sure the gimbal works

#ifdef _cplusplus
}
#endif

#endif

