#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "Source.h"
#include "Arduino.h"
#include "Wire.h"
#include "constants.h"

////////////////////////////////////////////////////////////////////////////////
//                               BEGIN CODE HERE                              //
////////////////////////////////////////////////////////////////////////////////
/**
 * This function initializes the Accelerometer by beginning a I2C transmission,
 * selecting the address of the MPU6050, clearing it's contents to begin
 * reading, and ending the transmission.
 */
void initAccelerometer()
{
    Wire.begin();
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(0x6B);
    Wire.write(0);
    Wire.endTransmission(true);
}

/** 
 * This functiion Takes in a char that refers to one of the axis on board the
 * MPU6050 accelerometer. First, the raw data from each of the axis is read by 
 * begining an I2C transmission to the MPU6050 that reads 6 bytes, storing the
 * data read into three 16-bit signed integers. Second, the inputted char value
 * is used to determine which axis should be calulated with the raw data
 * received. The angle is calculated using Euler equations for converting data 
 * from an accelerometer to roll, pitch, and yaw. The labels 'X', 'Y', and 'Z' 
 * are therefor inaccurate, as these labels do not exist in spherical context,
 * but will be used for our own clarity and continuity. The Euler equations 
 * used are: 
 *                          roll = atan(Yacc/sqrt(Xacc^2 + Zacc^2))
 *                          pitch = atan(sqrt(Xacc^2 + Yacc^2)/Zacc)
 *                          yaw = atan(Xacc/sqrt(Yacc^2 + Zacc^2))
 * The angle, as a double and in degrees, is returned.
 */
double getAngle(char axis)
{
    int16_t raw_x = 0; 
    int16_t  raw_y = 0; 
    int16_t raw_z = 0;
    double angle;
    float toDeg = 180 / 3.14;
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(ACCELEROMETER_ADDR);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU6050_ADDR, 6, true);
    raw_x = Wire.read() << 8 | Wire.read();
    raw_y = Wire.read() << 8 | Wire.read();
    raw_z = Wire.read() << 8 | Wire.read();
    
    switch (axis)
    {
        case 'X':
            angle = atan2(raw_y, sqrt(raw_x * raw_x + raw_z * raw_z)) * toDeg;
	    break;
	case 'Y':
            angle = atan2(sqrt(raw_x *raw_x + raw_y * raw_y), raw_z) * toDeg;
	    break;
	case 'Z':
            angle = atan2(raw_x, sqrt(raw_y * raw_y + raw_z * raw_z)) * toDeg;
	    break;
	default:
            angle = 0;
	    break;
    }
    return angle;
}

/**
 * This function calculates the duty cycle to be used by the servo based off of
 * the angle read by the accelerometer. The function takes a double and the
 * input, and references global values that represent the duty cycle limits as
 * specified by the servo's datasheet. This function calculates duty cycle
 * values in relation to the pwm driver characteristics of the esp32. A 10-bit
 * value is chosen for the pwm variable, giving the maximum possible pwm value
 * 1023. Upper and lower limists in this context are calulated using the
 * previously mentioned limits set by the servo datasheet. A limiter is added in 
 * the case that the accelerometer reads a value beyond the 180 degree range of 
 * the servo so that the servo does not attempt to move past that. A percentage 
 * of the total range is calculated based on the angle read, and this percentage 
 * is used to calculate a pwm value within the stated limitations.
 */
uint16_t getDutyCycle(double angle)
{
    uint16_t max_duty = ((1 << LEDC_TIMER_10_BIT) - 1);
    uint16_t lower_lim = max_duty * PWM_MIN_PERCENTAGE / 100;
    uint16_t upper_lim = max_duty * PWM_MAX_PERCENTAGE / 100;
    uint16_t range = upper_lim - lower_lim;
    uint16_t middle_point = lower_lim + (range / 2);
    
    if (angle < -90)
    {
        angle = -90;
    }
    else if (angle > 90)
    {
        angle = 90;
    }

    uint16_t percentage = 100 - (50 + (angle / 90) * 50);
    uint16_t new_duty = lower_lim + (range * percentage / 100);
    return new_duty;
}

////////////////////////////////////////////////////////////////////////////////
//                               CODE ENDS HERE                               //
////////////////////////////////////////////////////////////////////////////////