#include <stdlib.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "Arduino.h"
#include "EEPROM.h"
#include "Source.h"
#include "constants.h"
#include <string>
#include <cmath>
#include "../../../Utilities/Servo_Control.hpp"
#include "../../../Utilities/Servo_Control.cpp"


extern "C" void vSayHelloTask(void *pvParameters) {
    ParamsStruct* params = (ParamsStruct*) pvParameters;

    while(1) {
        // printf("Hello, %s! \n", params->name);
        printf("Heading: %i\n", params->heading);
	    vTaskDelay(700);
    }
}

/*
    This task demonstrates how to read and write from EEPROM,
    which is non-voltaile memory that we can use to store data on the 
    ESP.
*/

extern "C" void vServoTask(void *pvParameters)
{
    ParamsStruct* params = (ParamsStruct*) pvParameters;
    double duty;

    //(pin, Chanel, Timer, Freq, Min Max)
    Servo myServo(15, 0, 0, 50, 2, 5);


    while(1)
    {

        duty = 50 + ((params->heading / 90.0) * 50);
        printf("Heading: %i      XHR duty:%f\n",params->heading, duty);
        myServo.SetPositionPercent(duty);
        vTaskDelay(500);
    }
}



extern "C" void vRotundaTask(void *pvParameters)
{
    // Notes:
    // MS sends a command of 0 to 360 OR -180 to 180
    //     if the latter, just add 180 to target

    ParamsStruct* params = (ParamsStruct*) pvParameters;

    double current_position_translate;
    double rotunda_target_translate;
    double current_position = kRotundaStartPos;
    double prev_target = params->RotundaTarget;
    double rotunda_current_duty = kRotundaStartDuty;

    //(pin, Chanel, Timer, Freq, Min Max)
    Servo myServo(15, 0, 0, 50, 2, 5);  //all this needs to change, check the library
    myServo.SetPositionPercent(kRotundaStartDuty);

    while(1)
    {
        if(prev_target != params->RotundaTarget)  //remember: range is -180 to 180
        {
            printf("RotundaTarget: %f\n", params->RotundaTarget);
            printf("current_position: %f\n", current_position);

            current_position_translate = std::fmod(current_position, 360); //translate from 0-3600 into 0-360
            rotunda_target_translate = params->RotundaTarget + 180;

            printf("current_position_translated: %f\n", current_position_translate);
            printf("rotunda_target_translated: %f\n", rotunda_target_translate);

            if(current_position_translate >= params->RotundaTarget)
            {
                //if distance to target via rotating left is larger than rotating right
                if( (current_position_translate - params->RotundaTarget) 
                    >= ((params->RotundaTarget + 360) - current_position_translate) )
                {
                    current_position += (params->RotundaTarget + 360) - current_position_translate;
                    printf("Increasing duty cycle is most efficient, new position: %f\n\n",current_position);
                }
                else
                {
                    current_position += current_position_translate - params->RotundaTarget;
                    printf("Decreasing duty cycle is most efficient, new position: %f\n\n",current_position);
                }
            }
            else
            {
                printf("Fell through, current_position_translate < params->RotundaTarget");
            }
            //compare current_position_translate and params->RotundaTarget

            // figure out if distance to target is smaller going down or Up
            //     Ex: if current = 200, and new_target = 10, its more efficient to increase 170 degrees, resulting in a 
            //     modulo of 10, rather than decreasing 190 degrees
            
            // Make sure that direction is possible within the 0-3600 limit

            // Update the duty cycle accordingly
            //     Should probably move toward that new duty cycle over a period of time, so the robot isn't super jerky
            prev_target = params->RotundaTarget;
        }
        vTaskDelay(700);   
    }
}
