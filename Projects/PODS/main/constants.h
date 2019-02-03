////////////////////////////////////////////////////////////////////////////////
//                               CONSTANTS.H                                  //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// Use this header to store variables that are global for the entire project. //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
#include "driver/ledc.h"

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

static TaskHandle_t xGyger0, xGyger1, xGyger2, xGyger3, xGyger4, xGyger5, xGyger6;


/*********************
pins for gyger counter interupts
*********************/
static int gyger0_pin = 36;
static int gyger1_pin = 39;
static int gyger2_pin = 34;
static int gyger3_pin = 35;
static int gyger4_pin = 32;
static int gyger5_pin = 33;
static int gyger6_pin = 25;

/******************************
pins for the servos of the inoculation fluid
*******************************/
static int inoculation_servo0_pin = 26;
static int inoculation_servo1_pin = 27;
static int inoculation_servo2_pin = 14;
static int inoculation_servo3_pin = 13;
static int inoculation_servo4_pin = 9;
static int inoculation_servo5_pin = 10;
static int inoculation_servo6_pin = 0;
static int inoculation_servo7_pin = 23;

/***********************************
pins for servos that seal PODS units
**********************************/
static int lid_servo_pin0 = 22;
static int lid_servo_pin1 = 1;
static int lid_servo_pin2 = 3;
static int lid_servo_pin3 = 21;
static int lid_servo_pin4 = 19;
static int lid_servo_pin5 = 18;
static int lid_servo_pin6 = 5;

/************************************
variables that count the interupts from the gyger counters
**************************************/
static volatile int eCount0;
static volatile int eCount1;
static volatile int eCount2;
static volatile int eCount3;
static volatile int eCount4;
static volatile int eCount5;
static volatile int eCount6;

/******************
variables for gyger data
*******************/
static int cpm0, cpm1, cpm2, cpm3, cpm4, cpm5, cpm6;

#endif
