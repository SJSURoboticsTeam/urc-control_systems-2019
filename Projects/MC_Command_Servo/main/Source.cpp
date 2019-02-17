#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <WiFi.h>
#include "Source.h"
#include "Arduino.h"
#include "EEPROM.h"
#include "constants.h"
#include <iostream>

using namespace std;


void initServer(AsyncWebServer* server, ParamsStruct* params) {
    //Create Access Point
    WiFi.softAP("ZacksESP32AP", "testpassword");
    Serial.println();
    Serial.print("IP address: ");
    Serial.println(WiFi.softAPIP());

    
    AsyncEventSource events("/events");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");

    /* XHR Example.
        - Param "name" is sent from mission control.
        - "name" is copied to the params object
        - params object is then passed to vSayHelloTask - see main.cpp
        - vSayHello task then accesses name directly.

        Note: for ANY parameters you want to use, you must add them to
        the paramsStruct struct located in Source.h first. 
    */
    server->on("/Arm", HTTP_POST, [=](AsyncWebServerRequest *request){
        if(request->hasArg("RotundaTarget"))
        {
            params->RotundaTarget = atoi(request->arg("RotundaTarget").c_str());
        }
        printf("RotundaTarget Param\n");
        if(request->hasArg("ElbowTarget"))
        {
            params->ElbowTarget = atoi(request->arg("ElbowTarget").c_str());
        }
        printf("ElbowTarget Param\n");
        if(request->hasArg("ShoudlerDuration_ms"))
        {
            params->ShoudlerDuration_ms = atoi(request->arg("ShoudlerDuration_ms").c_str());
        }
        printf("ShoudlerDuration_ms Param\n");
        if(request->hasArg("Wrist_Delta"))
        {
            params->Wrist_Delta = atoi(request->arg("Wrist_Delta").c_str());
        }
        printf("Wrist_Delta Param\n");
        if(request->hasArg("Wrist_Dimension"))
        {
            params->Wrist_Dimension = atoi((request->arg("Wrist_Dimension").c_str()));
        }
        printf("Wrist_Dimension Param\n");
        if(request->hasArg("Wrist_Dimension") || request->hasArg("Wrist_Delta"))
        {
            xSemaphoreGive(params->xWristSemaphore);
        }
        //Raul's stuff
        if(request->hasArg("speed"))
        {
            params->update_speed = atoi(request->arg("speed").c_str());
        }
        printf("speed Param\n");
        if(request->hasArg("command"))
        {
            if(params->update_speed > 100) params->update_speed = 100;
            params->actuator_speed = params->update_speed;

            if(!strcmp(request->arg("command").c_str(),"open")){
                params->current_direction = 1;
            }
                else if(!strcmp(request->arg("command").c_str(),"close")){
                params->current_direction = -1;
                }
            else if(!strcmp(request->arg("command").c_str(),"stop")){
                params->current_direction = 2;
                params->actuator_speed = 0;
            }   
        }
        printf("command Param\n");
        // strcpy(params->name, request->arg("heading").c_str());
        // request->send(200, "text/plain", "Success");
    });
    
    
    /* SSE Example.
        - SSEs will be used to continuously send data that was
        not necessarily requested by mission control
        (e.g. temperature, something we should send periodically)

        - Once mission control declares the ESPs IP address at a certain 
        endpoint to be an EventSource, the ESP can trigger events on the web
        interface, which the web interface can attach event listeners to
        (similar to how we are attaching event listeners for when we recieve
        XHRs to /update_name above, allowing us to do things when we recieve an 
        XHR).
        - Below's example is an example of sending SSEs when mission control
        declares our ip address and endpoint (e.g. 192.168.4.1/events) to be
        an event source.
        - More info on this concept here: 
            https://developer.mozilla.org/en-US/docs/Web/API/EventSource
    */
    
    events.onConnect([](AsyncEventSourceClient *client) {
      if(client->lastId())
      {
        Serial.printf("Client reconnected! Last message ID that it gat is: %u\n", client->lastId());
      }
      // send event with message "hello!", id current millis
      // and set reconnect delay to 1 second
      client->send("hello!", NULL, millis(), 1000);
      delay(1000);
      client->send("hello!", NULL, millis(), 1000);
      delay(1000);
      client->send("hello!", NULL, millis(), 1000);
      delay(1000);
      client->send("hello!", NULL, millis(), 1000);
      delay(1000);
    });

    //Attach event source to the server.
    server->addHandler(&events);

    //Start server.
    server->begin();
}

bool initEEPROM() {
    bool status = EEPROM.begin(EEPROM_SIZE);
    switch(status)
    {
        case 0:
	    printf("ERROR: EEPROM initialization failure.\n");
	    usleep(1000);
	    break;
	case 1:
	    printf("Successfully initialized EEPROM, size = %d.\n", EEPROM_SIZE);
	    usleep(1000);
	    break;
	default:
	    break;
    }
    return status;    
}

int EEPROMCount(int addr)
{
    int data = EEPROM.read(addr);
    data++;
    EEPROM.write(addr, data);
    EEPROM.commit();
    return data;
}

void initClaw()
{
    pinMode(act_PHASE, OUTPUT);
    digitalWrite(act_PHASE,LOW);
}

bool openClaw()
{
    digitalWrite(act_PHASE,HIGH);
    if(digitalRead(act_PHASE) == HIGH) return true;
    else return false;
}

bool closeClaw()
{
    digitalWrite(act_PHASE,LOW);
    if(digitalRead(act_PHASE) == LOW) return true;
    else return false;
}