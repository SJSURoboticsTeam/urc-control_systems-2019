#ifndef SOURCE_H_
#define SOURCE_H_

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <stdlib.h>
#include <stdio.h>
#include "Servo_Control.hpp"
#include "constants.h"


#ifdef _cplusplus
extern "C" {
#endif

/**
 * mode: indicates the functionality of the rover, should only hold values 0-3
 *     0: Debug Mode
 *     1: Crab Mode
 *     2: Spin Mode
 *     3: Drive Mode
 *
 * AXIS_X, AXIS_Y, THROTTLE, and button_0 correspond to the raw inputs from the
 * joystick (AXIS_0, AXIS_1, AXIS_3 and button_0 respectively on Windows/Linux
 * machines).
 * 
 * The wheel booleans are used to test individual or multiple wheels in Debug
 * mode, or refer to the rear wheel in Drive mode.
 *
 * mast_position refers to the angle the mast is pointing relative to the
 * whel_A - wheel_B plane in degrees from -180 to 180.
 */
struct ParamsStruct {
    char* longitude;
    char* latitude;
};

enum DriveMode {
    DEBUG,
    CRAB,
    SPIN,
    DRIVE
};

/**
 * See Source.cpp comments for explaination of functionality.
 */
void initServer(AsyncWebServer* server, ParamsStruct* params);


#ifdef _cplusplus
}
#endif

#endif