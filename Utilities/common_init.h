#ifndef COMMON_INIT_H_
#define COMMON_INIT_H_

#include "driver/gpio.h"
#include "driver/adc.h"
#include "driver/i2c.h"
#include "esp_adc_cal.h"
#include "common_defines.h"

void I2C_master_init(gpio_num_t sda, gpio_num_t scl);

void adc1_config(adc1_channel_t channel, adc_bits_width_t width, esp_adc_cal_characteristics_t cal);

void gpio_configure(uint64_t pin, gpio_mode_t mode, gpio_pullup_t up_en, gpio_pulldown_t down_en, gpio_int_type_t intr);

#endif