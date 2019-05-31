#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <WiFi.h>
#include "Source.h"
#include "Arduino.h"
#include "EEPROM.h"
#include "constants.h"
#include <iostream>

using namespace std;


void initServer(AsyncWebServer* server, ParamsStruct* params) {

    // Create addresses for network connections
    char * ssid = "SJSURoboticsAP";
    char * password = "cerberus2019";
    IPAddress Ip(192, 168, 10, 52);
    IPAddress Gateway(192, 168, 10, 100);
    IPAddress NMask(255, 255, 255, 0);

    //Create Access Point
    WiFi.softAP("ZacksESP32AP", "testpassword");
    Serial.println();
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    WiFi.config(Ip, Gateway, NMask);
    WiFi.begin(ssid, password);
    // while (WiFi.status() != WL_CONNECTED)
    // {
    //     delay(500);
    //     printf("Connecting to WiFi... ");
    // }
    printf("\nConnected to %s\n", ssid);
    
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
    server->on("/Arm", HTTP_POST, [=](AsyncWebServerRequest *request){
        if(request->hasArg("RotundaTarget"))
        {
            params->RotundaTarget = atof(request->arg("RotundaTarget").c_str());
            printf("RotundaTarget Raw: %f\n", params->RotundaTarget);

            if((params->RotundaTarget <= 1) && (params->RotundaTarget >= -1))
            {
                params->RotundaTarget = fmap(params->RotundaTarget, -0.1, 0.1, -180, 179);  //axis 5
            }
            printf("RotundaTarget: %f\n", params->RotundaTarget);
        }
        // printf("RotundaTarget Param\n");

        if(request->hasArg("ElbowTarget"))
        {
            params->ElbowTarget = atof(request->arg("ElbowTarget").c_str());
            printf("ElbowTarget: %f\n", params->ElbowTarget);

            if((params->ElbowTarget <= 1) && (params->ElbowTarget >= -1))
            {
                params->ElbowTarget = (-1 + .72) - params->ElbowTarget;
                params->ElbowTarget = fmap(params->ElbowTarget, -.98, .88, 60, 240); //axis 1
            }
            printf("ElbowTarget: %f\n", params->ElbowTarget);
        }
        // printf("ElbowTarget Param\n");

        if(request->hasArg("ShoulderTarget"))
        {
            // if(atof(request->arg("ShoulderTarget").c_str()) >= 10)
            // {
                params->ShoulderTarget = atof(request->arg("ShoulderTarget").c_str());
            //     printf("Shoulder Target: %f \n", params->ShoulderTarget);
            // }
            printf("Shoulder Target Raw: %f \n", params->ShoulderTarget);

            if((params->ShoulderTarget <= 1) && (params->ShoulderTarget >= -1))
            {
                params->ShoulderTarget = fmap(params->ShoulderTarget, -.5, 1, 0, 90);    //axis 2
            }
            printf("ShoulderTarget: %f\n", params->ShoulderTarget);
        }
        // printf("ShoulderDuration_ms Param\n");

        if(request->hasArg("WristPitch"))
        {
            // if ((atof(request->arg("WristPitch").c_str()) > 10) || (atof(request->arg("WristPitch").c_str()) < -10))
            // {
                params->WristPitch = atof(request->arg("WristPitch").c_str());
            //     xSemaphoreGive(params->xWristPitchSemaphore);
            //     printf("Wrist Pitch!: %f\n\n\n", params->WristPitch);
            // }

            printf("Wrist Pitch Raw: %f \n", params->WristPitch);

            // if((params->WristPitch <= 1) && (params->WristPitch >= -1))
            // {
            //     //axis 0 on mimic
            //     params->WristPitch = fmap(params->WristPitch, -0.78, 1, kWristPitchLimitMin, kWristPitchLimitMax);
            // }
            printf("Wrist Pitch: %f \n\n", params->WristPitch);
        }
        // printf("WristPitch Param\n");

        if(request->hasArg("WristRoll"))
        {
            // if((atof(request->arg("WristRoll").c_str()) > 10) || (atof(request->arg("WristRoll").c_str()) < -10))
            // {
                params->WristRoll = atoi((request->arg("WristRoll").c_str()));
            //     printf("WristRoll! : %f\n", params->WristRoll);
                xSemaphoreGive(params->xWristRollSemaphore);
            // }
            printf("Wrist Roll Raw: %f \n", params->WristRoll);
            if((params->WristRoll <= 1) && (params->WristRoll >= -1))
            {
                params->WristRoll = fmap(params->WristRoll, -1, 1, 0, 360);
            }
            printf("Wrist Roll: %f \n\n", params->WristRoll);
        }
        // printf("WristRoll Param\n");

        //Raul's stuff
        if(request->hasArg("speed"))
        {
            params->update_speed = atoi(request->arg("speed").c_str());
            printf("Speed: %i\n", params->update_speed);
        }
        // printf("speed Param\n");

        if(request->hasArg("command"))
        {
            printf("Claw command recieved: %i\n", atoi(request->arg("command").c_str()));
            printf("%s\n",request->arg("command").c_str());
            if(params->update_speed > 100) params->update_speed = 100;
            params->actuator_speed = 100;

            if((atoi(request->arg("command").c_str()) == 1) || (strcmp(request->arg("command").c_str(), "open")==0)){
                params->current_direction = 1;
                printf("Claw open\n");
            }
            else if((atoi(request->arg("command").c_str()) == -1) || (strcmp(request->arg("command").c_str(), "close")==0)){
                params->current_direction = -1;
                printf("Claw close\n");
            }
            else if((atoi(request->arg("command").c_str()) == 0) || (strcmp(request->arg("command").c_str(), "stop")==0)){
                params->current_direction = 2;
                params->actuator_speed                                                                                                                                                           = 0;
                printf("Claw stop\n\n\n");
            }   
            // printf("Command: %i\n", params->current_direction);
        }
        // printf("command Param\n");
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

void initClaw()
{
    pinMode(act_PHASE, OUTPUT);
    digitalWrite(act_PHASE,LOW);
}

bool openClaw()
{
    digitalWrite(act_PHASE,HIGH);
    if(digitalRead(act_PHASE) == HIGH) return true;
    else return false;
}

bool closeClaw()
{
    digitalWrite(act_PHASE,LOW);
    if(digitalRead(act_PHASE) == LOW) return true;
    else return false;
}

// Current = (Target * α) + (Current * (1 - α))
double ExpMovingAvg(double Current, double Target, double Alpha)
{
    if(Alpha > 1)   return -1;

    double TargetPortion = Target * Alpha;
    double CurrentPotion = Current * (1 - Alpha);
    
    double EMA_Result = TargetPortion + CurrentPotion;
    return EMA_Result;
}

double fmap(double x, double in_min, double in_max, double out_min, double out_max)
{
    if(x > in_max)  x = in_max;
    else if(x < in_min)  x = in_min;

    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
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
    else              printf("done\n");

    return device_address;
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
