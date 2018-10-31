#include <stdlib.h>
#include <stdio.h>

#ifndef PROTOPROJECT_H_
#define PROTOPROJECT_H_

#ifdef _cplusplus
extern "C" {
#endif
	
constexpr int gpio = 15;

constexpr int servoFreq = 50;
constexpr int minPulseTime_ms = 0;
constexpr int minDuty = 1024 * ( minPulseTime_ms / static_cast<double>(1000/servoFreq) );

constexpr int maxPulseTime_ms = 50;
constexpr int maxDuty = 1024 * ( maxPulseTime_ms / static_cast<double>(1000/servoFreq) );

int degreesToDuty(unsigned int degrees);

#ifdef _cplusplus
}
#endif

#endif

