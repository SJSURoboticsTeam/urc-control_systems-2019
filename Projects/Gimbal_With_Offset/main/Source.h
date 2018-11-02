#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#ifndef PROTOPROJECT_H_
#define PROTOPROJECT_H_

#ifdef _cplusplus
extern "C" {
#endif
	
constexpr int servo_gpio_pin = 16;

constexpr int servoFreq = 50;

//These duty constants are the number representation 
// of my motor's duty cycle, with a 15 bit resolution

//Position the servo on its side, where: minDuty makes the servo point straight up
//minDuty makes the servo point straight up
//maxDuty makes the servo point straight down
constexpr int minDuty = 700;
constexpr int maxDuty = 4000;

constexpr int middleDuty = (minDuty + maxDuty) / 2;


double readPitch();

int32_t calcDutyOffset(int8_t targetAngle, double currentAngle );

#ifdef _cplusplus
}
#endif

#endif

