#include <stdlib.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/event_groups.h"
#include "freertos/queue.h"
#include "driver/ledc.h"
#include "esp_log.h"
#include "Arduino.h"
#include "Source.h"
#include "constants.h"
#include "RTOStasks.h"

// Create a queue so data can be sent between tasks.
QueueHandle_t Q = xQueueCreate(10, sizeof(double));
QueueHandle_t q_target_angle = xQueueCreate(10, sizeof(int8_t));
/**
 * This task controls the MPU6050. It begins by initializing the accelerometer.
 * In the loop, the current X angle (roll) is read from the accelerometer and is
 * printed then placed on the queue.
 */
extern "C" void vAccelerometerTask(void *pvParameters)
{
    double current_angle = 0;
    initAccelerometer();
    while(1)
    {
		current_angle = getAngle('X');
        printf("\nX = %f\n", current_angle);
		xQueueSend(Q, &current_angle, 0);
		vTaskDelay(100);
    }
}

/**
 * This task controls the operation of the servo. It begins by initializing
 * the pwm timer as well as creating a pwm channel. The process of how to do 
 * this is explained on page 306 of the Kolban ESP32 textbook. In the loop, the
 * task updates the duty cycle if there is an item on the queue.
 */
extern "C" void vServoTask(void *pvParameters)
{
    uint16_t servo_duty_cycle;
    double current_angle;
    int8_t target;

    ledc_timer_config_t servo_timer;
    servo_timer.duty_resolution = LEDC_TIMER_10_BIT;
    servo_timer.freq_hz = 50;
    servo_timer.speed_mode = LEDC_HIGH_SPEED_MODE;
    servo_timer.timer_num = LEDC_TIMER_0;
    ledc_timer_config(&servo_timer);


    ledc_channel_config_t servo;
    servo.channel = LEDC_CHANNEL_0;
    servo.duty = 1024;
    servo.gpio_num = 15;
    servo.intr_type = LEDC_INTR_DISABLE;
    servo.speed_mode = LEDC_HIGH_SPEED_MODE;
    servo.timer_sel = LEDC_TIMER_0;
    ledc_channel_config(&servo);

    while(1)
    {
        if (xQueueReceive(q_target_angle, &target, 0))
		{
            servo_duty_cycle = getDutyCycle(target);
            ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, servo_duty_cycle);
	    	ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
	    	vTaskDelay(100);
        }
    }
}

extern "C" void vUpdateTarget(void *pvParameters)
{
    int8_t target_degrees = 0;
    int8_t prev = target_degrees;
 
    while(1)
    {
        scanf("%hhd", &target_degrees);
        if(target_degrees > 88)		target_degrees = 88;
		if(target_degrees < -90)	target_degrees = -90;

        if(prev != target_degrees)
        {
            prev = target_degrees;
            printf("Target_degrees has been set to %i degrees \n", target_degrees);

            xQueueSend(q_target_angle, &target_degrees, 0);
        }   
        vTaskDelay(300);
    }   
}