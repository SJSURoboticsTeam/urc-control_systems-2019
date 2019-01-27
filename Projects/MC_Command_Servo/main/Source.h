#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef PROTOPROJECT_H_
#define PROTOPROJECT_H_

#ifdef _cplusplus
extern "C" {
#endif


constexpr int rotundaPosMin = 0;
constexpr int rotundaPosmax = 3600;

constexpr int ElbowPosMin = 0;
constexpr int ElbowPosmax = 90;

constexpr int shoulderPosMin = 0;
constexpr int shoulderPosmax = 90;


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

struct ParamsStruct {
    // char name[40];
	int heading = 5;
};

void initServer(AsyncWebServer* server, ParamsStruct* params);

bool initEEPROM();

int EEPROMCount(int addr);

#ifdef _cplusplus
}
#endif

#endif

