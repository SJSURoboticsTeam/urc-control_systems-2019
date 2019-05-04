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

constexpr uint32_t SERVO_FREQUENCY = 40; //Frequency = 50 Hz

// Pitch Servo Constants //

constexpr uint32_t PITCH_SERVO_PIN = 32; // ESP-32 signal pin 32

constexpr float PITCH_SERVO_MIN = 2.5; // Constrained pitch min duty cycle (2.5)

constexpr float PITCH_SERVO_MAX = 12.5; // Constrained pitch max duty cycle (12.5)

constexpr float SERVO_CENTER = 35; //Pitch init duty cycle to rotate camera to the middle (50) [36]

constexpr float SERVO_DOWN = 45; // Pitch duty cycle to rotate camera up (63 Limit) [45]

constexpr float SERVO_UP = 31; // Pitch duty cycle to rotate camera down (37 Limit) [25]

constexpr float SERVO_SHIFT = 2; // Pitch duty cycle value to increment or decrement when moving the gimbal up or down

// Camera Lens Constants //

constexpr uint32_t LENS_SERVO_PIN = 16; // ESP-32 signal pin 16

constexpr float LENS_SERVO_MIN = 2.5; // Placeholder min duty cycle 

constexpr float LENS_SERVO_MAX = 12.5; // Placeholder max duty cycle

// Pitch Servo Variables //

// Pitch Servo //
extern Servo Pitch_Servo;

#endif
