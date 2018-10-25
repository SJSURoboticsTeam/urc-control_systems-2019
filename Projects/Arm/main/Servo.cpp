#include "Servo.h"

extern "C" void ServoChannelConfig(Servo_t servo)
{
   ledc_channel_config_t config;
   config.channel = servo.channel;
   config.duty = 0;
   config.gpio_num = servo.gpio_pin;
   config.speed_mode = servo.speed_mode;
   config.timer_sel = servo.timer;
   ledc_channel_config(&config);
}

extern "C" void ServoTimerConfig(Servo_t servo)
{
   ledc_timer_config_t timer;
   timer.bit_num = servo.resolution;
   timer.freq_hz = servo.freq;
   timer.speed_mode = servo.speed_mode;
   timer.timer_num = servo.timer;
   ledc_timer_config(&timer);
}

extern "C" void SetServoAngle(Servo_t servo, uint16_t pwm, ledc_mode_t mode)
{
   // constrain(pwm, max, min);
   ledc_set_duty(mode, servo.channel, pwm);
   ledc_update_duty(mode, servo.channel);
}

extern "C" void MotorChannelConfig(Motor_t motor)
{
   ledc_channel_config_t config;
   config.channel = motor.channel;
   config.duty = 0;
   config.gpio_num = motor.gpio_pin;
   config.speed_mode = motor.speed_mode;
   config.timer_sel = motor.timer;
   ledc_channel_config(&config);
}

extern "C" void MotorTimerConfig(Motor_t motor)
{
   ledc_timer_config_t timer;
   timer.bit_num = motor.resolution;
   timer.freq_hz = motor.freq;
   timer.speed_mode = motor.speed_mode;
   timer.timer_num = motor.timer;
   ledc_timer_config(&timer);
}
