#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <WiFi.h>
#include "Source.h"
#include "Arduino.h"
#include "EEPROM.h"
#include "constants.h"
#include "Wire.h"


void initServer(AsyncWebServer* server, ParamsStruct* params) {
    //Create Access Point
    WiFi.softAP("ZacksESP32AP", "testpassword");
    Serial.println();
    Serial.print("IP address: ");
    Serial.println(WiFi.softAPIP());    
    AsyncEventSource events("/events");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");

    server->on("/update_name", HTTP_POST, [=](AsyncWebServerRequest *request){
        strcpy(params->name, request->arg("name").c_str());
        request->send(200, "text/plain", "Success");
    });

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

uint8_t  i2c_scanner()
{
    Wire.begin();
    uint8_t error = 0, address = 0, device_address = 0;
    uint8_t nDevices = 0;

    printf("Scanning...");

    for(address = 1; address < 127; address++)
    {
	Wire.beginTransmission(address);
	error = Wire.endTransmission();
	if(error == 0)
	{
	    printf("I2C device found at address 0x%x\n",address);
	    device_address = address;
	    nDevices++;
	}
	else if(error == 4)
	{
	    printf("Unknown error at address 0x%x\n",address);
	}
    }
    if(nDevices == 0) printf("No I2C devices found\n");
    else       	      printf("done\n");

    return device_address;
}

void initIMU(uint8_t IMU_ADDRESS)
{
    Wire.begin();
    //Get Device ID
    uint8_t chipID = readByte(IMU_ADDRESS, BNO055_CHIP_ID);;
    printf("ID = %x\n",chipID);

    //Set to IMU Mode
    writeByte(IMU_ADDRESS, BNO055_OPR_MODE, 0xF8);
}

void writeByte(uint8_t IMU_ADDRESS, uint8_t REGISTER_ADDRESS, uint8_t VALUE)
{
    Wire.beginTransmission(IMU_ADDRESS);
    Wire.write(REGISTER_ADDRESS);
    Wire.write(VALUE);
    Wire.endTransmission(); 
}

uint8_t readByte(uint8_t IMU_ADDRESS, uint8_t REGISTER_ADDRESS)
{
    uint8_t byte_val = 0;
    Wire.beginTransmission(IMU_ADDRESS);
    Wire.write(REGISTER_ADDRESS);
    Wire.requestFrom(IMU_ADDRESS, 1);
    byte_val = Wire.read();
    Wire.endTransmission();

    return byte_val;
}

uint16_t readTwoBytes (uint8_t IMU_ADDRESS, uint8_t REGISTER_ADDRESS)
{
    uint16_t bytes_val = 0;
    Wire.beginTransmission(IMU_ADDRESS);
    Wire.write(REGISTER_ADDRESS);
    Wire.requestFrom(IMU_ADDRESS, 2);
    bytes_val = (Wire.read() << 8) | Wire.read();
    Wire.endTransmission();

    return bytes_val;
}

int16_t getAxis(uint8_t IMU_ADDRESS, uint8_t REGISTER_ADDRESS)
{
    int16_t axis_value = readTwoBytes(IMU_ADDRESS, REGISTER_ADDRESS);
    return axis_value;
}
