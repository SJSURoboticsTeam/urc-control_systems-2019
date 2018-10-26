
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Servo.hpp"
#include "Motor.hpp"
extern "C" {
#include "Wifi.h"
#include "Structure.h"
}


#ifndef PROTOLOBE_H_
#define PROTOLOBE_H_

#ifdef _cplusplus
extern "C"{
#endif

extern "C" void DriveSystem_init();
extern "C" void DriveSystem_mode(int mode, Motor motor_object[], Servo servo_object[]);
extern "C" void DriveSystem_drive(Motor motor_object[], Servo servo_object[]);
extern "C" int DriveSystem_crab(Motor motor_object[], Servo servo_object[]);
extern "C" int DriveSystem_rotate(Motor motor_object[], Servo servo_object[]);
extern "C" void DriveSystem_debug(Motor motor_object[], Servo servo_object[]);
extern "C" void DriveSystem_idle(Motor motor_object[]);
extern "C" void DriveSystem_checkRPM(Motor motor_object[],uint8_t angle);
extern "C" void DriveSystem_slowServo(Servo servo_object[]);
extern "C" void DriveSystem_slowServoDrive(Servo servo_object[]);
extern "C" void DriveSystem_assertBrake(Motor motor_object[],uint8_t current_speed);
extern "C" void DriveSystem_tank(Motor motor_object[], Servo servo_object[]);
extern "C" void DriveSystem_checkRPMTank(Motor motor_object[]);



#ifdef _cplusplus
}
#endif

#endif 
