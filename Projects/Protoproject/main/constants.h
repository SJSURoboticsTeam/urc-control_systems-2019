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

// Pitch Servo Constants //

constexpr uint32_t PITCH_SERVO_PIN = 15; // ESP-32 signal pin = 15

constexpr uint32_t PITCH_SERVO_CHANNEL = 0;

constexpr uint32_t SERVO_TIMER = 0;

constexpr uint32_t SERVO_FREQUENCY = 50; //Frequency = 50 Hz

constexpr float SERVO_MEDIAN = 7.15; //Pitch init duty cycle to rotate camera to the middle

constexpr float SERVO_UP = 7.8; // Pitch duty cycle to rotate camera up

constexpr float SERVO_DOWN = 6.9; // Pitch duty cycle to rotate camera down

constexpr float SERVO_MIN_LIMIT = 6.3; // Pitch min duty cycle limit

constexpr float SERVO_MAX_LIMIT = 8.32; // Pitch max duty cycle limit

constexpr float SERVO_MIN = 6.4; // Constrained pitch min duty cycle

constexpr float SERVO_MAX = 8.3; // Constrained pitch max duty cycle

// Pitch Servo Variables //

// Pitch Servo //
extern Servo Pitch_Servo;

#endif
