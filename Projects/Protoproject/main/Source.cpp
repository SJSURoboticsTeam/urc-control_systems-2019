#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <WiFi.h>
#include "Source.h"
#include "Arduino.h"
#include "EEPROM.h"
#include "constants.h"
#include "Servo_Control.hpp"

// Current position of the gimbal pitch
float move_pitch_position = 0;

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

   server->on("/pitch_update", HTTP_POST, [=](AsyncWebServerRequest *request){
        strcpy(params->name, request->arg("name").c_str());
        strcpy(params->mode, request->arg("mode").c_str());
        params->manual_move = request->arg("manual_move").toInt();
        params->gimbal_position = request->arg("gimbal_position").toFloat();

        printf("handle_update endpoint running\n");
        printf("    mode: %s \n", params->mode);
        printf("    manual move: %i \n", params->manual_move);
        printf("    gimbal_position: %f \n", params->gimbal_position);
        request->send(200, "text/plain", "Success");
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

void initGimbal() {
    Pitch_Servo.InitServo(PITCH_SERVO_PIN, SERVO_CHANNEL, SERVO_TIMER, 
                      SERVO_FREQUENCY, PITCH_SERVO_MIN, PITCH_SERVO_MAX);

    // Pitch_Servo.InitServoMotor(uint32_t pin, uint32_t channel, uint32_t timer, 
    //                         uint32_t frequency, float max, float min, 
    //                         float dead_min, float dead_max);
    printf("Gimbal has been initialized for movement.\n");

}

void initCameraLens() {
    Pitch_Servo.InitServo(LENS_SERVO_PIN, SERVO_CHANNEL, SERVO_TIMER, 
                      SERVO_FREQUENCY, LENS_SERVO_MIN, LENS_SERVO_MAX);
}

void centerMovePitch() {
    Pitch_Servo.SetPositionPercent(SERVO_CENTER);
    printf("The camera is now centered.\n");

    vTaskDelay(200);
}

void upMovePitch(int position) {
    Pitch_Servo.SetPositionPercent(position);
    printf("The camera is rotating upwards.\n");

    vTaskDelay(200);
}

void downMovePitch(int position) {
    Pitch_Servo.SetPositionPercent(position);
    printf("The camera is rotating downwards.\n");

    vTaskDelay(200);
}

void sweepMovePitch() {

    upMovePitch(SERVO_UP);
    centerMovePitch();
    downMovePitch(SERVO_DOWN);
    printf("Sweeping has been enabled.\n");
    
}
