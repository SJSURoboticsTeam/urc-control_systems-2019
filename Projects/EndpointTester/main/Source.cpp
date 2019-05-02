#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <WiFi.h>
#include <string.h>
#include <math.h>
#include "Source.h"
#include "Arduino.h"
#include "EEPROM.h"
#include "constants.h"
#include "Servo_Control.hpp"
#include "Motor_Control_rev1.hpp"

void initServer(AsyncWebServer* server, ParamsStruct* params) {
    // Create addresses for network connections
    char * ssid = "SJSURoboticsAP";
    char * password = "cerberus2019";
    IPAddress Ip(192, 168, 10, 19);
    IPAddress Gateway(192, 168, 10, 100);
    IPAddress NMask(255, 255, 255, 0);
    
    // Configure the soft AP
    WiFi.mode(WIFI_AP_STA);    
    WiFi.softAP("Drive", "drive1234");
    Serial.println();
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());

    // Connect to the Rover's AP
    /*WiFi.config(Ip, Gateway, NMask);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        printf("Connecting to WiFi... ");
    }
    printf("\nConnected to %s\n", ssid);*/
    
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


    server->on("/handle_update", HTTP_POST, [=](AsyncWebServerRequest *request){
        const char *vars[13] = {
            "MODE", "T_MAX", "AXIS_X", "AXIS_Y", "YAW", "THROTTLE", "BRAKES",
            "MAST_POSITION", "TRIGGER", "REVERSE", "WHEEL_A", "WHEEL_B", "WHEEL_C"
        };
        for (int i=0; i<13; i++) {
            if (request->hasArg(vars[i])) {
                if (strcmp(vars[i], "MODE")) {
                    params->MODE = request->arg("MODE").toInt();  
                }
                if (strcmp(vars[i], "T_MAX")) {
                    params->T_MAX = request->arg("T_MAX").toInt();
                }
                if (strcmp(vars[i], "AXIS_X")) {
                    params->AXIS_X = 0 - (request->arg("AXIS_X").toFloat());
                    if (params->AXIS_X == -1)
                    {
                        params->AXIS_X = -0.99;
                    }   
                }
                if (strcmp(vars[i], "AXIS_Y")) {
                    params->AXIS_Y = request->arg("AXIS_Y").toFloat();    
                }
                if (strcmp(vars[i], "YAW")) {
                    params->YAW = request->arg("YAW").toFloat();    
                }
                if (strcmp(vars[i], "THROTTLE")) {
                    params->THROTTLE = (0 - (request->arg("THROTTLE").toFloat()) + 1)/2;
                    params->THROTTLE = params->THROTTLE * 0.3;    
                }
                if (strcmp(vars[i], "BRAKES")) {
                    params->BRAKES = (request->arg("BRAKES").toFloat() +1) / 2;    
                }
                if (strcmp(vars[i], "MAST_POSITION")) {
                    params->MAST_POSITION = request->arg("MAST_POSITION").toFloat();    
                }
                if (strcmp(vars[i], "TRIGGER")) {
                    params->TRIGGER = request->arg("TRIGGER").toInt();
                }
                if (strcmp(vars[i], "REVERSE")) {
                    params->REVERSE = request->arg("REVERSE").toInt();
                }
                if (strcmp(vars[i], "WHEEL_A")) {
                    params->WHEEL_A = request->arg("WHEEL_A").toInt();  
                }
                if (strcmp(vars[i], "WHEEL_B")) {
                    params->WHEEL_B = request->arg("WHEEL_B").toInt();  
                }
                if (strcmp(vars[i], "WHEEL_C")) {
                    params->WHEEL_C = request->arg("WHEEL_C").toInt();  
                }
            }
            else 
            {
                printf("ERROR. %s doesn't exist\n", vars[i]);
            }
        }
        
        printf("handle_update endpoint running\n");
        printf("    MODE: %i \n", params->MODE);
        printf("    T_MAX: %i \n", params->T_MAX);
        printf("    AXIS_X: %f \n", params->AXIS_X);
        printf("    AXIS_Y: %f \n", params->AXIS_Y);
        printf("    YAW: %f \n", params->YAW);
        printf("    THROTTLE: %f \n", params->THROTTLE);
        printf("    BRAKES: %f \n", params->BRAKES);
        printf("    MAST_POSITION: %f \n", params->MAST_POSITION);
        printf("    TRIGGER: %i \n", params->TRIGGER);
        printf("    REVERSE: %d \n", params->REVERSE);
        printf("    WHEEL_A: %d \n", params->WHEEL_A);
        printf("    WHEEL_B: %d \n", params->WHEEL_B);
        printf("    WHEEL_C: %d \n", params->WHEEL_C);
        printf("\n");
        
        request->send(200, "text/plain", "Success");
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

void initComponents()
{
    ledc_fade_func_install(ESP_INTR_FLAG_LEVEL1);

    servo_A.InitServo(SERVO_A_PIN, SERVO_A_CHANNEL, SERVO_TIMER, 
                      SERVO_FREQUENCY, SERVO_MAX, SERVO_MIN);

    servo_B.InitServo(SERVO_B_PIN, SERVO_B_CHANNEL, SERVO_TIMER,
                      SERVO_FREQUENCY, SERVO_MAX, SERVO_MIN);


    servo_C.InitServo(SERVO_C_PIN, SERVO_C_CHANNEL, SERVO_TIMER,
                      SERVO_FREQUENCY, SERVO_MAX, SERVO_MIN);

    motor_A.InitMotor(MOTOR_A_PIN, MOTOR_A_BRAKE, MOTOR_A_DIR, MOTOR_A_CHANNEL,
                      MOTOR_TIMER, MOTOR_FREQUENCY, MOTOR_MIN, MOTOR_MAX);

    motor_B.InitMotor(MOTOR_B_PIN, MOTOR_B_BRAKE, MOTOR_B_DIR, MOTOR_B_CHANNEL,
                      MOTOR_TIMER, MOTOR_FREQUENCY, MOTOR_MIN, MOTOR_MAX);
    
    motor_C.InitMotor(MOTOR_C_PIN, MOTOR_C_BRAKE, MOTOR_C_DIR, MOTOR_C_CHANNEL,
                      MOTOR_TIMER, MOTOR_FREQUENCY, MOTOR_MIN, MOTOR_MAX);

}

void initDriveMode(uint32_t heading)
{
    switch(heading)
    {
        case 0:
            servo_A.SetPositionPercent(DRIVE_POSITION_0[0]);
            servo_B.SetPositionPercent(DRIVE_POSITION_0[1]);
            servo_C.SetPositionPercent(DRIVE_POSITION_0[2]);
            break;
        case 1:            
            servo_A.SetPositionPercent(DRIVE_POSITION_1[0]);
            servo_B.SetPositionPercent(DRIVE_POSITION_1[1]);
            servo_C.SetPositionPercent(DRIVE_POSITION_1[2]);
            break;
        case 2:
            servo_A.SetPositionPercent(DRIVE_POSITION_2[0]);
            servo_B.SetPositionPercent(DRIVE_POSITION_2[1]);
            servo_C.SetPositionPercent(DRIVE_POSITION_2[2]);
            break;
        default:
            initDriveMode(0);
            break;
    }
}

void initSpinMode(bool direction)
{
    if (!direction)
    {
        servo_A.SetPositionPercent(ROTATE_POSITION_0);
        servo_B.SetPositionPercent(ROTATE_POSITION_0);
        servo_C.SetPositionPercent(ROTATE_POSITION_0);
    }
    else
    {
        servo_A.SetPositionPercent(ROTATE_POSITION_1);
        servo_B.SetPositionPercent(ROTATE_POSITION_1);
        servo_C.SetPositionPercent(ROTATE_POSITION_1);
    }
    printf("Spin Mode set to position %d.\n", direction);
}

void initCrabMode()
{
    servo_A.SetPositionPercent(50);
    servo_B.SetPositionPercent(50);
    servo_C.SetPositionPercent(50);
}

void setDirectionAllWheels(bool direction)
{
    motor_A.SetDirection(direction);
    motor_B.SetDirection(direction);
    motor_C.SetDirection(direction);
}

void setDirection(uint32_t wheel, bool direction)
{
    switch(wheel)
    {
        case 0: 
            motor_A.SetDirection(direction);
            break;
        case 1:
            motor_B.SetDirection(direction);
            break;
        case 2:
            motor_C.SetDirection(direction);
            break;
        default: break;
    }
}

void setSpeedAllWheels(double speed)
{
    motor_A.SetSpeed(speed);
    motor_B.SetSpeed(speed);
    motor_C.SetSpeed(speed);
}

void setSpeed(uint32_t wheel, uint32_t speed)
{
    switch(wheel)
    {
        case 0: 
            motor_A.SetSpeed(speed);
            break;
        case 1:
            motor_B.SetSpeed(speed);
            break;
        case 2:
            motor_C.SetSpeed(speed);
            break;
        default: break;
    }
}

void setHeading(uint32_t wheel, double percentage)
{
    switch(wheel)
    {
        case 0: 
            servo_A.SetPositionPercent(percentage);
            break;
        case 1:
            servo_B.SetPositionPercent(percentage);
            break;
        case 2:
            servo_C.SetPositionPercent(percentage);
            break;
        default: break;
    }
}

void applyBrakes(bool signal)
{
    motor_A.Brake(!signal);
    motor_B.Brake(!signal);
    motor_C.Brake(!signal);
    //printf("brakes applied\n");
}

double fmap(double x, double in_min, double in_max, double out_min, double out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

double driveModeMapping(double x, double y)
{
    // convert raw data to target angle
    double target_angle = atan2(fabs(x), fabs(y));
    double param1 = (target_angle * 180/3.1416) / 90;
    double param2 = 0;
    if(target_angle * 180/3.1416 == 45)
    {
        param2 = sqrt(pow(x, 2) + pow(y, 2)) / sqrt(2);
    }
    else if (target_angle * 180/3.1416 > 45)
    {
        param2 = sqrt(pow(x, 2) + pow(y, 2)) / sqrt(1 + pow(y, 2));
    }
    else
    {
        param2 = sqrt(pow(x, 2) + pow(y, 2)) / sqrt(pow(x, 2) + 1);
    }
    target_angle = param1 * param2 * 45;
    return target_angle;
}

void SetForward(double offset, double array[])
{
    double wheels[6] = {0}; 

    if(offset > -15 && offset < 15)
    { 
        wheels[0] = 90 + offset;
        wheels[1] = 0;
        wheels[2] = 210 + offset;
        wheels[3] = 0;
        wheels[4] = 150 + offset;
        wheels[5] = 1;
    }
    else if(offset > 45 && offset < 60)
    {
        wheels[0] = 90 + offset;
        wheels[1] = 1;
        wheels[2] = 90 + offset;
        wheels[3] = 0;
        wheels[4] = 150 + offset;
        wheels[5] = 1;
    }
    else if(offset >= 60 && offset < 75)
    {
        wheels[0] = 150 + offset - 120;
        wheels[1] = 1;
        wheels[2] = 210 + offset - 120;
        wheels[3] = 0;
        wheels[4] = 210 + offset - 120;
        wheels[5] = 1;   
    }
    else if(offset > 105 && offset < 135)
    {
        wheels[0] = 150 + offset - 120;
        wheels[1] = 1;
        wheels[2] = 210 + offset - 120;
        wheels[3] = 0;
        wheels[4] = 90 + offset - 120;
        wheels[5] = 10;
    }
    else if(offset > 165 && offset <= 180)
    {
        wheels[0] = 150 + offset - 120;
        wheels[1] = 1;
        wheels[2] = 90 + offset - 120;
        wheels[3] = 1;
        wheels[4] = 90 + offset - 120;
        wheels[5] = 0;
    }
    else if(offset >= -180 && offset < -165)
    {
        wheels[0] = 210 + offset + 120;
        wheels[1] = 1;
        wheels[2] = 150 + offset + 120;
        wheels[3] = 1;
        wheels[4] = 210 + offset + 120;
        wheels[5] = 0;
    }
    else if (offset > -35 && offset < -105)
    {
        wheels[0] = 90 + offset + 120;
        wheels[1] = 0;
        wheels[2] = 150 + offset + 120;
        wheels[3] = 1;
        wheels[4] = 210 + offset + 120;
        wheels[5] = 0;
    }
    else if(offset > -75 && offset < -60)
    {
        wheels[0] = 90 + offset + 120;
        wheels[1] = 0;
        wheels[2] = 150 + offset + 120;
        wheels[3] = 1;
        wheels[4] = 90 + offset + 120;
        wheels[5] = 1;
    }
    else if(offset >= -60 && offset < -45)
    {
        wheels[0] = 210 + offset;
        wheels[1] = 0;
        wheels[2] = 210 + offset;
        wheels[3] = 1;
        wheels[4] = 150 + offset;
        wheels[5] = 1;
    }

    for (uint16_t i = 0; i < 6; i++)
    {
        array[i] = wheels[i];
    }
}
/*
char *getHeading(double gps_data);
{

}
*/
/*bool getCurrentDirection(uint32_t wheel)
{
    bool direction;
    switch(wheel)
    {
        case 0:
            direction = motor_A->direction;
            break;
        case 1:
            direction = motor_B->direction;
            break;
        case 2:
            direction = motor_C->direction;
            break;
        default:
            direction = false;
            break;
    }
    return direction;
}*/
