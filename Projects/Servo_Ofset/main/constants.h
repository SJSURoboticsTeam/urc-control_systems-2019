////////////////////////////////////////////////////////////////////////////////
//                               CONSTANTS.H                                  //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// Use this header to store variables that are global for the entire project. //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
#include "driver/ledc.h"
#ifndef CONSTANTS_H_
#define CONSTANTS_H_

/**
 * Information for the accelerometer. Addresses found in the MPU6050 datasheet
 */
constexpr uint16_t MPU6050_ADDR = 0x68;
constexpr uint16_t ACCELEROMETER_ADDR = 0x3B;

/**
 * servo limits found from servo datasheet found at:
 * http://www.ee.ic.ac.uk/pcheung/teaching/DE1_EE/stores/sg90_datasheet.pdf
 */
constexpr uint32_t PWM_MIN_PERCENTAGE = 2.95;
constexpr uint32_t PWM_MAX_PERCENTAGE = 12;

#endif