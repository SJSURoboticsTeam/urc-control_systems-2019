
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <WiFi.h>
#include "Source.h"
#include "Arduino.h"
#include "EEPROM.h"
#include "constants.h"
#include "Wire.h"
#include "Adafruit_BNO055.h"

void initServer(AsyncWebServer* server, ParamsStruct* params) {
    //Create Access Point
    WiFi.softAP("PowerESP", "testpassword");
    Serial.println();
    Serial.print("IP address: ");
    Serial.println(WiFi.softAPIP());
    AsyncEventSource events("/events");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");

	server->on("/updateFPIReading", HTTP_POST, [=](AsyncWebServerRequest *request) {
            char fpi_string[10];
	    itoa(params->fpi_pin, fpi_string, 10);
	    request->send(200, "text/plain", fpi_string);
	});

    events.onConnect([](AsyncEventSourceClient *client) {
        if(client->lastId())
	{
	    Serial.printf("Client reconnected! Last message ID that it got is: %u\m", client->lastId());
	}
    });

    //Attach event source to the server.
    server->addHandler(&events);

    //Start server.
    server->begin();
}
