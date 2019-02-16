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
    
    // Delay to initalize pitch object to allow the gimbal to turn on first
    vTaskDelay(500);
    initGimbal();

    while(1) {
        /*
            Debug mode to troubleshoot the gimbal for any errors. IN PROGRESS.
        */
        if (strcmp(params->mode, "debug") == 0) {
            printf("Debug mode\n");
            printf("----------------------.\n");
            sweepMovePitch();
        }
        /*
            Manual mode to allow mission control user to change camera pitch position
            by different command move modes. 

            Case 0: MANUAL mode which interprets custom percentage to move pitch
            Case 1: UP mode makes the camera face upwards
            Case 2: CENTER mode makes the camera stay centered
            Case 3: DOWN mode makes the camera stay downwards

        */
        else if (strcmp(params->mode, "manual") == 0) {
            printf("Manual mode\n");
            printf("----------------------.\n");
                switch(params->command_move) {
                    case MANUAL: // MANUAL
                        if (params->manual_move != 0) {
                            manualMovePitch(params->manual_move);
                            params->pitch_position = params->manual_move;
                        } 
                        break;
                    case UP: // UP
                        upMovePitch();
                        break;
                    case CENTER: // CENTER
                        centerMovePitch();
                        break;
                    case DOWN: // DOWN
                        downMovePitch();
                        break;
                    default:
                        printf("Awaiting pitch position input from mission control.\n");
                        break;
                }
        }
        /*
            Arm mode forces the camera to stay downwards as the rotunda with the 
            robot arm and mast rotates. 
        */
        else if (strcmp(params->mode, "arm") == 0) {
            printf("Arm mode\n");
            printf("----------------------.\n");
            downMovePitch();
        }
        else {
            printf("Mode not valid\n");
            printf("----------------------.\n");
        }

        // upMovePitch();
        // downMovePitch();
        // centerMovePitch();
    }   
}

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
