#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#ifndef GIMBAL_H_
#define GIMBAL_H_

#ifdef _cplusplus
extern "C" {
#endif

void initAccelerometer();

double getAngle(char axis);

uint16_t getDutyCycle(double angle);

#ifdef _cplusplus
}
#endif

#endif