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
