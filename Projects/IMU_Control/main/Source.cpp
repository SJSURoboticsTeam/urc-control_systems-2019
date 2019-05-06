#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <WiFi.h>
#include "Source.h"
#include "Arduino.h"
#include "EEPROM.h"
#include "constants.h"
#include "Wire.h"
#include "Adafruit_BNO055.h"

void initServer(AsyncWebServer* server, ParamsStruct* params) {
    //Create Access Point
    WiFi.softAP("ZacksESP32AP", "testpassword");
    Serial.println();
    Serial.print("IP address: ");
    Serial.println(WiFi.softAPIP());    
    AsyncEventSource events("/events");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");

    server->on("/imu", HTTP_POST, [=](AsyncWebServerRequest *request){
        char yaw_str[10];
	itoa(params->yaw[0],yaw_str,10);
	printf("Yaw for IMU %i: %f",atoi(request->arg("imu_address").c_str()),params->yaw[0]);
	request->send(200,"text/plain",yaw_str);
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

    printf("Scanning...\n");

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

bool i2cScanAndInit(uint8_t imu_address)
{
    Wire.begin();
    uint8_t error = 0, address = 0, device_address = 0;
    uint8_t nDevices = 0;
    bool device_found = false;
    printf("Scanning...\n");

    for(address = 1; address < 127; address++)
    {
	Wire.beginTransmission(address);
	error = Wire.endTransmission();
	if(error == 0)
	{
	    printf("I2C device found at address 0x%x\n",address);
	    device_address = address;
	    if(device_address == imu_address)
	    {
	    	device_found = true;
	    	initIMU(imu_address, Adafruit_BNO055::OPERATION_MODE_IMUPLUS);
	    }
	    nDevices++;
	}
	else if(error == 4)
	{
	    printf("Unknown error at address 0x%x\n",address);
	}
    }
    if(nDevices == 0) printf("No I2C devices found\n");
    else       	      printf("done\n");

    return device_found;
}

void initIMU(uint8_t IMU_ADDRESS, uint8_t MODE)
{
    Wire.begin();
    //Get Device ID
    uint8_t chipID = readByte(IMU_ADDRESS, BNO055_CHIP_ID);;
    printf("ID = %x\n",chipID);

    //Set to Normal Power Mode
    uint8_t pwr_mode = readByte(IMU_ADDRESS, BNO055_PWR_MODE) & ~(0x3);
    writeByte(IMU_ADDRESS, BNO055_PWR_MODE, pwr_mode);

    uint8_t opr_mode = readByte(IMU_ADDRESS, BNO055_OPR_MODE) & ~(0xF);
    //Set to IMU Mode
    opr_mode |= MODE;

    //Set to NDOF Mode
    //opr_mode |= 0x0C;

    writeByte(IMU_ADDRESS, BNO055_OPR_MODE, opr_mode);

    // Unit Select
    uint8_t unit_sel = readByte(IMU_ADDRESS, BNO055_UNIT_SEL) & ~(0x4);
    writeByte(IMU_ADDRESS, BNO055_UNIT_SEL, unit_sel);
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

int16_t getAxis(uint8_t IMU_ADDRESS, uint8_t REGISTER_ADDRESS)
{
    uint16_t bytes_val = 0;
    Wire.beginTransmission(IMU_ADDRESS);
    Wire.write(REGISTER_ADDRESS);
    Wire.requestFrom(IMU_ADDRESS, 2);
    bytes_val = (Wire.read() << 8) | Wire.read();
    Wire.endTransmission();

    return bytes_val;
}

double calculatePitch(double xAxis, double yAxis, double zAxis)
{
    double pitch = atan2(-xAxis, sqrt(yAxis*yAxis + zAxis*zAxis)) * 180/PI;
    return pitch;
}

double calculateRoll(double xAxis, double yAxis, double zAxis)
{
    int sign = (zAxis > 0) ? 1 : -1;
    float miu = 0.001;
    double roll = atan2(yAxis, sign * sqrt(zAxis*zAxis + miu*xAxis*xAxis)) * 180/PI;
    return roll;
}
