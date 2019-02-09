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

extern QueueHandle_t xQueueISR;//, xTaskQueue; 


/******************
variables for gyger data

	stores emission data in counts per minute
*******************/
static	int cpm0, cpm1, cpm2, cpm3, cpm4, cpm5, cpm6;


/*********************
pins for gyger counter interupts
*********************/
static const int gyger0_pin = 23;
static const int gyger1_pin = 22;
static const int gyger2_pin = 34;
static const int gyger3_pin = 35;
static const int gyger4_pin = 32;
static const int gyger5_pin = 33;
static const int gyger6_pin = 25;

/******************************
pins for the servos of the inoculation fluid
*******************************/
static const int inoculation_servo0_pin = 26;
static const int inoculation_servo1_pin = 27;
static const int inoculation_servo2_pin = 14;
static const int inoculation_servo3_pin = 13;
static const int inoculation_servo4_pin = 9;
static const int inoculation_servo5_pin = 10;
static const int inoculation_servo6_pin = 0;
//static const int inoculation_servo7_pin = ;

/***********************************
pins for servos that seal PODS units
**********************************/
static const int lid_servo_pin0 = 39;
static const int lid_servo_pin1 = 1;
static const int lid_servo_pin2 = 3;
static const int lid_servo_pin3 = 21;
static const int lid_servo_pin4 = 19;
static const int lid_servo_pin5 = 18;
static const int lid_servo_pin6 = 5;



#endif
