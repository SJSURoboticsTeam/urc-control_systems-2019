#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <WiFi.h>
#include <math.h>
#include <iostream>
#include <string>
#include "PODS.h"
#include "Arduino.h"
#include "constants.h"
#include "driver/ledc.h"
#include "Servo_Control.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "RTOStasks.h"
using namespace std;
//portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
////////////////////////////////////////////////////////////////////////////////
//                               BEGIN CODE HERE                              //
////////////////////////////////////////////////////////////////////////////////


void initServer(AsyncWebServer* server, ParamsStruct* params) {
    //Create Access Point
    WiFi.softAP("MyESP32AP", "testpassword");
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
    server->on("/update_name", HTTP_POST, [=](AsyncWebServerRequest *request){
        strcpy(params->name, request->arg("name").c_str());
        request->send(200, "text/plain", "Success");
   
    });
    
    /*******************************
		start PODS comand
    ********************************/
    server->on("/start_pod", HTTP_POST, [=](AsyncWebServerRequest *request){
         int x = atoi(request->arg("id").c_str());
         bool y = atoi(request->arg("start").c_str());
         startPOD(y,x);
         
         if(y)
         {
        	request->send(200, "text/plain", "POD strarted");
		}
		else
		{
			startPOD(false, x);
			request->send(200, "text/pain", "POD suspended/failed to start");
		}
    });

    /*************************
		return data comand
    ***************************/
    server->on("/data", HTTP_POST, [=](AsyncWebServerRequest *request){
        String type = request->arg("type").c_str();
        int x = atoi(request->arg("id").c_str());
        int z = callData(x);
        char c[15];

        String data;
         itoa(z, c, 10);
         data = c;
        if(type == "cpm")
         	{
        		request->send(200, "text/plain", data);
        	}
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


void startPOD(bool start, int x)
{
	
	switch(x)
	{
		case 0: vTaskResume(xGyger0);
			break;
		case 1: vTaskResume(xGyger1);
			break;
		case 2: vTaskResume(xGyger2);
			break;
		case 3: vTaskResume(xGyger3);
			break;
		case 4: vTaskResume(xGyger4);
			break;
		case 5: vTaskResume(xGyger5);
			break;
		case 6: vTaskResume(xGyger6);
			break;
		default:
			break;
	}
}


void sealPODS(int x)
{

uint32_t servo1_frequency = 50; //hz
uint32_t servo1_gpio_pin = servoLidPin(x);
uint32_t servo1_timer = 0;
uint32_t servo1_channel = 0;
float servo1_min = 5;
float servo1_max = 10;

		//servo object for PODS door
	Servo servo1(servo1_gpio_pin,servo1_channel,servo1_timer, 
		servo1_frequency, servo1_max, servo1_min);

	servo1.SetPositionPercent(getPercent(-90));


}

/**************************
	rotates a servo to despens an amount of fluid
****************************/
void dispenseFluid(int x)
{

uint32_t servo1_frequency = 50; //hz
uint32_t servo1_gpio_pin = servoInoculationPin(x);
uint32_t servo1_timer = 0;
uint32_t servo1_channel = 0;
float servo1_min = 5;
float servo1_max = 10;
int up = 90;
int down = -90;

		//servo object for sterilization fluid
	Servo servo2(inoculation_servo7_pin,servo1_channel,servo1_timer, 
					servo1_frequency, servo1_max, servo1_min);

		//servo object for inoculation fluid
	Servo servo1(servo1_gpio_pin,servo1_channel,servo1_timer, 
		servo1_frequency, servo1_max, servo1_min);

	servo1.SetPositionPercent(getPercent(down));

	if(x == 7)
	{
		servo2.SetPositionPercent(getPercent(down));
	}

	vTaskDelay(10000 / portTICK_PERIOD_MS);

	if(x == 7)
	{
		servo2.SetPositionPercent(getPercent(up));
	}

	servo1.SetPositionPercent(getPercent(up));

}


double getPercent(int angle)
{
	//servo 5-10% 
	int percent = map(angle, -90, 90, 0, 100);

	

	//if(percent < 5 or percent > 10)
	//{
	//	percent = 6;
	//}


	return percent;

}




int servoInoculationPin(int x)
{
			switch(x)
	{
		case 0: return inoculation_servo0_pin;
			break;
		case 1: return inoculation_servo1_pin;
			break;
		case 2: return inoculation_servo2_pin;
			break;
		case 3: return inoculation_servo3_pin;
			break;
		case 4: return inoculation_servo4_pin;
			break;
		case 5: return inoculation_servo5_pin;
			break;
		case 6: return inoculation_servo6_pin;
			break;
		default: return -1;
			break;
	}
}

int servoLidPin(int x)
{
			switch(x)
	{
		case 0: return lid_servo_pin0;
			break;
		case 1: return lid_servo_pin1;
			break;
		case 2: return lid_servo_pin2;
			break;
		case 3: return lid_servo_pin3;
			break;
		case 4: return lid_servo_pin4;
			break;
		case 5: return lid_servo_pin5;
			break;
		case 6: return lid_servo_pin6;
			break;
		default: return -1;
			break;
		}
}

/*********************************
	interupt functions
********************************/

void initInteruptPins()
{
	pinMode(gyger0_pin, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(gyger0_pin), emissionCount0, RISING);

	pinMode(gyger1_pin, INPUT_PULLUP);
	//attachInterrupt(digitalPinToInterrupt(gyger1_pin), emissionCount1, FALLING);
	
	pinMode(gyger2_pin, INPUT_PULLUP);
	//attachInterrupt(digitalPinToInterrupt(gyger2_pin), emissionCount2, FALLING);
	
	pinMode(gyger3_pin, INPUT_PULLUP);
	//attachInterrupt(digitalPinToInterrupt(gyger3_pin), emissionCount3, FALLING);
	
	pinMode(gyger4_pin, INPUT_PULLUP);
	//attachInterrupt(digitalPinToInterrupt(gyger4_pin), emissionCount4, FALLING);
	
	pinMode(gyger5_pin, INPUT_PULLUP);
	//attachInterrupt(digitalPinToInterrupt(gyger5_pin), emissionCount5, FALLING);
	
	pinMode(gyger6_pin, INPUT_PULLUP);
	//attachInterrupt(digitalPinToInterrupt(gyger6_pin), emissionCount6, FALLING);
	
	std::cout <<"interrupt pins initialized \n";
}


int callData(int id)
{
	switch (id)
	{
		case 0: return cpm0;
			break;
		case 1: return cpm1;
			break;
		case 2: return cpm2;
			break;
		case 3: return cpm3;
			break;
		case 4: return cpm4;
			break;
		case 5: return cpm5;
			break;
		default: return -1;
			break;
	}
}



/****************************
	interupt ffunctions
*****************************/
void emissionCount0()
{
	
  	eCount0 ++;
}

void emissionCount1()
{
	eCount1++;
}

void emissionCount2()
{
	eCount2++;
}
void emissionCount3()
{
	eCount3++;
}
void emissionCount4()
{
	eCount4++;
}
void emissionCount5()
{
	eCount5++;
}
void emissionCount6()
{
	eCount6++;
}
/////////////////////////////////////////////////////////////////////////////////                               CODE ENDS HERE                               //
////////////////////////////////////////////////////////////////////////////////
