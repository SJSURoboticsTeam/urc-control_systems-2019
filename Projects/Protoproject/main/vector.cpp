#include <Wire.h>
#include "vector.hpp"

imu::Vector<3> scanAccel(uint8_t IMU_ADDRESS)
{
    int16_t accel_x, accel_y, accel_z;

    //Initialize Device
    Wire.begin();
    Wire.beginTransmission(IMU_ADDRESS);
    Wire.write(0x6B);
    Wire.write(0);
    Wire.endTransmission(true);

    //Read Accel Registers
    Wire.beginTransmission(IMU_ADDRESS);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(IMU_ADDRESS, 3*2, true);

    accel_x = Wire.read()<<8 | Wire.read(); //ACCEL_X 3B (H) 3C (L)
    accel_y = Wire.read()<<8 | Wire.read(); //ACCEL_Y 3D (H) 3E (L)
    accel_z = Wire.read()<<8 | Wire.read(); //ACCEL_Z 3F (H) 40 (L)

    imu::Vector<3> accel(accel_x, accel_y, accel_z);
    return accel;
}

imu::Vector<3> scanGyro(uint8_t IMU_ADDRESS)
{
    int16_t gyro_x, gyro_y, gyro_z;

    //Initialize Device
    Wire.begin();
    Wire.beginTransmission(IMU_ADDRESS);
    Wire.write(0x6B);
    Wire.write(0);
    Wire.endTransmission(true);

    //Read Accel, Gyro, Temp Registers
    Wire.beginTransmission(IMU_ADDRESS);
    Wire.write(0x43);
    Wire.endTransmission(false);
    Wire.requestFrom(IMU_ADDRESS, 3*2, true);

    gyro_x  = Wire.read()<<8 | Wire.read(); //GYRO_X  43 (H) 44 (L)
    gyro_y  = Wire.read()<<8 | Wire.read(); //GYRO_Y  45 (H) 46 (L)
    gyro_z  = Wire.read()<<8 | Wire.read(); //GYRO_Z  47 (H) 48 (L)

    imu::Vector<3> gyro(gyro_x, gyro_y, gyro_z);
    return gyro;
}