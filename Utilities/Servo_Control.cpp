////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                             Servo_Control.cpp                              //
//                          Version 1.0 - 10/24/2018                          //
//                                                                            //
// By:                         Colin Schardt, Control Systems Lead            //
// Organization:               SJSU Robotics 2019                             //
//                                                                            //
// Please reference ServoControl.hpp for function descriptions and usage.     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include "driver/ledc.h"
#include "Servo_Control.hpp"

Servo::Servo(uint32_t pin, uint32_t channel, uint32_t timer, uint32_t frequency,
             float max, float min)
{
    // Intitialize Static Variables //
    gpio_pin = pin;
    pwm_frequency = frequency;
    duty_cycle = 0;
    top_duty = max_duty * max / 100;
    base_duty = max_duty * min / 100;
    range = top_duty - base_duty;
    pwm_timer = (ledc_timer_t) timer;
    pwm_channel = (ledc_channel_t) channel;

    // Initialize PWM Timer //
    servo_timer.duty_resolution = LEDC_TIMER_10_BIT;
    servo_timer.freq_hz = pwm_frequency;
    servo_timer.speed_mode = LEDC_HIGH_SPEED_MODE;
    servo_timer.timer_num = pwm_timer;
    ledc_timer_config(&servo_timer);

    // Initialize PWM Channel //
    servo.channel = pwm_channel;
    servo.duty = 0;
    servo.gpio_num = gpio_pin;
    servo.intr_type = LEDC_INTR_DISABLE;
    servo.speed_mode = LEDC_HIGH_SPEED_MODE;
    servo.timer_sel = pwm_timer;
    ledc_channel_config(&servo);
}

void Servo::SetFrequency(uint32_t frequency)
{
    // Set the frequency of the PWM signal //
    pwm_frequency = frequency;
    ledc_set_freq(LEDC_HIGH_SPEED_MODE, pwm_timer, pwm_frequency);
}

void Servo::SetPositionPercent(double percentage)
{
    // Calculates the duty cycle based upon the input percentage //
    duty_cycle = base_duty + (range * percentage / 100);
    
    // Safety net in case the percentage exceeds the boundaries //
    if (duty_cycle > top_duty)
    {
        duty_cycle = top_duty;
    }
    else if (duty_cycle < base_duty)
    {
        duty_cycle = base_duty;
    }

    // Set the Duty Cycle //
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, pwm_channel, duty_cycle);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, pwm_channel);
}

void Servo::SetPositionDuty(uint32_t duty)
{
    //  Sets the Duty Cycle to the input value //
    duty_cycle = duty;
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, pwm_channel, duty_cycle);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, pwm_channel);
}

double Servo::GetPercentage(uint32_t max_rotation, double angle)
{
    // Safety Net in case desired angle is out of bounds //
    double percentage = 0;
    if (angle >= max_rotation)
    {
        percentage = 100;
    }
    else if (angle <= 0)
    {
        percentage = 0;
    }
    else
    {
        // Calculates percentage of angle in reference to allowed range //
        percentage = 100 * angle / max_rotation;
    }
    return percentage;
}

ServoMotor::ServoMotor(uint32_t pin, uint32_t channel, uint32_t timer, uint32_t frequency,
             float max, float min, float dead_min, float dead_max)
{
    // Define Statics //
    gpio_pin = pin;
    pwm_frequency = frequency;
    duty_cycle = 0;
    full_forward = max_duty * max / 100;
    full_reverse = max_duty * min / 100;
    stop_minimum = max_duty * dead_min / 100;
    stop_maximum = max_duty * dead_max / 100;
    stop_range = stop_maximum - stop_minimum;
    lower_range = stop_minimum - full_reverse;
    upper_range = full_forward - stop_maximum;
    
    direction = 0;
    pwm_timer = (ledc_timer_t) timer;
    pwm_channel = (ledc_channel_t) channel;

    //  Initialize PWM Timer //
    motor_timer.duty_resolution = LEDC_TIMER_10_BIT;
    motor_timer.freq_hz = pwm_frequency;
    motor_timer.speed_mode = LEDC_HIGH_SPEED_MODE;
    motor_timer.timer_num = pwm_timer;
    ledc_timer_config(&motor_timer);

    // Initialize PWM Channel //
    motor.channel = pwm_channel;
    motor.duty = stop_maximum;
    motor.gpio_num = gpio_pin;
    motor.intr_type = LEDC_INTR_DISABLE;
    motor.speed_mode = LEDC_HIGH_SPEED_MODE;
    motor.timer_sel = pwm_timer;
    ledc_channel_config(&motor);
    
    // Set Duty Cycle to within 'dead zone' bounds //
    duty_cycle = stop_maximum;
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, pwm_channel, duty_cycle);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, pwm_channel);
}

void ServoMotor::SetFrequency(uint32_t frequency)
{
    // Set the frequency of the PWM signal //
    pwm_frequency = frequency;
    ledc_set_freq(LEDC_HIGH_SPEED_MODE, pwm_timer, pwm_frequency);
}

void ServoMotor::SetSpeed(double percentage)
{
    // Saftey net if percentage is out of bounds //
    if (percentage > 100)
    {
        percentage = 100;
    }
    else if (percentage < 0)
    {
        percentage = 0;
    }
    
    // calculates duty cycle count value based on direction //
    if(!direction)
    {
        duty_cycle = stop_minimum - (lower_range * percentage / 100);
    }
    else
    {
        duty_cycle = stop_maximum + (upper_range * percentage / 100);
    }

    // Sets duty cycle to calculated value //
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, pwm_channel, duty_cycle);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, pwm_channel);
}

void ServoMotor::SetSpeedAndDirection(double percentage, bool dir)
{
    // sets direction then calls SetSpeed() function //	
    direction = dir;
    this->SetSpeed(percentage);
}

void ServoMotor::SetSpeedDuty(uint32_t duty)
{
    // Sets the duty cycle to input value and determines the new direction //
    duty_cycle = duty;
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, pwm_channel, duty_cycle);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, pwm_channel);
    if(duty_cycle < stop_minimum)
    {
        direction = 0;
    }
    else
    {
        direction = 1;
    }
}
     

void ServoMotor::SetDirection(bool dir)
{
    // If direction is unchanged, do nothing //
    if (direction != dir)
    {
        /**
         * If direction has changed, calculate new dudty cycle, taking into
         * account the dead-zone range. This is done by subtracting the current 
         * duty value plus the dead zone range from the top of the duty cycle
         * range, and then adding that value to the base duty cycle range.
         */
        duty_cycle = full_reverse + (full_forward - (duty_cycle + stop_range));
        ledc_set_duty(LEDC_HIGH_SPEED_MODE, pwm_channel, duty_cycle);
        ledc_update_duty(LEDC_HIGH_SPEED_MODE, pwm_channel);
	direction = !direction;
    }
}
