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

// Relevant rover dimensions in inches //
constexpr double SIDE = 40.25;
constexpr double SIDE_2_MID = 11.619;
constexpr double CORNER_2_MID = 23.238;

// Constants for the steering servos //
constexpr uint32_t SERVO_A_PIN = 15;
constexpr uint32_t SERVO_B_PIN = 16;
constexpr uint32_t SERVO_C_PIN = 17;

constexpr uint32_t SERVO_A_CHANNEL = 0;
constexpr uint32_t SERVO_B_CHANNEL = 1;
constexpr uint32_t SERVO_C_CHANNEL = 2;

constexpr uint32_t SERVO_TIMER = 0;
constexpr uint32_t SERVO_FREQUENCY = 50;

constexpr float SERVO_MIN = 2.2;
constexpr float SERVO_MAX = 11;

// Constants for the drive motors //
constexpr uint32_t MOTOR_A_PIN = 21;
constexpr uint32_t MOTOR_B_PIN = 22;
constexpr uint32_t MOTOR_C_PIN = 23;

constexpr uint32_t MOTOR_A_CHANNEL = 3;
constexpr uint32_t MOTOR_B_CHANNEL = 4;
constexpr uint32_t MOTOR_C_CHANNEL = 5;

constexpr uint32_t MOTOR_TIMER = 1;
constexpr uint32_t MOTOR_FREQUENCY = 50;

constexpr float MOTOR_MIN = 6.5;
constexpr float MOTOR_MAX = 8.5;
constexpr float DEAD_MIN = 7.67;
constexpr float DEAD_MAX = 7.71;

// Initial positions for different steering modes
/* test servos */
// Left = A, Right = B, Back = C
constexpr float DRIVE_POSITION_0[3] = {83.33, 16.67, 50};
// Left = B, Right = C, Back = A
constexpr float DRIVE_POSITION_1[3] = {50, 83.33, 16.67};
// Left = C, Right = A, Back = B
constexpr float DRIVE_POSITION_2[3] = {16.67, 50, 83.33};

/* Real Servos
// Left = A, Right = B, Back = C
constexpr float DRIVE_POSITION_0[3] = {72.22, 27.78, 50};
// Left = B, Right = C, Back = A
constexpr float DRIVE_POSITION_1[3] = {50, 72.22, 27.78};
// Left = C, Right = A, Back = B
constexpr float DRIVE_POSITION_2[3] = {27.78, 50, 72.22};
*/
constexpr float ROTATE_POSITION_0 = 0;
constexpr float ROTATE_POSITION_1 = 100;

// Create the Steering Servos //
extern Servo servo_A;
extern Servo servo_B;
extern Servo servo_C; 

// Create the Drive Motors //
extern ServoMotor motor_A;
extern ServoMotor motor_B;
extern ServoMotor motor_C;

// RTOS handlers //
extern TaskHandle_t xCarHandle;
extern TaskHandle_t xCrabHandle;
extern TaskHandle_t xSpinHandle;
extern TaskHandle_t xDebugHandle;

#endif