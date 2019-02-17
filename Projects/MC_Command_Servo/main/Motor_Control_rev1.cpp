#include <stdlib.h>
#include "Arduino.h"
#include "driver/ledc.h"
#include "driver/gpio.h"
#include "Motor_Control_rev1.hpp"

void Motor::InitMotor(uint32_t pin_signal, uint32_t pin_brake, 
                      uint32_t pin_direction, uint32_t s_channel,
                      uint32_t timer, uint32_t frequency, float min, float max)
{
    // Initialize Private variables
    signal_pin = pin_signal;
    brake_pin = pin_brake;
    direction_pin = pin_direction;
    signal_duty_cycle = 0;
    brake_duty_cycle = 0;
    pwm_frequency = frequency;

    top_duty = max_duty * max / 100;
    base_duty = max_duty * min / 100;
    range = top_duty - base_duty;
    pwm_timer = (ledc_timer_t) timer;
    pwm_s_channel = (ledc_channel_t) s_channel;

    // Initialize PWM Timer //
    motor_timer.duty_resolution = LEDC_TIMER_10_BIT;
    motor_timer.freq_hz = pwm_frequency;
    motor_timer.speed_mode = LEDC_HIGH_SPEED_MODE;
    motor_timer.timer_num = pwm_timer;
    ledc_timer_config(&motor_timer);

    // Initialize PWM Signal Channel //
    motor.channel = pwm_s_channel;
    motor.duty = 0;
    motor.gpio_num = signal_pin;
    motor.intr_type = LEDC_INTR_DISABLE;
    motor.speed_mode = LEDC_HIGH_SPEED_MODE;
    motor.timer_sel = pwm_timer;
    ledc_channel_config(&motor);

    /*
    // Initialize PWM Brake Channel //
    brake.channel = pwm_b_channel;
    brake.duty = 0;
    brake.gpio_num = brake_pin;
    brake.intr_type = LEDC_INTR_DISABLE;
    brake.speed_mode = LEDC_HIGH_SPEED_MODE;
    brake.timer_sel = pwm_timer;
    ledc_channel_config(&brake);
    */
    pinMode(brake_pin, OUTPUT);

    // Initialiaze Direction Pin //
    pinMode(direction_pin, OUTPUT);
}

void Motor::SetFrequency(uint32_t frequency)
{
    // Set the frequency of the PWM signal //
    pwm_frequency = frequency;
    ledc_set_freq(LEDC_HIGH_SPEED_MODE, pwm_timer, pwm_frequency);
}

void Motor::SetSpeed(double percentage)
{
    // Calculates the duty cycle based upon the input percentage //
    signal_duty_cycle = base_duty + (range * percentage / 100);
    
    // Safety net in case the percentage exceeds the boundaries //
    if (signal_duty_cycle > top_duty)
    {
        signal_duty_cycle = top_duty;
    }
    else if (signal_duty_cycle < base_duty)
    {
        signal_duty_cycle = base_duty;
    }

    // Set the Duty Cycle //
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, pwm_s_channel, signal_duty_cycle);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, pwm_s_channel);
    printf("Speed set to %f percent of total range.\n", percentage);
}

void Motor::SetSpeedAndDirection(double percentage, bool direction)
{
    // Calculates the duty cycle based upon the input percentage //
    signal_duty_cycle = base_duty + (range * percentage / 100);
    
    // Safety net in case the percentage exceeds the boundaries //
    if (signal_duty_cycle > top_duty)
    {
        signal_duty_cycle = top_duty;
    }
    else if (signal_duty_cycle < base_duty)
    {
        signal_duty_cycle = base_duty;
    }

    // Set the Duty Cycle //
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, pwm_s_channel, signal_duty_cycle);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, pwm_s_channel);
    // printf("Speed set to %f percent of total range.\n", percentage);

    // Set the Direction //
    SetDirection(direction);
}

void Motor::SetSpeedDuty(uint32_t duty)
{
    signal_duty_cycle = duty;
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, pwm_s_channel, signal_duty_cycle);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, pwm_s_channel);
}

void Motor::SetDirection(bool dir)
{
    //gpio_set_level((gpio_num_t) direction_pin, dir);
    digitalWrite(direction_pin, dir);
    //printf("Signal on pin %d set to %d.\n", direction_pin, gpio_get_level((gpio_num_t) direction_pin));
    }
    
    void Motor::Brake(bool brake)
{
    /*
    // Safety net in case the percentage exceeds the boundaries //
    if (percentage > 100)
    {
        brake_duty_cycle = 100;
    }
    else if (percentage < 0)
    {
        brake_duty_cycle = 0;
    }
    else
    {
        brake_duty_cycle = percentage;
    }

    // Set the Duty Cycle //
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, pwm_b_channel, brake_duty_cycle);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, pwm_b_channel);
    */
    digitalWrite(brake_pin, brake);
    // printf("Brake set to %f percent.\n", percentage);
}