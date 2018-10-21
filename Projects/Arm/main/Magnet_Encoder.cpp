#include "Magnet_Encoder.h"

esp_err_t I2C_mag_write(i2c_port_t i2c_num, uint8_t address, uint8_t data)
{
    esp_err_t ret;
    i2c_cmd_handle_t m_write = i2c_cmd_link_create();
    ret = i2c_master_start(m_write);
    ret = i2c_master_write_byte(m_write, AS5600_SLAVE_WRITE, ACK_EN);
    ret = i2c_master_write_byte(m_write, address, ACK_EN);
    ret = i2c_master_write_byte(m_write, data, ACK_EN);
    ret = i2c_master_stop(m_write);
    ret = i2c_master_cmd_begin(i2c_num, m_write, 100 / portTICK_RATE_MS);
    i2c_cmd_link_delete(m_write);
    return ret;
}

esp_err_t I2C_mag_write_multi(i2c_port_t i2c_num, uint8_t address, uint8_t *data, uint8_t size)
{
    esp_err_t ret;
    i2c_cmd_handle_t m_write = i2c_cmd_link_create();
    ret = i2c_master_start(m_write);
    ret = i2c_master_write_byte(m_write, AS5600_SLAVE_WRITE, ACK_EN);
    ret = i2c_master_write_byte(m_write, address, ACK_EN);
    ret = i2c_master_write(m_write, data, size - 1, ACK_EN);
    ret = i2c_master_stop(m_write);
    ret = i2c_master_cmd_begin(i2c_num, m_write, 100 / portTICK_RATE_MS);
    i2c_cmd_link_delete(m_write);
    return ret;
}

esp_err_t I2C_mag_read(i2c_port_t i2c_num, uint8_t address, uint8_t *data)
{
    esp_err_t ret;
    i2c_cmd_handle_t m_read = i2c_cmd_link_create();
    ret = i2c_master_start(m_read);
    ret = i2c_master_write_byte(m_read, AS5600_SLAVE_READ, ACK_EN);
    ret = i2c_master_write_byte(m_read, address, ACK_EN);
    ret = i2c_master_write_byte(m_read, address + 1, ACK_EN);
    ret = i2c_master_read_byte(m_read, data, NACK_VAL);
    ret = i2c_master_stop(m_read);
    ret = i2c_master_cmd_begin(i2c_num, m_read, 100 / portTICK_RATE_MS);
    i2c_cmd_link_delete(m_read);
    // printf("%X\n", ret);
    return ret;
}

esp_err_t I2C_mag_read_multi(i2c_port_t i2c_num, uint8_t address, uint8_t *data, uint8_t size)
{ 
    esp_err_t ret;
 
    i2c_cmd_handle_t m_read = i2c_cmd_link_create();
    i2c_master_start(m_read);
    ret = i2c_master_write_byte(m_read, AS5600_SLAVE_WRITE, ACK_EN);
    ret = i2c_master_write_byte(m_read, address, ACK_EN);
    ret = i2c_master_stop(m_read);
    ret = i2c_master_cmd_begin(i2c_num, m_read, 100 / portTICK_RATE_MS);
    i2c_cmd_link_delete(m_read);
 
    // printf("\n");
     vTaskDelay(30 / portTICK_RATE_MS);

    m_read = i2c_cmd_link_create();
    i2c_master_start(m_read);
    ret = i2c_master_write_byte(m_read, AS5600_SLAVE_READ, ACK_EN);
    ret = i2c_master_read_byte(m_read, data, ACK_VAL);
    ret = i2c_master_read_byte(m_read, (data + 1), NACK_VAL);
    ret = i2c_master_stop(m_read);
    ret = i2c_master_cmd_begin(i2c_num, m_read, 100 / portTICK_RATE_MS);
    i2c_cmd_link_delete(m_read);
    return ret;
}
