////////////////////////////////////////////////////////////////////////////////
//                               CONSTANTS.H                                  //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// Use this header to store variables that are global for the entire project. //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
#include "driver/ledc.h"
#include <string>
#include "freertos/FreeRTOS.h"
#include "Servo_Control.hpp"


#ifndef CONSTANTS_H_
#define CONSTANTS_H_

extern QueueHandle_t xQueueISR, xQueueTerminateTask,
		 xQueueServoID, xQueueServoAngle, xQueueServoType;//, xQueueLid;//, xTaskQueue; 

extern TaskHandle_t task0, task1, task2, task3, task4, task5, task6;


extern  Servo lid_servo0,
				lid_servo1,
				lid_servo2,
				lid_servo3,
				lid_servo4,
				lid_servo5,
				lid_servo6,
				inoculation_servo0,
				inoculation_servo1,
				inoculation_servo2,
				inoculation_servo3,
				inoculation_servo4,
				inoculation_servo5,
				inoculation_servo6;

static const uint32_t servo_frequency = 50; //hz
static const float servo_min = 2.5;//%
static const float servo_max = 12;//%
static const int open_angle = 75;
static const int close_angle = -60;
static const int up = 90;
static const int down = -70;


/******************
variables for gyger data
	-stores emission data in counts per minute
*******************/
//static	int cpm0, cpm1, cpm2, cpm3, cpm4, cpm5, cpm6;
static String data_string_0, data_string_1, data_string_2,data_string_3,data_string_4,
		data_string_5, data_string_6, test_servo;

static int test_angle, test_id, test_servo_pin;

/*********************
pins for gyger counter interupts
*********************/
static const int gyger0_pin = 2;
static const int gyger1_pin = 32;
static const int gyger2_pin = 33;
static const int gyger3_pin = 39;
static const int gyger4_pin = 36;
static const int gyger5_pin = 35;
static const int gyger6_pin = 34;

/******************************
pins for the servos of the inoculation fluid
*******************************/
static const int inoculation_servo0_pin = 12;
static const int inoculation_servo1_pin = 5;
static const int inoculation_servo2_pin = 16;
static const int inoculation_servo3_pin = 18;
static const int inoculation_servo4_pin = 23;
static const int inoculation_servo5_pin = 26;
static const int inoculation_servo6_pin = 21;
//static const int inoculation_servo7_pin = ;

/***********************************
pins for servos that seal PODS units
**********************************/
static const int lid_servo_pin0 = 14;
static const int lid_servo_pin1 = 17;
static const int lid_servo_pin2 = 4;
static const int lid_servo_pin3 = 19;
static const int lid_servo_pin4 = 22;
static const int lid_servo_pin5 = 27;
static const int lid_servo_pin6 = 25;

/*
extern Servo inoculation_servo0(inoculation_servo0_pin,0,0, 
        servo_frequency, servo_max, servo_min);

extern 	Servo inoculation_servo1(inoculation_servo1_pin,1,0, 
        servo_frequency, servo_max, servo_min);

extern 	Servo inoculation_servo2(inoculation_servo2_pin,2,0, 
        servo_frequency, servo_max, servo_min);

extern 	Servo inoculation_servo3(inoculation_servo3_pin,3,0, 
        servo_frequency, servo_max, servo_min);

extern 	Servo inoculation_servo4(inoculation_servo4_pin,4,0,
        servo_frequency, servo_max, servo_min);

extern 	Servo inoculation_servo5(inoculation_servo5_pin,5,0, 
        servo_frequency, servo_max, servo_min);

extern 	Servo inoculation_servo6(inoculation_servo6_pin,6,0, 
        servo_frequency, servo_max, servo_min);


extern 	Servo lid_servo0(lid_servo_pin0,7,0, 
        servo_frequency, servo_max, servo_min);

extern 	Servo lid_servo1(lid_servo_pin1,0,1, 
        servo_frequency, servo_max, servo_min);

extern 	Servo lid_servo2(lid_servo_pin2,1,1, 
        servo_frequency, servo_max, servo_min);

extern 	Servo lid_servo3(lid_servo_pin3,2,1, 
        servo_frequency, servo_max, servo_min);

extern 	Servo lid_servo4(lid_servo_pin4,3,1, 
        servo_frequency, servo_max, servo_min);

extern 	Servo lid_servo5(lid_servo_pin5,4,1, 
        servo_frequency, servo_max, servo_min);

 extern 	Servo lid_servo6(lid_servo_pin6,5,1, 
        servo_frequency, servo_max, servo_min);
*/
#endif
