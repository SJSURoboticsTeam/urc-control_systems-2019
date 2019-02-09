#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <stdlib.h>
#include <stdio.h>


#ifndef PODS_H_
#define PODS_H_

#ifdef _cplusplus
extern "C" {
#endif

struct ParamsStruct{
	char name[40];
	int id;
};


struct GygerCounter
	{
		float cps = 0;
		int cpm = 0;
		int count = 0;
		bool start = false;

	};
void initServer(AsyncWebServer* server, ParamsStruct* params);

void dispenseFluid(int x);// dispenses fluid for pod x

void sealPODS(int x); //seals POD x

void startPOD(bool start, int x);//takes boolean to start/stop gygercounter x

void killAll(bool x);//  unwritten

double getPercent(int angle);//returns a % duty cycle for a given angle

int gygerPin(int id); // returns pin number for gyger counter take POD ID parameter(0-6)

int writeData(bool type, int id, int val); //TRUE to write data. will return -1 
											//FALSE to return data
											// id -> PODS identifier (0-6) val -> cpm

int servoInoculationPin(int x); //returns servo pin for PODS #x that controls the incoculationi fluid

int servoLidPin(int x);// returns servo pin on PODS #x that seals the PODS
//interupt finctions that count the number of emmisions detected
void emissionCount(void* id);





#ifdef _cplusplus
}
#endif

#endif

