#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <vector>
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "Arduino.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "RTOStasks.h"
#include "Source.h"
#include "constants.h"

using namespace std;

// Parameters to be updated by mission control
ParamsStruct params;

// Server used to listen for XHRs, and send SSEs.
AsyncWebServer server(80);

// Create the Steering Servos

extern "C" void app_main() {
    
    initServer(&server, &params);

    //Create freeRTOS tasks.
    xTaskCreate(vGPSTask, "GPS", 4096, (void *) &params, 1, NULL);

}

void initServer(AsyncWebServer* server, ParamsStruct* params) {
    
    // Create addresses for network connections
    char * ssid = "SJSURoboticsAP";
    char * password = "cerberus2019";
    IPAddress Ip(192, 168, 10, 60);
    IPAddress Gateway(192, 168, 10, 100);
    IPAddress NMask(255, 255, 255, 0);
    
    // Configure the soft AP
    WiFi.mode(WIFI_AP_STA);    
    WiFi.softAP("MyESP32AP", "testpassword");
    Serial.println();
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());

    // Connect to the Rover's AP
    WiFi.config(Ip, Gateway, NMask);
    // WiFi.begin(ssid, password);
    // while (WiFi.status() != WL_CONNECTED)
    // {
    //     delay(500);
    //     printf("Connecting to WiFi... ");
    // }
    printf("\nConnected to %s\n", ssid);

    AsyncEventSource events("/events");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "PUT, GET, OPTIONS");
    

    server->on("/test", HTTP_POST, [=](AsyncWebServerRequest *request) {
        //declare individual vectors for your keys and values. 
        vector <String> keys, vals;
        keys.push_back("key1");
        keys.push_back("key2");
        keys.push_back("key3");
        //even if the values are ints, please convert them to strings (hacky, sorry.)
        vals.push_back("val1");
        vals.push_back("val2");
        vals.push_back("val3");

        //call the helper
        // String json = makeJsonString(keys, vals);
        String json = "makeJsonString(keys, vals)";

        //send
        request->send(200, "text/string", json);
    });
    
    
    // /* SSE Example.
    //     - SSEs will be used to continuously send data that was
    //     not necessarily requested by mission control
    //     (e.g. temperature, something we should send periodically)

    //     - Once mission control declares the ESPs IP address at a certain 
    //     endpoint to be an EventSource, the ESP can trigger events on the web
    //     interface, which the web interface can attach event listeners to
    //     (similar to how we are attaching event listeners for when we recieve
    //     XHRs to /update_name above, allowing us to do things when we recieve an 
    //     XHR).
    //     - Below's example is an example of sending SSEs when mission control
    //     declares our ip address and endpoint (e.g. 192.168.4.1/events) to be
    //     an event source.
    //     - More info on this concept here: 
    //         https://developer.mozilla.org/en-US/docs/Web/API/EventSource
    // */
    
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

    // //Attach event source to the server.
    server->addHandler(&events);

    //Start server.
    server->begin();
}