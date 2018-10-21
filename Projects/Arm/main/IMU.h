#ifndef IMU_H_
#define IMU_H_

#include "driver/i2c.h"
#include "Arm_Defines.h"
#include "common_defines.h"

esp_err_t BNO_accel_opr_setup(i2c_port_t i2c_num, uint8_t data);

esp_err_t BNO_accel_read_multi(i2c_port_t i2c_num, uint8_t address, uint8_t *data, uint8_t size);

esp_err_t MPU_accel_read_multi(i2c_port_t i2c_num, uint8_t address, uint8_t *data, uint8_t size);

esp_err_t I2C_gimbal_write(i2c_port_t i2c_num, uint8_t address, uint8_t data);

esp_err_t I2C_gimbal_write_multi(i2c_port_t i2c_num, uint8_t address, uint8_t data, uint8_t size);

esp_err_t I2C_gimbal_read(i2c_port_t i2c_num, uint8_t address, uint8_t *data);

esp_err_t I2C_gimbal_read_multi(i2c_port_t i2c_num, uint8_t address, uint8_t *data, uint8_t size);

extern "C" void I2C_Init();

extern "C" void BNO055_Init();

extern "C" esp_err_t BNO055_ReadData(int Reg_Value, uint8_t* data);

extern "C" int16_t BNO055_GetHeading();

extern "C" int16_t BNO055_GetRoll();

extern "C" int16_t BNO055_GetPitch();

void Gimbalinit();

int GimbalgetData();

void i2c_example_master_init();


#endif