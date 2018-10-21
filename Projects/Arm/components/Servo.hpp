#include <stdlib.h>
#include "driver.ledc.h"

#ifndef SERVO_H_
#define SERVO_H_

class Servo
{
    private:
        gpio_num_t gpio_pin;
	led_channel_t pwm_channel;
	led_timer_t pwm_timer;
	uint32_t pwm_frequency;
	uint32_t duty_cycle;
        uint32_t upper_limit;
	uint32_t lower_limit;
	ledc_timer_config_t servo_timer;
	ledc_channel_config_t servo;

    public:
        /**
         * Parameters:
	 *            pin     - The GPIO pin the servo has been assigned to.
         *            channel - Value of 0 - 7, must be unique for each servo.
         *            timer   - Value of 0 - 3, determines which set frequency 
         *                      the servo will use.
         *            max     - The maximum percentage duty cycle as specified 
         *                      by the servo manufacturer.
         *            min     - The minimum percentage duty cycle as specified
         *                      by the servo manufacturer.
         */
        Servo(uint32_t pin, uint32_t channel, uint32_t timer, uint32_t max, 
              uint32_t min);
        
        /**
         * This function sets the frequency of the PWM in hertz by updating the 
         * timer that has been assigned to the signal. There are only four timers 
         * available, so setting the frequency here will update the frequency 
         * for all objects assigned to the same timer.
         */
        void SetFrequency(uint32_t frequency);
        
	/**
         * This function calculates and implements the proper pwm signal based 
         * upon the range percentage input to this function and the maximum and 
         * minimum duty cycle percentages specified in the constructor.
         */ 
        void SetPosition(uint32_t percentage);
	
	/**
         * deconstructor
         */
	~Servo();
}
 
#endif

