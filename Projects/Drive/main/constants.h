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

#define EEPROM_SIZE 0x64

#define BEGINING_ADDR 0x00

// Constants for the steering servos
constexpr uint32_t SERVO_A_PIN = 15;
constexpr uint32_t SERVO_B_PIN = 16;
constexpr uint32_t SERVO_C_PIN = 17;

constexpr uint32_t SERVO_A_CHANNEL = 0;
constexpr uint32_t SERVO_B_CHANNEL = 1;
constexpr uint32_t SERVO_C_CHANNEL = 2;

constexpr uint32_t SERVO_TIMER = 0;
constexpr uint32_t SERVO_FREQUENCY = 50;

constexpr float SERVO_MIN = 3.5;
constexpr float SERVO_MAX = 13.5;

// Constants for the drive motors
constexpr uint32_t MOTOR_A_PIN = 21;
constexpr uint32_t MOTOR_B_PIN = 22;
constexpr uint32_t MOTOR_C_PIN = 23;

constexpr uint32_t MOTOR_A_CHANNEL = 3;
constexpr uint32_t MOTOR_B_CHANNEL = 4;
constexpr uint32_t MOTOR_C_CHANNEL = 5;

constexpr uint32_t MOTOR_TIMER = 1;
constexpr uint32_t MOTOR_FREQUENCY = 50;

constexpr float MOTOR_MIN = 5;
constexpr float MOTOR_MAX = 10;
constexpr float DEAD_MIN = 7.2;
constexpr float DEAD_MAX = 7.5;

// Initial positions for different steering modes
constexpr float DRIVE_POSITION_0[3] = {75, 25, 50};
constexpr float DRIVE_POSITION_1[3] = {50, 75, 25};
constexpr float DRIVE_POSITION_2[3] = {25, 50, 75};

constexpr float ROTATE_POSITION_0 = 0;
constexpr float ROTATE_POSITION_1 = 100;

// Create the Steering Servos
extern Servo servo_A;
extern Servo servo_B;
extern Servo servo_C; 

// Create the Drive Motors
extern ServoMotor motor_A;
extern ServoMotor motor_B;
extern ServoMotor motor_C;

#endif