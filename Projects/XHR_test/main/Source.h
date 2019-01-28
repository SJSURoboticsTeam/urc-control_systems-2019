#include <stdlib.h>
#include <stdio.h>

#ifndef PROTOPROJECT_H_
#define PROTOPROJECT_H_

#ifdef _cplusplus
extern "C" {
#endif

bool initEEPROM();

int EEPROMCount(int addr);

void hello_world();

#ifdef _cplusplus
}
#endif

#endif

