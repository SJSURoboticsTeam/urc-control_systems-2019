#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
extern "C" {

#ifndef DEBUG
   #include "Wifi.h"
   #endif
	#ifdef DEBUG
		#include "stubs/Wifi.h"
	#endif
#include "Structure.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "soc/ledc_reg.h"
#include "driver/i2c.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "Arm_Defines.h"
#include "Servo.h"
#include "esp_system.h"
#include "esp_log.h"
}


#ifndef SOURCE_H_
#define SOURCE_H_


#ifdef _cplusplus
extern "C"{
#endif

// typedef enum{
// 	ROTUNDA = 33, // servo
// 	SHOULDER = 27,  // motor
// 	SHOULDER_DIR_PIN = 26, 
// 	ELBOW = 32, // servo
	
// 	//WRIST PITCH
// 	GEARBOX_1_IN_1 = 0,
// 	GEARBOX_1_IN_2 = 2,
	
// 	GEARBOX_2_IN_1 = 16,
// 	GEARBOX_2_IN_2 = 4,
	
// 	//CLAW

// 	LEADSCREW_IN_1 = 5,
// 	LEADSCREW_IN_2 = 17,

// 	CAM_SERVO = 25,
// 	CAM_MUX_SEL = 3,

// 	//SENSORS
// 	SHOULDER_MAG = 39,
// 	WRIST_ENCODER_A = 1,
// 	WRIST_ENCODER_B = 22,
// 	WRIST_MAG = 36,

// 	//SENSORS PROGRAMMING
// 	SHOUDLER_MAG_PRGM = 18,
// 	WRIST_MAG_PRGM = 23,

// 	//OTHER
// 	I2C3_CLK = 19,
// 	I2C3_DATA = 21,
// 	TMS = 14,
// 	TDI = 12,
// 	TCK = 13,
// 	TDO = 15
// } ArmPins;
  
#define MANUAL 0
#define AUTOMATIC 1
  
#define P_ON_M 0
#define P_ON_E 1

typedef struct 
{
   	unsigned long lastTime;
	double Input;
	double Output;
	double Setpoint;
	double ITerm;
	double outputSum;
	double outMin;
	double outMax;
	double lastInput;
	double kp;
	double ki;
	double kd;
	int16_t SampleTime = 100; //1 sec
	bool inAuto;
	int16_t controllerDirection = DIRECT;
}PID_t;

typedef struct {
	int base;
	int shoulder;
	int elbow;
	int wrist_pitch;
	int wrist_rotation;
	int claw;
}current_pos_t;

extern "C" void Arm_Init();

extern "C" void adc1_config(adc1_channel_t channel, adc_bits_width_t width, esp_adc_cal_characteristics_t cal);

inline long map(int16_t x, int16_t in_min, int16_t in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void SetShoulderAngle(Motor_t motor, uint16_t pwm, bool dir, ledc_mode_t mode);

void SetDirection(gpio_num_t pin, uint8_t direction);

void SetWrist(Motor_t m1, Motor_t m2, int16_t pwm, int16_t current, ledc_mode_t mode);

void RotateWrist(Motor_t m1, Motor_t m2, int16_t angle, int16_t current, ledc_mode_t mode);

void ClawControl(Motor_t m1, Motor_t m2, uint8_t ctrl, uint16_t pwm, ledc_mode_t mode);

uint16_t MagEncOffset(uint16_t in, int16_t offset, uint16_t min, uint16_t max);

void QuadEnc(int16_t *count, uint8_t *quad);

// PID
void Compute(PID_t *pid);

void SetTunings(PID_t *pid, double Kp, double Ki, double Kd);

void SetSampleTime(PID_t *pid, int16_t NewSampleTime);

void SetOutputLimits(PID_t *pid, double Min, double Max);

void SetMode(PID_t *pid, int16_t Mode);

void PidInit(PID_t *pid);

// void SetControllerDirection(PID_t pid, int Direction);

double convert(double current, uint16_t time);

#ifdef _cplusplus
}
#endif

#endif 
