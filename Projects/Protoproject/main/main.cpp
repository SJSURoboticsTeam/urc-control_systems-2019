#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <string>
#include <iostream>
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "Arduino.h"
#include "EEPROM.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "RTOStasks.h"
#include "Source.h"
#include "constants.h"

void initServer();
AsyncWebServer server(80);
AsyncEventSource events("/events");
const char kSsid[] = "MyESP32AP";
const char kPassword[] = "testpassword";

extern "C" void app_main()
{
    Serial.begin(115200);
    initArduino();
    initServer();
    pinMode(4, OUTPUT);
    digitalWrite(4, HIGH);

    if (!initEEPROM())
    {
        for (int i = 10; i >= 0; i--)
	{
	    printf("Restarting in %d seconds...\n", i);
	    usleep(1000);
	}
	printf("Restarting now.\n");
	fflush(stdout);
	esp_restart();
    }

    xTaskCreate(vSayHelloTask, "Hello", 4096, NULL, 1, NULL);
    xTaskCreate(vCountTask, "Count", 4096, NULL, 1, NULL);
}


void initServer() {
    WiFi.softAP(kSsid, kPassword);
    Serial.println();
    Serial.print("IP address: ");
    Serial.println(WiFi.softAPIP());

    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");

    // XHR example. 
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
            } 
            else {
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

    //SSE Example. When webserver opens event source at /events
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

    server.addHandler(&events);
    server.begin();
}