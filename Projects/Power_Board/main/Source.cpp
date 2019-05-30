
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

	server->on("/updateControlVals", HTTP_POST, [=](AsyncWebServerRequest *request) {
		int params = request->params();
		for(int i=0;i<params;i++){
			AsyncWebParameter* p = request->getParam(i);

			if (!strcmp( p->name().c_str(), "input1" )) {
				digitalWrite(32, atoi(p->value().c_str()));
			} else if (!strcmp( p->name().c_str(), "input2" )) {
				digitalWrite(33, atoi(p->value().c_str()));
			} else if (!strcmp( p->name().c_str(), "input3" )) {
				// NOT USED - FOR KILL SWITCH
			} else if (!strcmp( p->name().c_str(), "input4" )) {
				digitalWrite(26, atoi(p->value().c_str()));

			} else if (!strcmp( p->name().c_str(), "input5" )) {
				digitalWrite(27, atoi(p->value().c_str()));
				
			} else if (!strcmp( p->name().c_str(), "output1" )){
				digitalWrite(0, atoi(p->value().c_str()));

			} else if (!strcmp( p->name().c_str(), "output2" )) {
				digitalWrite(4, atoi(p->value().c_str()));

			} else if (!strcmp( p->name().c_str(), "output3" )) {
				digitalWrite(16, atoi(p->value().c_str()));
				
			} else if (!strcmp( p->name().c_str(), "output4" )) {
				digitalWrite(17, atoi(p->value().c_str()));

			} else if (!strcmp( p->name().c_str(), "output5" )) {
				digitalWrite(5, atoi(p->value().c_str()));				
			}
		}
		// input
		// Battery1 32
		// digitalWrite(32, );
		// Battery2 33
		// Battery3 physical kill switch
		// Battery4 26
		// Battery5 27

		// // output
		// output1 0
		// Output2 4
		// output3 16
		// Output4 17
		// Output5 5



		request->send(200, "text/plain", "Success");
	});

	// server->on("/requestFeedbackData", null, (res) => {
		
	// });

    // server->on("/power", HTTP_POST, [=](AsyncWebServerRequest *request){
	// 	if(request->hasArg("gpio_pin"))
	// 	{
	// 		//gpio_pin parameter determines which pin to write to
	// 		//state parameter: 1(HIGH) 0(LOW)
	// 		int32_t GPIO_PIN = atoi(request->arg("gpio_pin").c_str());
	// 		if(atoi(request->arg("state").c_str()))
	// 		{
	// 			digitalWrite(GPIO_PIN,HIGH);
	// 		}
	// 		else
	// 		{
	// 		digitalWrite(GPIO_PIN, LOW);
	// 		}
	// 	}
	// 	if(request->hasArg("isns"))
	// 	{
	// 		printf("Current Sense Values: CH1: %i\tCH2: %i\tCH3: %i\tCH4: %i\n",params->ch1_isns,params->ch2_isns,params->ch3_isns,params->ch4_isns);
	// 		char isns_str[4][10];
	// 		itoa(params->ch1_isns, isns_str[0], 10);
	// 		request->send(200, "text/plain", isns_str[0]);

	// 		itoa(params->ch2_isns, isns_str[1], 10);
	// 			request->send(200, "text/plain", isns_str[1]);

	// 		itoa(params->ch3_isns, isns_str[2], 10);
	// 			request->send(200, "text/plain", isns_str[2]);

	// 		itoa(params->ch4_isns, isns_str[3], 10);
	// 			request->send(200, "text/plain", isns_str[3]);
	// 	}
    // });

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
