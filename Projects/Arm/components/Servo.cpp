#include <stdlib.h>
#include "driver/ledc.h"

Servo::Servo(uint32_t pin, uint32_t channel, uint32_t timer, uint32_t max, 
             uint32_t min)
{
    gpio_pin = pin;
    pwm_channel = channel;
    pwm_frequency = 0;
    duty_cycle = 0;
    upper_limit = max;
    lower_limit = min;

    switch(timer)
    {
        case 0: 
            pwm_timer = LEDC_TIMER_0;
            break;
        case 1:
            pwm_timer = LEDC_TIMER_1;
            break;
        case 2:
            pwm_timer = LEDC_TIMER_2;
            break;
        case 3:
            pwm_timer = LEDC_TIMER_3;
            break;
        default:
            pwm_timer = LEDC_TIMER_0;
	    break;
    }

    switch(channel)
    {
        case 0:
            pwm_channel = LEDC_CHANNEL_0;
	    break;
	case 1:
	    pwm_channel = LEDC_CHANNEL_1;
	    break;
	case 2:
	    pwm_channel = LEDC_CHANNEL_2;
	    break;
	case 3:
	    pwm_channel = LEDC_CHANNEL_3;
	    break;
	case 4:
	    pwm_channel = LEDC_CHANNEL_4;
	    break;
	case 5:
	    pwm_channel = LEDC_CHANNEL_5;
	    break;
	case 6:
	    pwm_channel = LEDC_CHANNEL_6;
	    break;
	case 7:
	    pwm_channel = LEDC_CHANNEL_7;
	    break;
	default:
	    pwm_channel = LEDC_CHANNEL_0;
	    break;
    }
    	
    servo_timer.duty_resolution = LEDC_TIMER_10_BIT;
    servo_timer.freq_hz = pwm_frequency;
    servo_timer.speed_mode = LEDC_HIGH_SPEED_MODE;
    servo_timer.timer_num = pwm_timer;
    ledc_timer_config(&servo_timer);


    servo.channel = pwm_channel;
    servo.duty = 0;
    servo.gpio_num = gpio_pin;
    servo.intr_type = LEDC_INTR_DISABLE;
    servo.speed_mode = LEDC_HIGH_SPEED_MODE;
    servo.timer_sel = pwm_timer;
    ledc_channel_config(&servo);
}

Servo::SetFrequency(uint32_t frequency)
{
    pwm_frequency = frequency;
    ledc_set_freq(LEDC_HIGH_SPEED_MODE, pwm_timer, pwm_frequency);
}

Servo::SetPosition(uint32_t percentage)
{
    uint16_t max_duty = ((1 << LEDC_TIMER_10_BIT) - 1);
    uint16_t base_duty = max_duty * lower_limit / 100;
    uint16_t top_duty  = max_duty * upper_limit / 100;
    uint16_t range = top_duty - base_duty;
    
    duty_cycle = base_duty + (range * percentage / 100);
    
    if (duty_cycle > top_duty)
    {
        duty_cycle = top_duty;
    }

    ledc_set_duty(LEDC_HIGH_SPEED_MODE, pwm_channel, duty_cycle);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, pwm_channel);
}

Servo::~Servo()
{
    delete[] servo;
    delete[] servo_timer;
}
