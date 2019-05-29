#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <WiFi.h>
#include <vector>
#include "Source.h"
#include "Arduino.h"
#include "EEPROM.h"
#include "constants.h"
#include <iostream>

using namespace std;

String makeJsonString(vector<String>& keys, vector<String>& vals);

void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
 
  if(type == WS_EVT_CONNECT){
 
    Serial.println("Websocket client connection received");
    client->text("Hello from ESP32 Server");
 
  } else if(type == WS_EVT_DISCONNECT){
    Serial.println("Client disconnected");
 
  }
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
        String json = makeJsonString(keys, vals);

        //send
        request->send(200, "application/json", json);
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

    AsyncWebSocket ws("/ws");
    ws.onEvent(onWsEvent);
    server->addHandler(&ws);

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