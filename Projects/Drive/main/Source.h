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
    int mode;
    double AXIS_X;
    double AXIS_Y;
    double THROTTLE;
    bool button_0;
    bool wheel_A;
    bool wheel_B;
    bool wheel_C;
    double mast_position;
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

/**
 * Ths function initializes the motor and servo class objects responsible for
 * controlling the movement of the rover.
 */ 
void initComponents();

/**
 * This function sets the wheels facing the same direction the mas is pointed so
 * that the rover can be driven like a car.
 */
void initDriveMode(uint32_t heading);

/**
 * This function sets all of the wheels parallel to the chasis.
 */
void initSpinMode(bool direction);

/**
 * This function sets all of the steering servos to 50% of their total range.
 */
void initCrabMode();

/**
 * This function determines which direction all the wheels should spin. And sets
 * the corresponding pins to the appropriate level. 
 */
void setDirectionAllWheels(bool direction);

/*
 * This function determines which direction the stated wheel should spin and 
 * sets the corresponding pin to that level;
 */
void setDirection(uint32_t wheel, bool direction);

/**
 * This function sets the speed at which all wheels should rotate. It is a
 * percentage of the maximum speed the hubmotor can rotate.
 */
void setSpeedAllWheels(double speed);

/**
 * This function sets the speed at which the stated wheel should rotate. Speed 
 * the percentage of the maximum speed the hubmotor can spin at.
 */
void setSpeed(uint32_t wheel, uint32_t speed);

/**
 * This function sets the heading of the specified wheel to the specified 
 * percent of the range of the servo..
 */
void setHeading(uint32_t wheel, double percentage);

/**
 * This function enables/disabes the brakes pin according to the boolean sent.
 */
void applyBrakes(bool signal);

/**
 * Modified arduino mapping function for floats/doubles
 */
double fmap(double x, double in_min, double in_max, double out_min, double out_max);

/**
 * Processes raw joystic data to drive mode steering angles
 */
double driveModeMapping(double x, double y);

#ifdef _cplusplus
}
#endif

#endif