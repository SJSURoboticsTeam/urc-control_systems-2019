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


struct ParamsStruct {
    char mode[10];
    int wheel_A_speed;
    int wheel_A_heading;
    int wheel_B_speed;
    int wheel_B_heading;
    int wheel_C_speed;
    int wheel_C_heading;
    bool brake;
};

enum DriveMode {
    CRAB,
    CAR,
    SPIN,
    DEBUG
};

void initServer(AsyncWebServer* server, ParamsStruct* params);

void initComponents();

/**
 * This function takes all of the necessary steps required to prep the drive
 * systems for use in driving the rover like a normal vehicle. This includes
 * choosing a direction to be the "front" of the rover, locking the wheels
 * necessary for such an activity, and other yet-to-be determined processes.
 * As of 10/5/18, this is a conceptual function, and has not yet been
 * implemented.
 */
void initDriveMode(uint32_t heading);

/**
 * This function takes all necessary steps required to prep the drive system
 * for use in rotating the rover in place. This process involves synchronizing
 * the wheels to rotate in unison, and to stop when they are each parallel to 
 * the rover's chassis. This will enable the rover to spin when mission control
 * gives the signal to move. As of 10/5/18, this is a conceptual function, and 
 * has not yet been implemented.
 */
void initRotateMode(bool direction);

/**
 * This function takes all of the necessary steps required to prep the drive
 * system for use in freely moving the rover in any given direction. This
 * involves calculating the correct rotation of the servos necessary for the 
 * rover to safely and accurately spin and turn at any given velocity. As of 
 * 10/5/18, this is a conceptual function, and has not yet been implemented.
 */
void initTranslateMode();

/**
 * This function determines which direction all the wheels should spin. As of
 * 10/5/18, this is a conceptual function and has not yet been implemented.
 */
void setDirectionAllWheels(bool direction);

/**This function determines which direction the stated wheel should spin. As of
 * 10/5/18, this is a conceptual function and has not yet been implemented.
 */
//void setDirection(uint32_t wheel, bool direction);

/**
 * This function sets the speed at which all wheels should rotate. As of
 * 10/5/18, this is a conceptual funtion and has not yet been implemented.
 */
void setSpeedAllWheels(double speed);

/**
 * This function sets the speed at which the stated wheel. should rotate. As of
 * 10/5/18, this is a conceptual function and has not yet been implemented.
 */
//void setSpeed(uint32_t wheel, uint32_t speed);

/**
 * This function determines the current heading of the rover based on raw GPS
 * data. As of 10/5/18, this is a conceptual function and has not yet been 
 * implemented.
 */
//char *getHeading(double gps_data);

/**
 * This function determines the actual rotation of the stated wheel based upon
 * potentiometer data. As of 10/5/18, this is a conceptual function and has not
 * yet been implemented.
 */
//bool getCurrentDirection(uint32_t wheel);

/**
 * Calculates the differential speeds for the front wheels during drive mode. As
 * of 10/30/18, this has yet to be defined.
 */

//void getDriveDifferentials(double turn_angle, double speed);


bool initEEPROM();

int EEPROMCount(int addr);

#ifdef _cplusplus
}
#endif

#endif