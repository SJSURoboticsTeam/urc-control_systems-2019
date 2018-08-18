#ifndef SERVO_H_
#define SERVO_H_

#include "driver/ledc.h"
#include "utilities.h"

typedef struct{
	gpio_num_t gpio_pin;
	uint32_t freq;
	ledc_channel_t channel;
	ledc_mode_t speed_mode;
	ledc_timer_t timer;
	ledc_timer_bit_t resolution;
}Servo_t;

typedef struct{
	gpio_num_t gpio_pin;
	uint32_t freq;
	ledc_channel_t channel;
	uint8_t max_rpm;
	uint8_t direction;
	ledc_mode_t speed_mode;
	ledc_timer_t timer;
	ledc_timer_bit_t resolution;
}Motor_t;

void ServoChannelConfig(Servo_t servo);

void ServoTimerConfig(Servo_t servo);

void SetServoAngle(Servo_t servo, uint16_t pwm, ledc_mode_t mode);

void MotorChannelConfig(Motor_t motor);

void MotorTimerConfig(Motor_t motor);

void SetMotorPWM(Motor_t motor, int16_t pwm, ledc_mode_t mode);

#endif