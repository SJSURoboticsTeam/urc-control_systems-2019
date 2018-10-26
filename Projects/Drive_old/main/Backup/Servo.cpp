#include "driver/mcpwm.h"
#include "soc/mcpwm_reg.h"
#include "soc/mcpwm_struct.h"
#include <iostream>
#include "Servo.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/event_groups.h"


using namespace std;

Servo :: Servo(const uint32_t gpio_pin, const uint32_t frequency, const uint32_t channel, const uint32_t max_pulse, const uint32_t min_pulse,const uint32_t max_degree)
{
    SERVO_MIN_PULSEWIDTH=min_pulse; //Minimum pulse width in microsecond
    SERVO_MAX_PULSEWIDTH=max_pulse; //Maximum pulse width in microsecond
    SERVO_MAX_DEGREE=max_degree; //Maximum angle in degree upto which servo can rotate
    SERVO_CHANNEL = channel;
    CURRENT_FREQUENCY = frequency;
    GPIO_PIN = gpio_pin;
    Servo_gpio_config();
    Servo_write_angle(90);
    vTaskDelay(10);
}

int Servo :: Servo_gpio_config()
{
    char pin_version[2] = {'A','B'};
    int error_flag;
    error_flag=mcpwm_gpio_init(MCPWM_UNIT_0, static_cast<mcpwm_io_signals_t>(SERVO_CHANNEL), GPIO_PIN);
    if(error_flag == ESP_OK)
    {
      printf("IT HIT \n");
      error_flag=Servo_pwm_config(CURRENT_FREQUENCY,0,(SERVO_CHANNEL/2),pin_version[SERVO_CHANNEL%2]);
    }
    return error_flag;
}
int Servo :: Servo_pwm_config(const int frequency, const int init_duty_cycle, const int channel_timer, const char pin_version)
{
    int error_flag= -2;
    mcpwm_config_t pwm_config;
    pwm_config.frequency = frequency;		//frequency = 50Hz, i.e. for every servo motor time period 	should be 20ms
    if(pin_version == 'A')
    {
        printf("A %i \n", GPIO_PIN);
        pwm_config.cmpr_a = init_duty_cycle;     //duty cycle of PWMxA = 0
        error_flag = 1; 
    }
    if(pin_version == 'B'){
        printf("B %i \n", GPIO_PIN);
        pwm_config.cmpr_b = init_duty_cycle;     //duty cycle of PWMxb = 0
        error_flag = 1;
    }
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
    if( channel_timer == 0 || channel_timer == 1 || channel_timer == 2 )
    {
        printf("CHANNEL TIMER %i\n", channel_timer);
	    mcpwm_init(MCPWM_UNIT_0,static_cast<mcpwm_timer_t>(channel_timer), &pwm_config);
    }
    else
    {
      error_flag=-2;
    }
    return error_flag;
}

int Servo :: Servo_write_angle(uint32_t set_angle)
{
    int error_flag;
    //printf("set angle : %i\n", set_angle);
    CURRENT_PULSEWIDTH = Servo_per_degree_init(set_angle);
    //printf("pulse width %i\n", CURRENT_PULSEWIDTH);
    // error_flag = mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, CURRENT_PULSEWIDTH);
    if(SERVO_CHANNEL%2 == 0 )
    {
        error_flag=mcpwm_set_duty_in_us(MCPWM_UNIT_0, static_cast<mcpwm_timer_t>(SERVO_CHANNEL/2), MCPWM_OPR_A, CURRENT_PULSEWIDTH); 
    }
    else
    {
        error_flag=mcpwm_set_duty_in_us(MCPWM_UNIT_0, static_cast<mcpwm_timer_t>(SERVO_CHANNEL/2), MCPWM_OPR_B, CURRENT_PULSEWIDTH);
    }
    return error_flag;
}

uint32_t Servo :: Servo_per_degree_init(uint32_t degree_of_rotation)
{
    uint32_t cal_pulsewidth = 0;
    cal_pulsewidth = (SERVO_MIN_PULSEWIDTH + (((SERVO_MAX_PULSEWIDTH - SERVO_MIN_PULSEWIDTH) * (degree_of_rotation)) / (SERVO_MAX_DEGREE)));
    return cal_pulsewidth;
}
