#ifndef SERVO_H
#define SERVO_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_err.h"
#include "Servo.hpp"
#include <driver/gpio.h>

class Servo
{
     private :
          uint32_t SERVO_GPIO_PIN;
          uint32_t SERVO_FREQUENCY;           //Servo frequency
          uint32_t SERVO_CHANNEL;             //Servo Timer channel
          uint32_t SERVO_MIN_PULSEWIDTH;      //Servo Pin 
          uint32_t SERVO_MAX_PULSEWIDTH;
          uint32_t SERVO_MAX_DEGREE;
          uint32_t SERVO_CURRENT_PULSEWIDTH;

     public :
          Servo(const uint32_t gpio_pin, const uint32_t frequency, const uint32_t channel,
                const uint32_t max_pulse, const uint32_t min_pulse,const uint32_t max_degree);
          void Servo_gpio_config(const int gpio_pin);
          void Servo_timer_config(const int frequency);
          void Servo_channel_config(const int gpio_pin, const int channel);
          int  Servo_write_angle(uint32_t set_angle);
          uint32_t Servo_per_degree_init(uint32_t degree_of_rotation);
};

#endif