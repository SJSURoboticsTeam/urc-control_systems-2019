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
  server.on("/calculator", HTTP_POST, [](AsyncWebServerRequest *request){
      int op = 2; //0 for add, 1 for subtract, 2 if uninitialized
      int result = 0;
      int args = request->args();
      for(int i=0;i<args;i++) {
        Serial.printf("ARG[%s]: %s\n", request->argName(i).c_str(), request->arg(i).c_str());
        //check if arg is op code or value
        if (!strcmp(request->argName(i).c_str(),"operation")) {
          //decide if performing addition or subtraction
          op = (!strcmp(request->arg(i).c_str(),"\"add\"") ? 0 : 1);
        } else {
          int current_val = atoi(request->arg(i).c_str());
          if (op == 0) { //addition
            result += current_val;
          }
          else { //subtraction
            if (i == 1) {
              result = current_val; 
            } else {
              result -= current_val;
            }
          }
        }
      }

    std::cout << "Result: " << result << std::endl;

    //convert int to char for concatenation in return string
    char result_str[10];
    itoa(result, result_str, 10);
    request->send(200, "text/plain", result_str);
  });

  while(true)
  {

  }

    vTaskDelete(NULL);
}