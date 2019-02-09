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



extern "C" void vSayHelloTask(void *pvParameters) {
    ParamsStruct* params = (ParamsStruct*) pvParameters;

    while(1) {
        printf("Hello, %s! \n", params->name);
	    vTaskDelay(500);
    }
}

extern "C" void vPitchTask(void *pvParameters) {
    ParamsStruct* params = (ParamsStruct*) pvParameters;
    
    
    // Initalize pitch object after the delay to allow the gimbal to turn on first
    vTaskDelay(500);
    initGimbal();

    while(1) {
        //sweepMovePitch(); 
        
        // if (params->pitch_position != 0) {
        //     manualMovePitch(params->pitch_position);
        // } 
        // else {
        //     printf("Awaiting pitch position value input from mission control.\n");
        // }

        // Pitch_Servo.SetPositionPercent(30);
        // vTaskDelay(500);
        // Pitch_Servo.SetPositionPercent(90);

        manualMovePitch(30);
        vTaskDelay(500);
        manualMovePitch(90);

        vTaskDelay(500);
    }   
}

/*
    This task demonstrates how to read and write from EEPROM,
    which is non-voltaile memory that we can use to store data on the 
    ESP.
*/
extern "C" void vCountTask(void *pvParameters)
{
    int count = 0;
    EEPROM.put(BEGINING_ADDR, count);
    EEPROM.commit();

    while(1) {
        count = EEPROMCount(BEGINING_ADDR);
        printf("I have said hello %d times!\n\n\n", count);
        vTaskDelay(500);
    }
}
