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
constexpr uint32_t PITCH_SERVO_PIN = 15;

constexpr uint32_t PITCH_SERVO_CHANNEL = 0;

constexpr uint32_t SERVO_TIMER = 0;

constexpr uint32_t SERVO_FREQUENCY = 50;

constexpr float SERVO_MIN = 2.2;
constexpr float SERVO_MAX = 11;


// Pitch Servo //
extern Servo Pitch_Servo;

#endif
