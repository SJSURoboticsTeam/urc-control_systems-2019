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


void initServer(AsyncWebServer* server, ParamsStruct* params);

void dispenseFluid(int x);// dispenses fluid for pod x

void sealPODS(int x, bool open); //seals POD x 
								// open = true -> open lid
								// open = false -> close lid

void startPOD(bool start, int x);//takes boolean to start/stop gygercounter  and pod id ->x

double getPercent(int angle);//maps -+90 to 0 - 100

int gygerPin(int id); // returns pin number for gyger counter take POD ID parameter(0-6)

String writeData(bool type, int id, int val, u_long time_stamp); //TRUE to write data. will return -1 
											//FALSE to return data
											// id -> PODS identifier (0-6) val -> cpm
void resetString(int id); //resets string that writeData() writes to to "POD: id"

int servoInoculationPin(int x); //returns servo pin for PODS #x that controls the incoculationi fluid

int servoLidPin(int x);// returns servo pin on PODS #x that seals the PODS

//interupt finctions that count the number of emmisions detected
void emissionCount(void* id);





#ifdef _cplusplus
}
#endif

#endif

