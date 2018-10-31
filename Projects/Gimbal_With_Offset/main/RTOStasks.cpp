#include <stdlib.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/event_groups.h"
#include "driver/ledc.h" //!!!
#include "esp_log.h"
#include "Arduino.h"
#include "EEPROM.h"
#include "Source.h"
#include "constants.h"
#include "RTOStasks.h"  //!!!
//#include "Servo.h"


#include <iostream>
using namespace std;

unsigned int target;
int current;

void vUpdateTarget(void *pvParameters)
{
    int target_duty;
    while(1)
    {
        
        scanf("%i", &target);
        printf("Target has been set to %i degrees \n", target);

        target_duty =  degreesToDuty(target);
        if(target_duty < minDuty) target_duty = minDuty;
        if(target_duty > maxDuty) target_duty = maxDuty;

        printf("Target_Duty has been set to %i", target_duty);
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
    //Servo_t servo;

    ledc_timer_config_t myTimer;
    myTimer.duty_resolution = LEDC_TIMER_10_BIT;
    myTimer.speed_mode      = LEDC_HIGH_SPEED_MODE;
    myTimer.timer_num       = LEDC_TIMER_0;
    myTimer.freq_hz         = servoFreq;   //change this for our servo
    ledc_timer_config(&myTimer);

    ledc_channel_config_t myServo;
    myServo.channel     = LEDC_CHANNEL_0;
    myServo.duty        = 1024; //?????
    myServo.gpio_num    = gpio;
    myServo.intr_type   = LEDC_INTR_DISABLE;
    myServo.speed_mode  = LEDC_HIGH_SPEED_MODE;
    myServo.timer_sel   = LEDC_TIMER_0;
    ledc_channel_config(&myServo);


    while(1)
    {

        //logic to update duty cycle goes here
        //Period = 1/myTimer.freq_hz
        //Look at min pulse time and max pulse time for our servo
        //find the min/max time as a percent of Period
        //Use that percent to find the duty limit with our 10bit freq, 
        //  0 <= minPercent*1024 <= new_Duty <= maxPercent*1024 < 1024

        ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0,/* new_Duty */);
        ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);

    }   
}
