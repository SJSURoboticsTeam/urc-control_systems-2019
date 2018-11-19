#include <stdlib.h>
#include <stdio.h>
#include "constants.h"

#ifndef PODS_H_
#define PODS_H_

#ifdef _cplusplus
extern "C" {
#endif

void dispenseFluid(int x);

void sealPODS(int x);

int servoInoculationPin(int x); //returns servo pin for PODS #x that controls the incoculationi fluid

int servoHatchPin(int x);// returns servo pin on PODS #x that seals the PODS

void startPOD(bool start, int x);

void killAll(bool x);

int getPercent(int angle);

void initInteruptPins();

void emissionCount0();
void emissionCount1();
void emissionCount2();
void emissionCount3();
void emissionCount4();
void emissionCount5();
void emissionCount6();




#ifdef _cplusplus
}
#endif

#endif

