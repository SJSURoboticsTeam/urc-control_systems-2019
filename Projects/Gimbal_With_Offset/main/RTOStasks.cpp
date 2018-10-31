#include <stdlib.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/event_groups.h"
#include "freertos/queue.h"
#include "driver/ledc.h"
#include "esp_log.h"
#include "Arduino.h"
#include "EEPROM.h"
#include "Source.h"
#include "constants.h"
#include "RTOStasks.h"


#include <iostream>
using namespace std;

unsigned int target_degrees;
int current;

QueueHandle_t q = xQueueCreate(10, sizeof(int));

void vUpdateTarget(void *pvParameters)
{
    int prev = target_degrees;
 
    while(1)
    {
        scanf("%i", &target_degrees);
        if(prev != target_degrees)
        {
            prev = target_degrees;
            printf("Target_degrees has been set to %i degrees \n", target_degrees);

            xQueueSend(q, &target_degrees, 0);
        }   
        vTaskDelay(300);
    }   
}

void vIMUTask(void *pvPArameters)
{

    while(1)
    {

    }   
}

void vServoTask(void *pvParameters)
{
    
    ledc_timer_config_t myTimer;
    myTimer.duty_resolution = LEDC_TIMER_10_BIT;
    myTimer.speed_mode      = LEDC_HIGH_SPEED_MODE;
    myTimer.timer_num       = LEDC_TIMER_0;
    myTimer.freq_hz         = servoFreq;   //change this for our servo
    ledc_timer_config(&myTimer);

    ledc_channel_config_t myServo;
    myServo.channel     = LEDC_CHANNEL_0;
    myServo.duty        = middle;
    myServo.gpio_num    = gpio;
    myServo.intr_type   = LEDC_INTR_DISABLE;
    myServo.speed_mode  = LEDC_HIGH_SPEED_MODE;
    myServo.timer_sel   = LEDC_TIMER_0;
    ledc_channel_config(&myServo);

    int degreesReceived, newDuty;


    while(1)
    {

        if(xQueueReceive(q, &degreesReceived, portMAX_DELAY))
        {
            newDuty =  degreesToDuty(degreesReceived);
            if(newDuty < minDuty) newDuty = minDuty;
            if(newDuty > maxDuty) newDuty = maxDuty;

            printf("newDuty has been set to %i", newDuty);

            ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, newDuty);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
        }
    }   
}
