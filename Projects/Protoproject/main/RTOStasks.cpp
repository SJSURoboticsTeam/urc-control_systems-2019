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

    int pitch_position = 35;
    int powermode = 0;

    vTaskDelay(500);
    initGimbal();
    initCameraLens();
    initPower();
    centerMovePitch();
    powerGimbal(powermode);

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

            Case 0: CENTER mode makes the camera gimbal stay centered
            Case 1: UP mode makes the camera gimbal face upwards
            Case 2: DOWN mode makes the camera gimbal stay downwards
            Case 3: STOP mode makes the camera gimbal stop moving and stay in one position

        */
        else if (strcmp(params->mode, "manual") == 0) {
            printf("Manual mode\n");
            printf("----------------------.\n");
            switch(params->manual_move) {
                case CENTER: // CENTER
                    pitch_position = SERVO_CENTER;

                    centerMovePitch();
                    printf("Pitch Position: %i\n", pitch_position);
                    params->gimbal_position = SERVO_CENTER;
                    break;
                case UP: // UP
                    pitch_position -= SERVO_SHIFT;
                    if (pitch_position <= SERVO_UP) {
                        pitch_position = SERVO_UP;
                    }

                    printf("Pitch Position: %i\n", pitch_position);
                    params->gimbal_position = pitch_position;
                    upMovePitch(pitch_position);
                    break;
                case DOWN: // DOWN
                    pitch_position += SERVO_SHIFT;
                    if (pitch_position >= SERVO_DOWN) {
                        pitch_position = SERVO_DOWN;
                    }

                    printf("Pitch Position: %i\n", pitch_position);
                    params->gimbal_position = pitch_position;
                    downMovePitch(pitch_position);
                    break;
                case STOP: // STOP
                    printf("Pitch Position: %i\n", pitch_position);
                    params->gimbal_position = pitch_position;
                    stopMovePitch(pitch_position);
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
            downMovePitch(SERVO_DOWN);
        }
        /*
            The gimbal switches on for camera movement operation.
        */
        else if (strcmp(params->mode, "on") == 0) {
            printf("The gimbal is on\n");
            printf("----------------------.\n");
            powermode = 1;
            powerGimbal(powermode);
        }
        /*
            The gimbal turns off.
        */
        else if (strcmp(params->mode, "off") == 0) {
            printf("The gimbal is off\n");
            printf("----------------------.\n");
            powermode = 0;
            powerGimbal(powermode);
        }
        else {
            // printf("Mode not valid\n");
            // printf("----------------------.\n");
        }

        // sweepMovePitch();
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
