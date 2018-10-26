#include "common_init.h"

void I2C_master_init(gpio_num_t sda, gpio_num_t scl)
{
     // i2c_port_t i2c_master_port = I2C_EXAMPLE_MASTER_NUM;
    i2c_config_t config;
    config.mode = I2C_MODE_MASTER;
    config.sda_io_num = sda;
    config.sda_pullup_en = GPIO_PULLUP_ENABLE;
    config.scl_io_num = scl;
    config.scl_pullup_en = GPIO_PULLUP_ENABLE;
    config.master.clk_speed = I2C_EXAMPLE_MASTER_FREQ_HZ;
    i2c_param_config(I2C_EXAMPLE_MASTER_NUM, &config);
    i2c_driver_install(I2C_EXAMPLE_MASTER_NUM, config.mode, I2C_EXAMPLE_MASTER_RX_BUF_DISABLE, I2C_EXAMPLE_MASTER_TX_BUF_DISABLE, 0);
}

void adc1_config(adc1_channel_t channel, adc_bits_width_t width, esp_adc_cal_characteristics_t cal)
{
   adc1_config_width(width);
   adc1_config_channel_atten(channel, ADC_ATTEN_0db);
   esp_adc_cal_get_characteristics(ADC_V_REF, ADC_ATTEN_0db, width, &cal);
}

void gpio_configure(uint64_t pin, gpio_mode_t mode, gpio_pullup_t up_en, gpio_pulldown_t down_en, gpio_int_type_t intr)
{
   gpio_config_t io_config =  {
                                 1ULL << pin,
                                 mode,
                                 up_en,
                                 down_en,
                                 intr
                              }; 
   gpio_config(&io_config);
}
