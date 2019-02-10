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

/*
 * mode: indicates the functionality of the rover, should only hold values 0-3
 *     0: Debug Mode
 *	   1: Crab Mode
 *	   2: Spin Mode
 *	   3: Drive Mode
 *
 * Speeds are to be recieved in values ranging from -100 to 100 with double 
 * floating point precision.
 * 
 * Headings are to be recieved as follows:
 *     Debug: 0% - 100% to represent min to max rotation
 *     Crab:  heading_A: raw input from the joystick's AXIS 1
 *            heading_B: raw input from the joystick's AXIS 0
 *            heading_C: the current rotunda position from 0 - 360 degrees
 *     Spin:  heading_A: boolean
 *     Drive: heading_A: inverse raw input from the joystick's AXIS 0
 *            heading_B: 0, 1, 2, the side that the mast is facing
 * 
 * Brake is a boolean.
 */
struct ParamsStruct {
    int mode;
    double speed_A;
    double heading_A;
    double speed_B;
    double heading_B;
    double speed_C;
    double heading_C;
    bool brake;
};

enum DriveMode {
    DEBUG,
    CRAB,
    SPIN,
    DRIVE
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
void initSpinMode(bool direction);

/**
 * This function takes all of the necessary steps required to prep the drive
 * system for use in freely moving the rover in any given direction. This
 * involves calculating the correct rotation of the servos necessary for the 
 * rover to safely and accurately spin and turn at any given velocity. As of 
 * 10/5/18, this is a conceptual function, and has not yet been implemented.
 */
void initCrabMode();

/**
 * This function determines which direction all the wheels should spin. 
 */
void setDirectionAllWheels(bool direction);

/**This function determines which direction the stated wheel should spin. 
 */
void setDirection(uint32_t wheel, bool direction);

/**
 * This function sets the speed at which all wheels should rotate.
 */
void setSpeedAllWheels(double speed);

/**
 * This function sets the speed at which the stated wheel should rotate.
 */
void setSpeed(uint32_t wheel, uint32_t speed);

/**
 * This function sets the heading of the specified wheel.
 */
void setHeading(uint32_t wheel, double percentage);

/**
 * This function Applies PWM to all the brake pins.
 */
void applyBrakes(bool signal);

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