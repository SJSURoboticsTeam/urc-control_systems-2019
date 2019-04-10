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

//This task displays current levels of vPos and vNeg
extern "C" void vClawTask(void *pvParameters)
{
    printf("Begin Claw Task...\n");
    ParamsStruct* myParams = (ParamsStruct*) pvParameters;
    bool task_complete = false;
    //Pin, Channel, Timer, Freq, max, min
    Servo Claw(act_ENABLE,2,1,5000,50,0);
    Claw.SetPositionPercent(0); //Set Duty Cycle to 0 at init
    initClaw();
    while(1) {
	if(myParams->current_direction == 0)
	{
	    //Do Nothing
	}
	else if(myParams->current_direction == 2)
	{
	    Claw.SetPositionPercent(0);
	}
	else if(myParams->current_direction == 1)
	{
	    Claw.SetPositionPercent(myParams->actuator_speed);
	    task_complete = openClaw();
	    if(task_complete) printf("Opening\n");
	    task_complete = false;
	    myParams->current_direction = 0;
	}
	else if(myParams->current_direction == -1)
	{
	    Claw.SetPositionPercent(myParams->actuator_speed);
	    task_complete = closeClaw();
	    if(task_complete) printf("Closing\n");
	    task_complete = false;
	    myParams->current_direction = 0;
	}
	printf("PHASE = %i  ENABLE = %i\n", digitalRead(act_PHASE),myParams->actuator_speed);
	vTaskDelay(100);
    }
}
