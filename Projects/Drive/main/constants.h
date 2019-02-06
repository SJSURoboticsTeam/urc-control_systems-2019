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
#include "Motor_Control_rev1.hpp"
#include "freertos/FreeRTOS.h"

// Relevant rover dimensions in inches //
constexpr double SIDE = 40.25;
constexpr double SIDE_2_MID = 11.619;
constexpr double CORNER_2_MID = 23.238;
constexpr double MAX_DIST = 1058.35; // Distance where out wheel turns 1 degree

// HAL Sensor Pins //
constexpr uint32_t HAL_A_PIN = 27;
constexpr uint32_t HAL_B_PIN = 0;
constexpr uint32_t HAL_C_PIN = 4;

// Constants for the steering servos //
constexpr uint32_t SERVO_A_PIN = 25;
constexpr uint32_t SERVO_B_PIN = 10;
constexpr uint32_t SERVO_C_PIN = 19;

constexpr uint32_t SERVO_A_CHANNEL = 0;
constexpr uint32_t SERVO_B_CHANNEL = 1;
constexpr uint32_t SERVO_C_CHANNEL = 2;

constexpr uint32_t SERVO_TIMER = 0;
constexpr uint32_t SERVO_FREQUENCY = 50;

constexpr float SERVO_MIN = 2.5;
constexpr float SERVO_MAX = 12.5;

// Constants for the drive motors //
constexpr uint32_t MOTOR_A_PIN = 33;
constexpr uint32_t MOTOR_B_PIN = 2;
constexpr uint32_t MOTOR_C_PIN = 23;

constexpr uint32_t MOTOR_A_DIR = 32;
constexpr uint32_t MOTOR_B_DIR = 18;
constexpr uint32_t MOTOR_C_DIR = 22;

constexpr uint32_t MOTOR_A_BRAKE = 26;
constexpr uint32_t MOTOR_B_BRAKE = 16;
constexpr uint32_t MOTOR_C_BRAKE = 21;

constexpr uint32_t MOTOR_A_CHANNEL = 3;
constexpr uint32_t MOTOR_B_CHANNEL = 4;
constexpr uint32_t MOTOR_C_CHANNEL = 5;

constexpr uint32_t BRAKE_CHANNEL = 6;

constexpr uint32_t MOTOR_TIMER = 1;

/* Test servos 
constexpr uint32_t MOTOR_FREQUENCY = 50;
*/
/* Real Motors DH-03x */
constexpr uint32_t MOTOR_FREQUENCY = 78000;


constexpr float MOTOR_MIN = 0;
constexpr float MOTOR_MAX = 100;
constexpr float DEAD_MIN = 7.67;
constexpr float DEAD_MAX = 7.71;

// Initial positions for Car Mode (percent)
/* Test servos 
// Left = A, Right = B, Back = C
constexpr float DRIVE_POSITION_0[3] = {83.33, 16.67, 50};
// Left = B, Right = C, Back = A
constexpr float DRIVE_POSITION_1[3] = {50, 83.33, 16.67};
// Left = C, Right = A, Back = B
constexpr float DRIVE_POSITION_2[3] = {16.67, 50, 83.33};
*/

/* Real Servos DH-03X */
// Left = A, Right = B, Back = C
constexpr float DRIVE_POSITION_0[3] = {70.00, 30.00, 50};
// Left = B, Right = C, Back = A
constexpr float DRIVE_POSITION_1[3] = {50, 70.00, 30.00};
// Left = C, Right = A, Back = B
constexpr float DRIVE_POSITION_2[3] = {30.00, 50, 70.00};


// Positions for Spin Mode (percent)
/* Test Servos 
constexpr float ROTATE_POSITION_0 = 0;
constexpr float ROTATE_POSITION_1 = 100;
*/

/* Real Servos DH-03x */
constexpr float ROTATE_POSITION_0 = 20.00;
constexpr float ROTATE_POSITION_1 = 80.00;


// Boandaries for Crab Mode (degrees)
/* Test Servos 
constexpr double MAX_ROTATION = 180;
constexpr double MIN_ROTATION = 0;
*/

/* Real Servos DH-03X */
constexpr double MAX_ROTATION = 300;
constexpr double MIN_ROTATION = 0;


// Create the Steering Servos //
extern Servo servo_A;
extern Servo servo_B;
extern Servo servo_C; 

// Create the Drive Motors //
/*
extern ServoMotor motor_A;
extern ServoMotor motor_B;
extern ServoMotor motor_C;
*/
extern Motor motor_A;
extern Motor motor_B;
extern Motor motor_C;


// RTOS handlers //
extern TaskHandle_t xCarHandle;
extern TaskHandle_t xCrabHandle;
extern TaskHandle_t xSpinHandle;
extern TaskHandle_t xDebugHandle;

#endif