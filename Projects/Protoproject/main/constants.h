////////////////////////////////////////////////////////////////////////////////
//                               CONSTANTS.H                                  //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// Use this header to store variables that are global for the entire project. //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <stdlib.h>
#include "Servo_Control.hpp"
#include "freertos/FreeRTOS.h"

#define EEPROM_SIZE 0x64

#define BEGINING_ADDR 0x00

// Overall Servo Constants //

constexpr uint32_t SERVO_CHANNEL = 0;

constexpr uint32_t SERVO_TIMER = 0;

constexpr uint32_t SERVO_FREQUENCY = 50; //Frequency = 50 Hz

// Kill Switch MOSFET Constants //

constexpr uint32_t MOSFET_PIN = 23;

// Pitch Servo Constants //

constexpr uint32_t PITCH_SERVO_PIN = 32; // ESP-32 signal pin 32

constexpr float PITCH_SERVO_MIN = 2.5; // Constrained pitch min duty cycle (2.5)

constexpr float PITCH_SERVO_MAX = 12.5; // Constrained pitch max duty cycle (12.5)

constexpr float SERVO_CENTER = 50; //Pitch init duty cycle to rotate camera to the middle (50- 50) [40- 36]

constexpr float SERVO_DOWN = 60; // Pitch duty cycle to rotate camera up (50- 63 Limit) [40- 45]

constexpr float SERVO_UP = 40; // Pitch duty cycle to rotate camera down (50- 37 Limit) [40- 25]

constexpr float SERVO_SHIFT = 1; // Pitch duty cycle value to increment or decrement when moving the gimbal up or down

// Camera Lens Constants //

constexpr uint32_t LENS_SERVO_PIN = 16; // ESP-32 signal pin 16

constexpr float LENS_SERVO_MIN = 2.5; // Placeholder min duty cycle 

constexpr float LENS_SERVO_MAX = 12.5; // Placeholder max duty cycle

// IMU Constants //

#define MPU6050_ACCEL_XOUT_H 0x3B
#define MPU6050_ACCEL_XOUT_L 0x3C
#define MPU6050_ACCEL_YOUT_H 0x3D
#define MPU6050_ACCEL_YOUT_L 0x3E
#define MPU6050_ACCEL_ZOUT_H 0x3F
#define MPU6050_ACCEL_ZOUT_L 0x40
#define MPU6050_TEMP_OUT_H   0x41
#define MPU6050_TEMP_OUT_L   0x42
#define MPU6050_GYRO_XOUT_H  0x43
#define MPU6050_GYRO_XOUT_L  0x44
#define MPU6050_GYRO_YOUT_H  0x45
#define MPU6050_GYRO_YOUT_L  0x46
#define MPU6050_GYRO_ZOUT_H  0x47
#define MPU6050_GYRO_ZOUT_L  0x48

#define MPU6050_ADDR0        0x68
#define MPU6050_ADDR1	     0x69
#define MPU6050_PWR_MGMT_1   0x6B

// Pitch Servo //
extern Servo Pitch_Servo;

#endif
