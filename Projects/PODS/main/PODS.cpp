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
#include "esp_intr_alloc.h"

using namespace std;
//portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
////////////////////////////////////////////////////////////////////////////////
//                               BEGIN CODE HERE                              //
////////////////////////////////////////////////////////////////////////////////


void initServer(AsyncWebServer* server, ParamsStruct* params) {
    //Create Access Point
    WiFi.softAP("ROAR", "testpassword");
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
		start/stop PODS comand
			-takes pod id (int) 0-6
			-takes stape param "true" or "false"
				-true starts pod
				-false stops pod
    ********************************/
    server->on("/toggle_pod", HTTP_POST, [=](AsyncWebServerRequest *request){
         printf("XHR recieved \n");
         int x = atoi(request->arg("pod").c_str()); 
         string y = request->arg("state").c_str();
         x -= 1;
         std::cout << y << "\n";
         std::cout << x << "\n";
         bool z;

         if(y =="True" or y == "true" or "TRUE")
         {
         	 z = true;
         }
         else if(y == "false" or y == "False" or y == "FALSE")
         {
         	z = false;
         }
        startPOD(z,x);
         
         y = "Pod toggled";


        char response[y.length() + 1];
		strcpy(response, y.c_str());
         request->send(200, "text/plain", response);

    });
       /*****************************
		kills all pod functionalliy
			-currently only stops pod tasks
				####need to kill power in startPOD false switch- unwritten ######
       *****************************/

        server->on("/stop_all", HTTP_POST, [=](AsyncWebServerRequest *request){
         
         for(int i = 0; i <=6; i++)
         {
         	startPOD(false,i);
         }
         
         request->send(200, "text/pain", "All POD suspended");

    });

    /*************************
		return data comand
			-returns cpm using XHR post request
			-requires (int) id and typre (string) cpm
			########### unused#########
    ***************************/
    server->on("/data", HTTP_POST, [=](AsyncWebServerRequest *request){
        
        int x = atoi(request->arg("pod").c_str());
        String data = writeData(false, x, 0, 0);
        data += "\n";
        resetString(x);
        //char c[15];

        //String data = "";
         //itoa(z, c, 10);
        // data += ;
      
        		request->send(200, "text/plain", data);
        	
    }); 
        server->on("/servo", HTTP_POST, [=](AsyncWebServerRequest *request){
        
        test_id = atoi(request->arg("pod").c_str());
        test_servo = request->arg("type").c_str();
        test_angle = atoi(request->arg("angle").c_str());

        xTaskCreate(vLidTask, "toggle lid" , 4060, NULL, 2, NULL);
      
   		request->send(200, "text/plain", "servo moved");
        	
    }); 
   /*****************
	open/close pod lid from mission control
		- pod -> id number
		- state ->  true = open lid
					false = close lid 
   *********************/
    server->on("/toggle_lid", HTTP_POST, [=](AsyncWebServerRequest *request){
         printf("XHR recieved \n");
         int x = atoi(request->arg("pod").c_str()); 
         printf(" toggle lid id: %d\n", x );
         string y = request->arg("state").c_str();
         int m = 0;
         std::cout << y << "\n";
         std::cout << x << "\n";
         //bool z;

         if(y =="True" or y == "true" or y == "TRUE")
         {
         	 //z = true;
         	 m = x + 100;
         	
         }
         else if(y == "false" or y == "False" or y == "FALSE")
         {
         	//z = false;
         	m = x;
         }

  	



        xTaskCreate(vLidTask, "toggle lid" , 4060, (void*) m, 2, NULL);
         y = "Pod lid toggled";


        char response[y.length() + 1];
		strcpy(response, y.c_str());
         request->send(200, "text/plain", response);

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
     client->send("HELLO! Conection Established", NULL, millis(), 1000);
     /*
      client->send("hello!", NULL, millis(), 1000);
      delay(1000);
      client->send("hello!", NULL, millis(), 1000);
      delay(1000);
      client->send("hello!", NULL, millis(), 1000);
      delay(1000);
      client->send("hello!", NULL, millis(), 1000);
      delay(1000);
      */
    });

    //Attach event source to the server.
    server->addHandler(&events);

    //Start server.
    server->begin();
}


/**********************
	start/stop PODS
		-bool start -> "false" = stop POD 
						"true" = start POD
		-int id -> POD ID
***********************/
void startPOD(bool start, int x)
{

	
		printf("startPOD function\n x = %d \n", x);
		//printf("state = %d \n", start );
		
	if(start == true)
	{
		switch(x)
		{
		case 0: xTaskCreate(vGygerTask, "gyger1 data", 4060, (void*)0, 1, NULL); 
				printf("task %d started \n", x);
			break;
		case 1: xTaskCreate(vGygerTask, "gyger1 data", 4060, (void*)1, 1, NULL); 
				printf("task %d started \n", x);				
			break;
		case 2: xTaskCreate(vGygerTask, "gyger2 data", 4060, (void*)2, 1, NULL); 
				printf("task %d started \n", x);
			break;
		case 3: xTaskCreate(vGygerTask, "gyger3 data", 4060, (void*)3, 1, NULL); 
				printf("task %d started \n", x);
			break;
		case 4: xTaskCreate(vGygerTask, "gyger4 data", 4060, (void*)4, 1, NULL);
				printf("task %d started \n", x);
			break;
		case 5: xTaskCreate(vGygerTask, "gyger5 data", 4060, (void*)5, 1, NULL);
				printf("task %d started \n", x);
			break;
		case 6: xTaskCreate(vGygerTask, "gyger6 data", 4060, (void*)6, 1, NULL);
				printf("task %d started \n", x);
			break;
		default:
			break;
		}
	}

	else if(start == false)
	{
		switch(x)
		{
		case 0: xQueueSend(xQueueTerminateTask, (void*) x,  pdFALSE);
				printf("task %d stopped \n", x);
			break;
		case 1: xQueueSend(xQueueTerminateTask, (void*) x,  pdFALSE);
				printf("task %d stopped \n", x);				
			break;
		case 2: xQueueSend(xQueueTerminateTask, (void*) x,  pdFALSE); 
				printf("task %d stopped \n", x);
			break;
		case 3:xQueueSend(xQueueTerminateTask, (void*) x,  pdFALSE);
				printf("task %d stopped \n", x);
			break;
		case 4: xQueueSend(xQueueTerminateTask, (void*) x,  pdFALSE);
				printf("task %d stopped \n", x);
			break;
		case 5: xQueueSend(xQueueTerminateTask, (void*) x,  pdFALSE);
				printf("task %d stopped \n", x);
			break;
		case 6:xQueueSend(xQueueTerminateTask, (void*) x,  pdFALSE);
				printf("task %d stopped \n", x);
			break;
		default:
			break;
		}
	}	

	
}



/***********************
	closes lid of POD 
		- (int)x -> POD ID
*************************/
void sealPODS(int x, bool open)
{

uint32_t servo1_frequency = 50; //hz
uint32_t servo1_gpio_pin = servoLidPin(x);
uint32_t servo1_timer = 0;
uint32_t servo1_channel = 0;
float servo1_min = 2.5;//%
float servo1_max = 12;//%
int open_angle = 90;
int close_angle = -90;
		//servo object for PODS door
	Servo servo1(servo1_gpio_pin,servo1_channel,servo1_timer, 
		servo1_frequency, servo1_max, servo1_min);
//	printf("moving servo open\n");
	//servo1.SetPositionDuty(1);
//	servo1.SetPositionPercent(getPercent(open_angle));
	//printf("user given percent: %f \n", getPercent(close_angle) );
	//vTaskDelay(3000/portTICK_PERIOD_MS);
	//delay(3000);
	if(open == true)
	{
		servo1.SetPositionPercent(getPercent(open_angle));
		//printf("%f \n", getPercent(open_angle) );
		printf("moving lid open\n");
	}

	else if(open == false)
	{
		servo1.SetPositionPercent(getPercent(close_angle));
		//printf("User given percent: %f \n", getPercent(open_angle) );
		//servo1.SetPositionDuty(150);
		printf("moving lid close\n");
	}

}

/**************************
	rotates a servo to despens inoculation fluid

****************************/
void dispenseFluid(int x)
{

uint32_t servo1_frequency = 50; //hz
uint32_t servo1_gpio_pin = servoInoculationPin(x);
uint32_t servo1_timer = 0;
uint32_t servo1_channel = 0;
float servo1_min = 2.5;
float servo1_max = 12;
double up = 90;
double down = -90;


		//servo object for inoculation fluid
	Servo servo1(servo1_gpio_pin,servo1_channel,servo1_timer, 
		servo1_frequency, servo1_max, servo1_min);

	servo1.SetPositionPercent(getPercent(down));


	vTaskDelay(5000 / portTICK_PERIOD_MS);


	servo1.SetPositionPercent(getPercent(up));

}


void moveServo(int x, String servo, int angle)
{
	
	uint32_t servo1_frequency = 50; //hz
	uint32_t servo1_gpio_pin = -1;
	uint32_t servo1_timer = 0;
	uint32_t servo1_channel = 0;
	float servo1_min = 2.5;//%
	float servo1_max = 12;//%

	if(servo == "fluid")
	{

		 servo1_gpio_pin = servoInoculationPin(x);


	}
	else if(servo == "lid")
	{

		 servo1_gpio_pin = servoLidPin(x);



	}
		//servo object for inoculation fluid
	Servo servo1(servo1_gpio_pin,servo1_channel,servo1_timer, 
		servo1_frequency, servo1_max, servo1_min);

	servo1.SetPositionPercent(getPercent(angle));

}

/*****************
	returns percent (double)0-100
		-takes angle (int) -+90 degrees
******************/
double getPercent(int angle)
{
	//servo 5-10% 
	double percent = map(angle, -90, 90, 0, 100);

	return percent;
}


/***********************
	returns pin number forr inoculation servo pin 
		- x -> POD ID
**************************/

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

/***************************
	returns pin number of POD lid servo
		- x -> POD ID
****************************/

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

/**********************
	returns pin number of gyger 
		-id -> POD ID
**********************/

int gygerPin(int id)
{
	switch(id)
	{
		case 0: return gyger0_pin;
			break;
		case 1: return gyger1_pin;
			break;
		case 2: return gyger2_pin;
			break;
		case 3: return gyger3_pin;
			break;
		case 4: return gyger4_pin;
			break;
		case 5: return gyger5_pin;
			break;
		case 6: return gyger6_pin;
			break;
		default: 
			break;
	}

	return -1;
}

/****************************************
	writes gyger data to global variables. data stored as counts per minute(cpm)
		-(bool)type -> read or write ddata
			-"true" -> write data. Will return -1
			-"false" -> read data. Will return cpm
		-(int)id -> POD ID
		-(int)val -> vallue to be writen to global variable. will b ignore if reading data
						but the parameter is still required

*****************************************/
String writeData(bool type, int id, int val, u_long time_stamp)
{

	if(type == true)
	{
			switch (id)
		{
		case 0://  cpm0 = val;
			data_string_0 += " ";
			data_string_0 += val;
			data_string_0 += ";";
			data_string_0 += time_stamp;
			break;
		case 1: // cpm1 = val;
			data_string_1 += " ";
			data_string_1 += val;
			data_string_1 += ";";
			data_string_1 += time_stamp;		
			break;
		case 2:  //cpm2 = val;
			data_string_2 += " ";
			data_string_2 += val;
			data_string_2 += ";";
			data_string_2 += time_stamp;
			break;
		case 3:  //cpm3 = val;
			data_string_3 += " ";
			data_string_3 += val;
			data_string_3 += ";";
			data_string_3 += time_stamp;
			break;
		case 4: // cpm4 = val;
			data_string_4 += " ";
			data_string_4 += val;
			data_string_4 += ";";
			data_string_4 += time_stamp;
			break;
		case 5: // cpm5 = val;
			data_string_5 += " ";
			data_string_5 += val;
			data_string_5 += ";";
			data_string_5 += time_stamp;
			break;
		case 6:// cpm6 = val;
			data_string_6 += " ";
			data_string_6 += val;
			data_string_6 += ";";
			data_string_6 += time_stamp;
			break;
		default:  
			break;
		}


	}

	if(type == false)
	{
			switch (id)
		{
		case 0: return data_string_0;
			break;
		case 1: return data_string_1;
			break;
		case 2: return data_string_2;
			break;
		case 3: return data_string_3;
			break;
		case 4: return data_string_4;
			break;
		case 5: return data_string_5;
			break;
		case 6: return data_string_6;
			break;
		default: 
			break;
		}
	}

	return "-1";
}

void resetString(int id)
{
			switch (id)
		{
		case 0:  data_string_0 = "POD: 0 cpm: ";
			break;
		case 1:  data_string_1 = "POD: 1 cpm: ";
			break;
		case 2:  data_string_2 = "POD: 2 cpm: ";
			break;
		case 3:  data_string_3 = "POD: 3 cpm: ";
			break;
		case 4:  data_string_4 = "POD: 4 cpm: ";
			break;
		case 5:  data_string_5 = "POD: 5 cpm: ";
			break;
		case 6:  data_string_6 = "POD: 6 cpm: ";
			break;
		default: 
			break;
		}	
}

/****************************
	interupt function
	converts pin_num to POD ID number (0-6) then writes ID to queue for 
	gyger task to read 
		-pin_num -> recieves pin number that called the ISR


*****************************/
void emissionCount(void* pin_num)
{
	int id;
	switch((int)pin_num)
	{
		case gyger0_pin: 
				id = 0; 
			break;
		case gyger1_pin: 
				id = 1; 
			break;
		case gyger2_pin: 
				id = 2; 
			break;
		case gyger3_pin: 
				id = 3; 
			break;
		case gyger4_pin: 
				id = 4; 
			break;
		case gyger5_pin: 
				id = 5;
		 	break;
		case gyger6_pin: 
				id = 6;
			 break;
		default: id = -1; break;
	}
	
	xQueueSendFromISR(xQueueISR, (void*) &id, pdFALSE);
    portYIELD_FROM_ISR();
}
/////////////////////////////////////////////////////////////////////////////////                               CODE ENDS HERE                               //
////////////////////////////////////////////////////////////////////////////////
