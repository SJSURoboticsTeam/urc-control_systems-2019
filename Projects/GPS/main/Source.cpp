#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <WiFi.h>
#include <string.h>
#include <math.h>
#include <ctime>
#include <sstream>
#include <vector>
#include "Source.h"
#include "Arduino.h"
#include "EEPROM.h"
#include "constants.h"
#include "Servo_Control.hpp"
#include "Motor_Control_rev1.hpp"

String makeJsonString(vector<String>& keys, vector<String>& vals);

void initServer(AsyncWebServer* server, ParamsStruct* params) {
    // Create addresses for network connections
    char * ssid = "SJSURoboticsAP";
    char * password = "cerberus2019";
    IPAddress Ip(192, 168, 10, 19);
    IPAddress Gateway(192, 168, 10, 100);
    IPAddress NMask(255, 255, 255, 0);
    
    // Configure the soft AP
    WiFi.mode(WIFI_AP_STA);    
    WiFi.softAP("GPS", "GPS1234");
    Serial.println();
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());

    // Connect to the Rover's AP
    //WiFi.config(Ip, Gateway, NMask);
    //WiFi.begin(ssid, password);
    //while (WiFi.status() != WL_CONNECTED)
    //{
    //    delay(500);
    //    printf("Connecting to WiFi... ");
    //}
    //printf("\nConnected to %s\n", ssid);
    
    AsyncEventSource events("/events");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");

    server->on("/getPositionalData", HTTP_POST, [=](AsyncWebServerRequest *request) {
        //current format: [longitude],[latitude],[epoch]
        char* output_str;

        std::time_t time_obj = std::time(nullptr);

        std::stringstream ss;
        ss << params->longitude << ',' << params->latitude << ',' << time_obj;

        //send
        request->send(200, "text/plain", ss.str().c_str());
    });

    server->on("/getPositionalDataJSON", HTTP_POST, [=](AsyncWebServerRequest *request) {
        //declare individual vectors for your keys and values. 
        vector <String> keys, vals;
        keys.push_back("longitude");
        keys.push_back("latitude");
        keys.push_back("epoch");


        //even if the values are ints, please convert them to strings (hacky, sorry.)
        vals.push_back(params->longitude);
        vals.push_back(params->latitude);

        std:time_t time_obj = std::time(nullptr);
        vals.push_back(std::to_string(time_obj));

        String json = makeJsonString(keys, vals);
        std::cout <<  json << std::endl;

        request->send(200, "application/json", json);
    })

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

    //Attach event source to the server.
    server->addHandler(&events);

    //Start server.
    server->begin();
    //printf("initServer done\n");
}

String makeJsonString(vector<String>& keys, vector<String>& vals) {
    String json = "{";
    
    for (int i=0; i<keys.size(); i++) {
      if (i) json += ",";
      json += ("\"" + keys[i] + "\":");
      json += ("\"" + vals[i] + "\"");
    }
    
    json += "}";

    return json;
}
