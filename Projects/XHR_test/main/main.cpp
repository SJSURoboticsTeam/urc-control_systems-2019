#include "Arduino.h"
#include <WiFi.h>
#include <FS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <string>
#include <iostream>

constexpr uint8_t kLedBuiltin = 2;
const char kSsid[] = "MyESP32AP";
const char kPassword[] = "testpassword";

AsyncWebServer server(80);
AsyncEventSource events("/events");

void XHRTask(void * parameter);


void setup()
{
    Serial.begin(115200);

    pinMode(kLedBuiltin, OUTPUT);

    WiFi.softAP(kSsid, kPassword);
    Serial.println();
    Serial.print("IP address: ");
    Serial.println(WiFi.softAPIP());

    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");

    xTaskCreate(
                XHRTask,     /* Task function. */
                "XHR_test",   /* String with name of task. */
                10000,        /* Stack size in words. */
                NULL,        /* Parameter passed as input of the task */
                1,           /* Priority of the task. */
                NULL);       /* Task handle. */


    events.onConnect([](AsyncEventSourceClient *client)
    {
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
    //HTTP Basic authentication
    // events.setAuthentication("user", "pass");
    server.addHandler(&events);
    server.begin();
}

void loop() {
  
}

void XHRTask( void * parameter )
{
  

  while(true)
  {

  }

    vTaskDelete(NULL);
}