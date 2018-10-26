#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_err.h"
#include "Motor.hpp"
#include <driver/gpio.h>

#define MOTOR_HS_MODE           LEDC_HIGH_SPEED_MODE
#define MOTOR_HS_TIMER          LEDC_TIMER_0

const float limit = .70;

Motor :: Motor(const uint32_t gpio_pin,const uint32_t frequency, const uint32_t channel,const uint32_t max_rpm, const uint32_t direction, const uint32_t brake) 
{
  MOTOR_FREQUENCY = frequency;
  MOTOR_CHANNEL = channel;
  MOTOR_GPIO_PIN = gpio_pin;
  MOTOR_MAX_RPM = max_rpm;
  MOTOR_DIRECTION_PIN = direction;
  MOTOR_BRAKE_PIN = brake;
  previous_direction = 0;
  current_direction = 0;
  Motor_gpio_config(MOTOR_DIRECTION_PIN);
  //Motor_gpio_config(MOTOR_BRAKE_PIN);
  Motor_brake_config(MOTOR_BRAKE_PIN);
  Motor_timer_config(MOTOR_FREQUENCY);
  Motor_channel_config(MOTOR_GPIO_PIN, MOTOR_CHANNEL);
}

void Motor :: Motor_gpio_config(const uint32_t gpio_pin)
{
    printf("pin %i \n", gpio_pin );
    gpio_config_t pin_config;
    pin_config.pin_bit_mask = 1ULL << gpio_pin;
    pin_config.mode = GPIO_MODE_OUTPUT_OD;
    pin_config.pull_up_en = GPIO_PULLUP_DISABLE;
    pin_config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    pin_config.intr_type =  GPIO_INTR_DISABLE;
    gpio_config(&pin_config);
}


void Motor :: Motor_brake_config(const uint32_t gpio_pin)
{
    printf("pin %i \n", gpio_pin );
    gpio_config_t pin_config;
    pin_config.pin_bit_mask = 1ULL << gpio_pin;
    pin_config.mode = GPIO_MODE_OUTPUT_OD;
    pin_config.pull_up_en = GPIO_PULLUP_DISABLE;
    pin_config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    pin_config.intr_type =  GPIO_INTR_DISABLE;
    gpio_config(&pin_config);
}


void Motor :: Motor_timer_config(const uint32_t frequency) 
{
  ledc_timer_config_t motor_timer;
  motor_timer.bit_num = LEDC_TIMER_10_BIT;
  motor_timer.freq_hz = frequency;
  motor_timer.speed_mode = MOTOR_HS_MODE;
  motor_timer.timer_num = MOTOR_HS_TIMER;
  ledc_timer_config(&motor_timer);
}

void Motor :: Motor_channel_config(const uint32_t gpio_pin, const uint32_t channel) 
{
  //printf("pin %i\n", gpio_pin);
  ledc_channel_config_t motor_channel; 
  motor_channel.channel = static_cast<ledc_channel_t>(channel);
  motor_channel.duty = 0;
  motor_channel.gpio_num = gpio_pin;
  motor_channel.speed_mode = MOTOR_HS_MODE;
  motor_channel.timer_sel = MOTOR_HS_TIMER;
  ledc_channel_config(&motor_channel);
}

void Motor :: Motor_set_rpm(int32_t percentage) 
{
  if(percentage < 0 )
  {
    percentage*= -1;
    Motor_set_direction(0); // backward
    current_direction = 0;
  }
  else if(percentage > 0)
  {
    Motor_set_direction(1);
    current_direction = 1;
  }
  else
  {
    Motor_set_direction(current_direction);
  }
  //current_direction = previous_direction;
  uint32_t bit_limit = float(1023)*limit;
  uint32_t duty = (float(percentage)/100)*(bit_limit); //1023 is max decimal number in 10 bit
  // float divide = float(target_rpm / MOTOR_MAX_RPM);
  // float result = divide*1023;
  // int duty = result;
  //printf("duty : %i , max_rpm : %i, motor channel: %i  \n", duty, MOTOR_MAX_RPM, MOTOR_CHANNEL);
  ledc_set_duty(MOTOR_HS_MODE, static_cast<ledc_channel_t>(MOTOR_CHANNEL), duty);
  ledc_update_duty(MOTOR_HS_MODE, static_cast<ledc_channel_t>(MOTOR_CHANNEL));
}

void Motor :: Motor_set_direction(uint8_t direction)
{
  int32_t debug;
  //gpio_set_direction(static_cast<gpio_num_t>(MOTOR_DIRECTION_PIN),GPIO_MODE_OUTPUT_OD);
  //gpio_set_level(static_cast<gpio_num_t>(MOTOR_DIRECTION_PIN),direction);
  //printf("Pin %i \n", MOTOR_DIRECTION_PIN);
  if(MOTOR_DIRECTION_PIN == 23 || MOTOR_DIRECTION_PIN == 16)
  {
    if (direction == 1 )
    {
      //printf("Backward\n");
      gpio_set_level(static_cast<gpio_num_t>(MOTOR_DIRECTION_PIN),0);
    }
    else
    {
      //printf("Forward\n");
      gpio_set_level(static_cast<gpio_num_t>(MOTOR_DIRECTION_PIN),1);
    }
  }
  else
  {
    //printf("Direction %i \n", direction);
    debug = gpio_set_level(static_cast<gpio_num_t>(MOTOR_DIRECTION_PIN),direction);
    //printf("debug %i \n",debug );
  }
}

void Motor :: Motor_set_brake(int8_t brake)
{
  gpio_set_level(static_cast<gpio_num_t>(MOTOR_BRAKE_PIN),brake);
}
