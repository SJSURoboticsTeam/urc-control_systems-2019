#ifndef MAGNET_ENCODER_H_
#define MAGNET_ENCODER_H_

#include "driver/i2c.h"
#include "Arm_Defines.h"
#include "common_defines.h"

esp_err_t I2C_mag_write(i2c_port_t i2c_num, uint8_t address, uint8_t data);

esp_err_t I2C_mag_write_multi(i2c_port_t i2c_num, uint8_t address, uint8_t data, uint8_t size);

esp_err_t I2C_mag_read(i2c_port_t i2c_num, uint8_t address, uint8_t *data);

esp_err_t I2C_mag_read_multi(i2c_port_t i2c_num, uint8_t address, uint8_t *data, uint8_t size);

#endif