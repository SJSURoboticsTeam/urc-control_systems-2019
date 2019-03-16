#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/event_groups.h"
#include <stdlib.h>
#include <stdio.h>

#ifndef PROTOPROJECT_H_
#define PROTOPROJECT_H_

#ifdef _cplusplus
extern "C" {
#endif

constexpr int kRotundaPin = 18;		//Servo 1
constexpr int kRotundaPosMin = 0;
constexpr int kRotundaPosmax = 3600;
constexpr double kRotundaStartPos = 1800;
constexpr double kRotundaStartDuty = 50.0;
constexpr double kRotundaFreq = 50;
constexpr double kRotundaPWMMin = 5;
constexpr double kRotundaPWMMax = 10;

constexpr int kElbowPin = 25;		//Servo 2
constexpr int kElbowLimitMin = 0;
constexpr int kElbowLimitMax = 300;
constexpr double kElbowRange = 300;
constexpr double kElbowStartPos = 150;
constexpr double kElbowFreq = 50;
constexpr double kElbowPWMMin = 2.5;
constexpr double kElbowPWMMax = 12.5;


constexpr uint32_t kMotorFreq = 5000;
constexpr uint32_t kShoulderSigPin = 33;
constexpr uint32_t kShoulderDirPin = 32;
constexpr uint32_t kShoulderEnablePWMMin = 0;	//percentage
constexpr uint32_t kShoulderEnablePWMMax = 50;	//percentage

constexpr uint32_t kWristLeftSigPin = 4;
constexpr uint32_t kWristLeftDirPin = 16;

constexpr uint32_t kWristRightSigPin = 17;
constexpr uint32_t kWristRightDirPin = 5;	//port 2


/* PINOUTS:
	Servo 1: GPIO 6
	Servo 2: GPIO 7

	i2c:
		SDA: GPIO 23
		SCL: GPIO 21

	Motors:
		Motor Driver 1:	DIFF GEARBOX
			VPROPI: GPIO 39
			Mode 2: GPIO 27
			Phase:	GPIO 16
			Enable:	GPIO 4

		Motor Driver 2:	DIFF GEARBOX
			VPROPI: GPIO 36
			Mode 2: GPIO 1
			Phase:	GPIO 5
			Enable:	GPIO 17

		Motor Driver 3:
			VPROPI: GPIO 34
			Mode 2: GPIO 3
			Phase:	GPIO 0
			Enable:	GPIO 2
*/


struct ParamsStruct {
    // char name[40];
	double RotundaTarget = -180.0; //range of -180 to 180
	double ElbowTarget = kElbowStartPos;
	double ShoudlerDuration_ms = 0;
	int Wrist_Dimension = 0;;
	double Wrist_Delta = 0;   
	SemaphoreHandle_t xWristSemaphore;

	//Raul's Stuff
	int current_direction = 0;
   //PWM Signal between 0-50 to limit voltage up to 12V
   uint32_t actuator_speed = 0;
   uint32_t update_speed = 0;
};

void initServer(AsyncWebServer* server, ParamsStruct* params);

bool initEEPROM();

int EEPROMCount(int addr);

void initClaw();

bool openClaw();

bool closeClaw();

/*
	Rotunda:
		*3600 degrees posible, but MS wants to simplify the XHR to send a 0-360 degree number
		*Since XHR will send 0-360, need to set current 3600 position to nearest 360 degree placement
			* current_pos_360 = current_pos_3600 % 360; //to get the current position in a 360 limit?
		*Control Specifcations:
			*1ms Pulse Mode: Pulse width of 1-2ms		//Most likely 1-2ms Mode
			*2ms Pulse Mode: Pulse width of 0.5-2.5ms
				***Need to figure out which mode is being assembled.
			*Angle Limit: 3600 degrees
	Elbow Servo:
		*Waiting on mechanical to find final range of the Arm,
		*Control Specifcations:
			*1ms Pulse Mode: Pulse width of 1-2ms		//Most likely 1-2ms Mode
			*2ms Pulse Mode: Pulse width of 0.5-2.5ms 
				***Need to figure out which mode is being assembled.
			*Angle Limit: 3600 degrees
	Shoulder Motor:	
		*For time being, Motor PWMs should not be above 50% duty cycle due to electrical errors
		*Waiting on mechanical to find final range of the Arm,
		*For time being 0-50% Duty
		*Control Specifcations:

	Wrist Motors:
		*For time being, Motor PWMs should not be above 50% due to electrical errors
		*Waiting on mechanical to find final range of the Arm,
		*For time being 0-50% Duty
		*Control Specifcations:

	Linear Actuator
		*Haven't looked into this yet

*/

#ifdef _cplusplus
}
#endif

#endif

