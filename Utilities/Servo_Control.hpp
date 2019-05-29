////////////////////////////////////////////////////////////////////////////////                        
//                                                                            //
//                                Servo_Control.h                             //
//                            Version 1.0 - 10/24/2018                        //
//                                                                            //
// By:                            Colin Schardt, Control Systems Lead         //
// Organization:                  SJSU Robotics 2019                          //
//                                                                            //
// This library provides functionality for both positional and continuous     //
// servos to interface with the esp32 microcontroller. This library utilizes  //
// ledc driver in the esp-idf as a source for PWM control. Please reference   //
// function descriptions for usage.                                           //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include "driver/ledc.h"

#ifndef SERVOCONTROL_H_
#define SERVOCONTROL_H_

class Servo
{
    private:
	ledc_channel_t pwm_channel;
	ledc_timer_t pwm_timer;
	uint32_t gpio_pin;
	uint32_t pwm_frequency;
	uint32_t duty_cycle;
	uint32_t max_duty = ((1 << LEDC_TIMER_10_BIT) - 1);
	uint32_t top_duty;
	uint32_t base_duty;
        uint32_t range;	
	ledc_timer_config_t servo_timer;
	ledc_channel_config_t servo;

    public:
        /**
         * Parameters:
         *            pin       - The GPIO pin the servo has been assigned to.
         *            channel   - Value of 0 - 7, must be unique for each servo.
         *            timer     - Value of 0 - 3, determines which set frequency 
         *                        the servo will use.
         *            frequency - The frequency of the the PWM in Hz as 
         *                        specified by the servo manufacturer.
         *            max       - The maximum percentage duty cycle as specified 
         *                        by the servo manufacturer.
         *            min       - The minimum percentage duty cycle as specified
         *                        by the servo manufacturer.
         */
        Servo(uint32_t pin, uint32_t channel, uint32_t timer, uint32_t frequency, 
              float max, float min);
        
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
        void SetPositionPercent(double percentage);

        /**
         * This function updates the duty_cycle variable of the Servo class
         * object and updates the esp32's registers to implement this duty cycle 
         * in the pwm output. duty should reference the value at which the timer
	 * sets the signal low.
         */
        void SetPositionDuty(uint32_t duty);
        /**
         * This function calculates the percentage of the range of the servo a 
         * desired angle represents compared to the maximum rated rotation of 
         * the servo.
         */
        double GetPercentage(uint32_t max_rotation, double angle);

	/**
         * deconstructor
         */
	~Servo() {}
};

class ServoMotor
{
    private:        
        ledc_channel_t pwm_channel;
        ledc_timer_t pwm_timer;
        uint32_t gpio_pin;
        uint32_t pwm_frequency;
        uint32_t duty_cycle;
	uint32_t max_duty = ((1 << LEDC_TIMER_10_BIT) -1);
        uint32_t full_forward;
        uint32_t full_reverse;
	uint32_t stop_minimum;;
	uint32_t stop_maximum;
	uint32_t stop_range;
	uint32_t upper_range;
	uint32_t lower_range;
	bool direction;
        ledc_timer_config_t motor_timer;
        ledc_channel_config_t motor;

    public:

        /**
         * Parameters:
         *            pin       - The GPIO pin the motor has been assigned to.
         *            channel   - Value of 0 - 7, must be unique for each motor.
         *            timer     - Value of 0 - 3, determines which set frequency 
         *                        the motor will use.
         *            frequency - The frequency of the the PWM in Hz as 
         *                        specified by the motor manufacturer.
         *            max       - The maximum percentage duty cycle as specified 
         *                        by the motor manufacturer.
         *            min       - The minimum percentage duty cycle as specified
         *                        by the motor manufacturer.
         *            dead_min  - The begining percentage of the range where the
         *                        motor will not move, as specified by the 
         *                        manufacturer.
         *            dead_max  - The ending percentage of the range where the
         *                        motor will not move, as specified by the
         *                        manufacturer.
         */
        ServoMotor(uint32_t pin, uint32_t channel, uint32_t timer, uint32_t frequency, 
              float max, float min, float dead_min, float dead_max);

        /**
         * This function sets the frequency of the PWM in hertz by updating the 
         * timer that has been assigned to the signal. There are only four timers 
         * available, so setting the frequency here will update the frequency 
         * for all objects assigned to the same timer.
         */
        void SetFrequency(uint32_t frequency);

        /**
         * This function calculates and updates the speed in the current 
         * direction the motor is rotating, based upon the input percentage. It 
         * takes into accoun the 'dead zone' in which the motor will not move as
         * well as the upper and lower duty cycle limits as specified by the 
         * manufacturer.
         */
        void SetSpeed(double percentage);

        /**
         * This function calculates and updates the speed and direction the
         * motor rotates, based upon the input percentage and direction. It
         * takes into accoun the 'dead zone' in which the motor will not move as
         * well as the upper and lower duty cycle limits as specified by the 
         * manufacturer.
         */
        void SetSpeedAndDirection(double percentage, bool dir);

        /**
         * This function updates the speed the motor spins at based upon the 
         * duty cycle value input. This value corresponds to the count at which 
         * the pwm timer will turn off the pwm signal. After the duty cycle has 
         * been set, this function determines the direction the motor is 
         * spinning and updates the direction variable accordingly.	
         */
        void SetSpeedDuty(uint32_t duty);

        /**
         * This function updates the direction variable, and calculates and 
         * implements a duty cycle that corresponds to the current one, but in 
         * the opposite direction.
         */
        void SetDirection(bool direction);
         
        /**
         * Deconstructor
         */
	~ServoMotor() {}
};
 
#endif

