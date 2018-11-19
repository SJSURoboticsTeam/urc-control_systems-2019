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


static bool gyger0_start = false;
static bool gyger1_start = false;
static bool gyger2_start = false;
static bool gyger3_start = false;
static bool gyger4_start = false;
static bool gyger5_start = false;
static bool gyger6_start = false;

/*********************
pins for gyger counter interupts
*********************/
static int gyger0_pin = 4;
static int gyger1_pin = 5;
static int gyger2_pin = 6;
static int gyger3_pin = 7;
static int gyger4_pin = 8;
static int gyger5_pin = 9;
static int gyger6_pin = 10;

/******************************
pins for the servos of the inoculation fluid
*******************************/
static int inoculation_servo0_pin = 11;
static int inoculation_servo1_pin = 12;
static int inoculation_servo2_pin = 13;
static int inoculation_servo3_pin = 14;
static int inoculation_servo4_pin = 15;
static int inoculation_servo5_pin = 16;
static int inoculation_servo6_pin = 17;
static int inoculation_servo7_pin = 18;

/***********************************
pins for servos that seal PODS units
**********************************/
static int hatch_servo_pin0 = 19;
static int hatch_servo_pin1 = 20;
static int hatch_servo_pin2 = 21;
static int hatch_servo_pin3 = 22;
static int hatch_servo_pin4 = 23;
static int hatch_servo_pin5 = 24;
static int hatch_servo_pin6 = 25;

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
objects for storing temproary calculations from gygercounter
*******************/
struct GygerCounter
	{
		float cps;
		int cpm;
		int count;
		bool start;

	};


#endif
