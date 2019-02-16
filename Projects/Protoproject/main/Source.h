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
    int command_move; // changes the position of the gimbal manual, up, center, or down based on "0", "1", "2", or "3" respectively
    double manual_move = 0; // manually changes the position of the gimbal based on a custom value between 37 and 63 position percent
    double pitch_position = 0; // current y axis value of the gimbal
};

enum CommandMoveMode {
    MANUAL, UP, CENTER, DOWN
};

void initServer(AsyncWebServer* server, ParamsStruct* params);

bool initEEPROM();

int EEPROMCount(int addr);

void hello_world(char* name);

void initGimbal(); // Initialize the servo object of the camera pitch

void readIMU(); // Interprets values from the IMU underneath the gimbal

void centerMovePitch(); // Rotates gimbal to the center

void upMovePitch(); // Rotates gimbal to face the camera upwards

void downMovePitch(); // Rotates gimbal to face the camera downwards

void manualMovePitch(double percentage); // Interprets duty cycle percentages in order manually rotate the camera pitch

void sweepMovePitch(); // Test function to make sure the gimbal works

#ifdef _cplusplus
}
#endif

#endif

