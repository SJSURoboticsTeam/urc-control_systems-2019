#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
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

#include "Wire.h"


#include <iostream>
using namespace std;


QueueHandle_t q_target_angle = xQueueCreate(10, sizeof(int8_t));
QueueHandle_t q_actual_angle = xQueueCreate(10, sizeof(double));



//Reads from "Mission Control" (terminal) to get the target angle
//that the servo should be pointing at

//0 degrees is when the servo is pointing horizontal to the right
//90 degrees is when ther servo is pointing directly up
//-90 degrees is when ther servo is pointing directly down
void vUpdateTarget(void *pvParameters)
{
    int8_t target_degrees = 0;
    int8_t prev = target_degrees;
 
    while(1)
    {
        scanf("%hhd", &target_degrees);
        if(prev != target_degrees)
        {
            prev = target_degrees;
            printf("Target_degrees has been set to %i degrees \n", target_degrees);

            xQueueSend(q_target_angle, &target_degrees, 0);
        }   
        vTaskDelay(300);
    }   
}


//SDA pin is GPIO 21
//SCL pin is GPIO 22
//Reads from the MPU 6050 and calculates its pitch
//Where,
//0 degrees is when the servo is pointing horizontal to the right
//90 degrees is when ther servo is pointing directly up
//-90 degrees is when ther servo is pointing directly down

void vIMUTask(void *pvPArameters)
{
    double pitch = 0;
    Wire.begin();
    Wire.beginTransmission(0x68);
    Wire.write(0x6B);
    Wire.write(0);
    Wire.endTransmission(true);

    while(1)
    {
        pitch = readPitch();
        printf("%f \n\n", pitch);
        xQueueSend(q_actual_angle, &pitch, 0);

        vTaskDelay(100);
    }   
}


//This task is sent the current angle from vIMUTask
//And target angle from vUpdateTarget
//it calculates the difference in those angle, and uses that difference
//to determine how much the current Duty Cycle needs to be altered
void vServoTask(void *pvParameters)
{
    
    ledc_timer_config_t myTimer;
    myTimer.duty_resolution = LEDC_TIMER_15_BIT;
    myTimer.speed_mode      = LEDC_HIGH_SPEED_MODE;
    myTimer.timer_num       = LEDC_TIMER_0;
    myTimer.freq_hz         = servoFreq;
    ledc_timer_config(&myTimer);

    ledc_channel_config_t myServo;
    myServo.channel     = LEDC_CHANNEL_0;
    myServo.duty        = middleDuty;
    myServo.gpio_num    = servo_gpio_pin;
    myServo.intr_type   = LEDC_INTR_DISABLE;
    myServo.speed_mode  = LEDC_HIGH_SPEED_MODE;
    myServo.timer_sel   = LEDC_TIMER_0;
    ledc_channel_config(&myServo);

    uint32_t dutyCycle = middleDuty; 
    int32_t changeOfDuty;
    int8_t targetAngle = 0;
    double degreesReceived;

    while(1)
    {
        if(xQueueReceive(q_actual_angle, &degreesReceived, portMAX_DELAY))
        {
            xQueueReceive(q_target_angle, &targetAngle, 0);

            changeOfDuty = calcDutyOffset(targetAngle,degreesReceived);
            dutyCycle += changeOfDuty;

            if      (dutyCycle < minDuty) dutyCycle = minDuty;
            else if (dutyCycle > maxDuty) dutyCycle = maxDuty;



            printf("dutyCycle has been set to %i", dutyCycle);

            ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, dutyCycle);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
        }
    }   
}

