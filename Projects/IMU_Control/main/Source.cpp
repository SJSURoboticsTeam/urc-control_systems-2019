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
	printf("Yaw for IMU %i: %i",atoi(request->arg("imu_address").c_str()),params->yaw[0]);
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

int32_t calculatePitch(int32_t accel_x, int32_t accel_y, int32_t accel_z)
{
    int32_t pitch = 180 * atan( accel_y / sqrt ( accel_x * accel_x + accel_z * accel_z ))/PI;
    return pitch;
}

int32_t calculateRoll(int32_t accel_x, int32_t accel_y, int32_t accel_z)
{
    int32_t roll = 180 * atan( accel_x / sqrt ( accel_y * accel_y + accel_z * accel_z ))/PI;
    return roll;
}

imu::Vector<3> scanMPU6050(uint8_t IMU_ADDRESS)
{
    int16_t accel_x, accel_y, accel_z;
    int16_t gyro_x, gyro_y, gyro_z;
    int16_t temp;

    //Initialize Device
    Wire.begin();
    Wire.beginTransmission(IMU_ADDRESS);
    Wire.write(0x6B);
    Wire.write(0);
    Wire.endTransmission(true);

    //Read Accel, Gyro, Temp Registers
    Wire.beginTransmission(IMU_ADDRESS);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(IMU_ADDRESS, 6*2, true);

    accel_x = Wire.read()<<8 | Wire.read(); //ACCEL_X 3B (H) 3C (L)
    accel_y = Wire.read()<<8 | Wire.read(); //ACCEL_Y 3D (H) 3E (L)
    accel_z = Wire.read()<<8 | Wire.read(); //ACCEL_Z 3F (H) 40 (L)
    temp    = Wire.read()<<8 | Wire.read(); //TEMP    41 (H) 42 (L)
    gyro_x  = Wire.read()<<8 | Wire.read(); //GYRO_X  43 (H) 44 (L)
    gyro_y  = Wire.read()<<8 | Wire.read(); //GYRO_Y  45 (H) 46 (L)
    gyro_z  = Wire.read()<<8 | Wire.read(); //GYRO_Z  47 (H) 48 (L)

    imu::Vector<3> accel(accel_x, accel_y, accel_z);
    return accel;
    //Convert To Euler
    //int32_t roll = 180 * atan( accel_x / sqrt ( accel_y * accel_y + accel_z * accel_z ))/PI;
    //int32_t pitch = 180 * atan( accel_y / sqrt ( accel_x * accel_x + accel_z * accel_z ))/PI;
    //printf("GYRO_X: %i\tGYRO_Y: %i\tGYRO_Z: %i\n",gyro_x,gyro_y,gyro_z);
    //printf("PITCH: %i\tROLL: %i\n",pitch,roll);
}
