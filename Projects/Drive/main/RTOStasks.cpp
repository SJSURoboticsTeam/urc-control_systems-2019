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

/*
    This task demonstrates how to read and write from EEPROM,
    which is non-voltaile memory that we can use to store data on the 
    ESP.
*/
extern "C" void vDebugTask(void *pvParameters)
{
    ParamsStruct* Params = (ParamsStruct*) pvParameters;
    setDirection(0, ((Params->wheel_A_speed>0)?1:0));
    setSpeed(0, Params->wheel_A_speed);
    setHeading(0, Params->wheel_A_heading);
    setDirection(1, ((Params->wheel_B_speed>0)?1:0));
    setSpeed(1, Params->wheel_B_speed);
    setHeading(1, Params->wheel_B_heading);
    setDirection(2, ((Params->wheel_C_speed>0)?1:0));
    setSpeed(2, Params->wheel_C_speed);
    setHeading(2, Params->wheel_C_heading);
    ParamsStruct PrevParams; 
    PrevParams.mode = Params->mode;
    PrevParams.wheel_A_speed = Params->wheel_A_speed;
    PrevParams.wheel_A_heading = Params->wheel_A_heading;
    PrevParams.wheel_B_speed = Params->wheel_B_speed;
    PrevParams.wheel_B_heading = Params->wheel_B_heading;
    PrevParams.wheel_C_speed = Params->wheel_C_speed;
    PrevParams.brake = Params->brake;

    while(1)
    {
        if(Params->mode == 3)
        {
            printf("Debug Mode\n");
            if (Params->wheel_A_speed != PrevParams.wheel_A_speed) 
            {
                setDirection(0, ((Params->wheel_A_speed > 0) ? 1:0));
                setSpeed(0, abs(Params->wheel_A_speed));
                PrevParams.wheel_A_speed = Params->wheel_A_speed;
                printf("    wheel_A_speed: %f \n", Params->wheel_A_speed);
            }
            if (Params->wheel_A_heading != PrevParams.wheel_A_heading)
            {
                setHeading(0, Params->wheel_A_heading);
                PrevParams.wheel_A_heading = Params->wheel_A_heading;
                printf("    wheel_A_heading: %f \n", Params->wheel_A_heading);            
            }
            if (Params->wheel_B_speed != PrevParams.wheel_B_speed)
            {
                setDirection(1, ((Params->wheel_B_speed > 0) ? 1:0));
                setSpeed(1, abs(Params->wheel_B_speed));
                PrevParams.wheel_B_speed = Params->wheel_B_speed;
                printf("    wheel_B_speed: %f \n", Params->wheel_B_speed);

            }
            if (Params->wheel_B_heading != PrevParams.wheel_B_heading)
            {
                setHeading(1, Params->wheel_B_heading);
                printf("    wheel_B_heading: %f \n", Params->wheel_B_heading);
                PrevParams.wheel_B_heading = Params->wheel_B_heading;

            }
            if (Params->wheel_C_speed != PrevParams.wheel_C_speed)
            {
                setDirection(2, ((Params->wheel_C_speed > 0) ? 1:0));
                setSpeed(2, abs(Params->wheel_C_speed));
                PrevParams.wheel_C_speed = Params->wheel_C_speed;
                printf("    wheel_C_speed: %f \n", Params->wheel_C_speed);
            }
            if (Params->wheel_C_heading != PrevParams.wheel_C_heading)
            {
                setHeading(2, Params->wheel_C_heading);
                printf("    wheel_C_heading: %f \n", Params->wheel_C_heading);
                PrevParams.wheel_C_heading = Params->wheel_C_heading;
            }
        }
        vTaskDelay(500); 
        //PrevParams = *Params;
    }
}