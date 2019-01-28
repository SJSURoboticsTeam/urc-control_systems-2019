#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef PROTOPROJECT_H_
#define PROTOPROJECT_H_

#ifdef _cplusplus
extern "C" {
#endif

struct ParamsStruct {
<<<<<<< HEAD
    char name[40]; // test parameter
    int mode; // arm or manual control mode
    double pitch_value; // y axis value of the gimbal
    
=======
    char name[40];
>>>>>>> f98b6e9e4ef0b1394cad1d5506dec987de5c5f5c
};

void initServer(AsyncWebServer* server, ParamsStruct* params);

bool initEEPROM();

int EEPROMCount(int addr);

void hello_world(char* name);

#ifdef _cplusplus
}
#endif

#endif

