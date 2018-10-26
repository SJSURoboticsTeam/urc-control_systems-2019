#ifndef MOTOR_H
#define MOTOR_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_err.h"
#include "Motor.hpp"
#include <driver/gpio.h>

class Motor
{
     private :
          uint32_t MOTOR_MAX_RPM;
          uint32_t MOTOR_FREQUENCY;         //Servo frequency
          uint32_t MOTOR_CHANNEL;             //Servo Timer channel
          uint32_t MOTOR_GPIO_PIN;                  //Servo Pin 
          uint32_t MOTOR_DIRECTION_PIN;
          uint32_t MOTOR_BRAKE_PIN;
          uint8_t previous_direction;
          uint8_t current_direction;
     public :
          Motor(const uint32_t gpio_pin, const uint32_t frequency, const uint32_t channel,const uint32_t max_rpm, const uint32_t direction, const uint32_t brake);
          void Motor_timer_config(const uint32_t frequency);
          void Motor_channel_config(const uint32_t gpio_pin, const uint32_t channel);
          void Motor_gpio_config(const uint32_t gpio_pin);
          void Motor_set_rpm(int32_t percentage);
          void Motor_set_direction(uint8_t direction);
          void Motor_set_brake(int8_t brake);
          void Motor_brake_config(const uint32_t gpio_pin);
};

#endif