#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <string>
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "Arduino.h"
#include "Servo_Control.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "RTOStasks.h"
#include "PODS.h"
#include "constants.h" 


QueueHandle_t xQueueISR, xQueueTerminateTask, xQueueServoID,
                xQueueServoAngle, xQueueServoType;//, xQueueLid;
TaskHandle_t task0, task1, task2, task3, task4, task5, task6;
ParamsStruct params;
AsyncWebServer server(80);

    Servo inoculation_servo0(inoculation_servo0_pin,0,0, 
        servo_frequency, servo_max, servo_min);

    Servo inoculation_servo1(inoculation_servo1_pin,1,0, 
        servo_frequency, servo_max, servo_min);

    Servo inoculation_servo2(inoculation_servo2_pin,2,0, 
        servo_frequency, servo_max, servo_min);

    Servo inoculation_servo3(inoculation_servo3_pin,3,0, 
        servo_frequency, servo_max, servo_min);

    Servo inoculation_servo4(inoculation_servo4_pin,4,0,
        servo_frequency, servo_max, servo_min);

    Servo inoculation_servo5(inoculation_servo5_pin,5,0, 
        servo_frequency, servo_max, servo_min);

    Servo inoculation_servo6(inoculation_servo6_pin,6,0, 
        servo_frequency, servo_max, servo_min);

    Servo lid_servo0(lid_servo_pin0,7,0, 
        servo_frequency, servo_max, servo_min);

    Servo lid_servo1(lid_servo_pin1,0,1, 
        servo_frequency, servo_max, servo_min);

    Servo lid_servo2(lid_servo_pin2,1,1, 
        servo_frequency, servo_max, servo_min);

    Servo lid_servo3(lid_servo_pin3,2,1, 
        servo_frequency, servo_max, servo_min);

    Servo lid_servo4(lid_servo_pin4,3,1, 
        servo_frequency, servo_max, servo_min);

    Servo lid_servo5(lid_servo_pin5,4,1, 
        servo_frequency, servo_max, servo_min);

    Servo lid_servo6(lid_servo_pin6,5,1, 
        servo_frequency, servo_max, servo_min);

extern "C" void app_main()
{
    
    Serial.begin(115200);
    initArduino();
    initServer(&server, &params);
    for(int x = 0; x <= 6; x++)
    {
    	resetString(x);
    }
    xQueueISR = xQueueCreate(10, sizeof(int));// queue to pass interupt id
    xQueueTerminateTask = xQueueCreate(1, sizeof(int));
    xQueueServoID = xQueueCreate(3, sizeof(int));
    xQueueServoAngle = xQueueCreate(3, sizeof(int));
    xQueueServoType = xQueueCreate(3, sizeof(int));
   // sealPODS(0, false);
    //xTaskCreate(vGygerTask, "gyger0 data", 4060, (void*)0, 1, NULL);
    //xTaskCreate(vLidTask, "toggle lid" , 4060, NULL, 2, NULL);

	

}
