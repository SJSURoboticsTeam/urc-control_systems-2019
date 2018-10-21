#include "IMU.h"

esp_err_t I2C_gimbal_write(i2c_port_t i2c_num, uint8_t address, uint8_t data)
{
    esp_err_t ret;
    i2c_cmd_handle_t g_write = i2c_cmd_link_create();
    ret = i2c_master_start(g_write);
    ret = i2c_master_write_byte(g_write, LIS3DH_SA0_1_WRITE, ACK_EN);
    ret = i2c_master_write_byte(g_write, address, ACK_EN);
    ret = i2c_master_write_byte(g_write, data, ACK_EN);
    ret = i2c_master_stop(g_write);
    ret = i2c_master_cmd_begin(i2c_num, g_write, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(g_write);
    return ret;
}

esp_err_t I2C_gimbal_write_multi(i2c_port_t i2c_num, uint8_t address, uint8_t *data, uint8_t size)
{
    esp_err_t ret;
    i2c_cmd_handle_t g_write = i2c_cmd_link_create();
    ret = i2c_master_start(g_write);
    ret = i2c_master_write_byte(g_write, LIS3DH_SA0_1_WRITE, ACK_EN);
    ret = i2c_master_write_byte(g_write, address, ACK_EN);
    ret = i2c_master_write(g_write, data, size - 1, ACK_EN);
    ret = i2c_master_stop(g_write);
    ret = i2c_master_cmd_begin(i2c_num, g_write, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(g_write);
    return ret;
}

esp_err_t I2C_gimbal_read(i2c_port_t i2c_num, uint8_t address, uint8_t *data)
{
    esp_err_t ret;
    i2c_cmd_handle_t g_read = i2c_cmd_link_create();
    ret = i2c_master_start(g_read);
    ret = i2c_master_write_byte(g_read, LIS3DH_SA0_1_WRITE, ACK_EN);
    ret = i2c_master_write_byte(g_read, address, ACK_EN);
    ret = i2c_master_read_byte(g_read, data, ACK_VAL);
    ret = i2c_master_stop(g_read);
    ret = i2c_master_cmd_begin(i2c_num, g_read, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(g_read);
    return ret;
}

esp_err_t I2C_gimbal_read_multi(i2c_port_t i2c_num, uint8_t address, uint8_t *data, uint8_t size)
{
    esp_err_t ret;
    i2c_cmd_handle_t g_read = i2c_cmd_link_create();
    ret = i2c_master_start(g_read);
    ret = i2c_master_write_byte(g_read, LIS3DH_SA0_1_WRITE, ACK_EN);
    ret = i2c_master_write_byte(g_read, address, ACK_EN);
    ret = i2c_master_read(g_read, data, size - 1, ACK_VAL);
    ret = i2c_master_stop(g_read);
    ret = i2c_master_cmd_begin(i2c_num, g_read, 10000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(g_read);
    return ret;
}

esp_err_t BNO_accel_opr_setup(i2c_port_t i2c_num, uint8_t data)
{
    esp_err_t ret;
    uint8_t data_h,data_l;

    // printf("\nWRITE ");
    i2c_cmd_handle_t a_write = i2c_cmd_link_create();
    i2c_master_start(a_write);
    ret = i2c_master_write_byte(a_write, (BNO055_ADDR_COM3_HIGH << 1) | WRITE_BIT, ACK_EN);
    // printf("slave: %X ", ret);
    ret = i2c_master_write_byte(a_write, BNO055_OPR_MODE_ADDR, ACK_EN);
    // printf("opr: %X ", ret);
    ret = i2c_master_write_byte(a_write, data, ACK_EN);
    // printf("data: %X ", ret);
    ret = i2c_master_stop(a_write);
    // printf("stop: %X ", ret);
    ret = i2c_master_cmd_begin(i2c_num, a_write, 1000 / portTICK_RATE_MS);
    // printf("begin: %X ", ret);
    i2c_cmd_link_delete(a_write);
    
    vTaskDelay(30 / portTICK_RATE_MS);
    a_write = i2c_cmd_link_create();
    ret = i2c_master_start(a_write);
    ret = i2c_master_write_byte(a_write,(BNO055_ADDR_COM3_HIGH << 1) | WRITE_BIT, ACK_CHECK_EN);
    // printf("slave: %X ", ret);
    ret = i2c_master_write_byte(a_write, 0x3B, ACK_CHECK_EN);
    // printf("unit: %X ", ret);
    ret = i2c_master_write_byte(a_write, 0x00, ACK_CHECK_EN);
    // printf("data: %X ", ret);
    ret = i2c_master_stop(a_write);
    // printf("stop: %X ", ret);
    ret = i2c_master_cmd_begin(i2c_num, a_write, 1000 / portTICK_RATE_MS);
    // printf("begin: %X\n", ret);
    i2c_cmd_link_delete(a_write);
    // a_write = i2c_cmd_link_create();
    // i2c_master_start(a_write);
    // ret = i2c_master_write_byte(a_write, (BNO055_ADDR_COM3_HIGH << 1) | WRITE_BIT, ACK_EN);
    // ret = i2c_master_write_byte(a_write, BNO055_UNIT_SEL, ACK_EN);
    // ret = i2c_master_write_byte(a_write, 0x00, ACK_EN);
    // ret = i2c_master_stop(a_write);
    // ret = i2c_master_cmd_begin(i2c_num, a_write, 1000 / portTICK_RATE_MS);
    // i2c_cmd_link_delete(a_write);

    return ret;
}

esp_err_t BNO_accel_read_multi(i2c_port_t i2c_num, uint8_t address, uint8_t *data, uint8_t size)
{
    esp_err_t ret;
 
    // printf("\nWRITE ");
    i2c_cmd_handle_t a_read = i2c_cmd_link_create();
    i2c_master_start(a_read);
    ret = i2c_master_write_byte(a_read, (BNO055_ADDR_COM3_HIGH << 1) | WRITE_BIT, ACK_EN);
    // printf("slave: %X ", ret);
    ret = i2c_master_write_byte(a_read, address, ACK_EN);
    // printf("reg 1: %X ", ret);
    ret = i2c_master_stop(a_read);
    // printf("stop: %X ", ret);
    ret = i2c_master_cmd_begin(i2c_num, a_read, 1000 / portTICK_RATE_MS);
    // printf("begin: %X ", ret);
    i2c_cmd_link_delete(a_read);
 
    // printf("\n");
    vTaskDelay(30 / portTICK_RATE_MS);
    // printf("READ ");
    a_read = i2c_cmd_link_create();
    i2c_master_start(a_read);
    ret = i2c_master_write_byte(a_read, (BNO055_ADDR_COM3_HIGH << 1) | READ_BIT, ACK_EN);
    // printf("slave: %X ", ret);
    ret = i2c_master_read_byte(a_read, data, ACK_VAL);
    // printf("data H: %X ", ret);
    ret = i2c_master_read_byte(a_read, (data + 1), ACK_VAL);
    ret = i2c_master_read_byte(a_read, (data + 2), ACK_VAL);
    ret = i2c_master_read_byte(a_read, (data + 3), ACK_VAL);
    ret = i2c_master_read_byte(a_read, (data + 4), ACK_VAL);
    ret = i2c_master_read_byte(a_read, (data + 5), NACK_VAL);
    // printf("data L: %X ", ret);
    ret = i2c_master_stop(a_read);
    // printf("stop: %X ", ret);
    ret = i2c_master_cmd_begin(i2c_num, a_read, 1000 / portTICK_RATE_MS);
    // printf("begin: %X \n", ret);
    i2c_cmd_link_delete(a_read);
    return ret;
}

esp_err_t MPU_accel_read_multi(i2c_port_t i2c_num, uint8_t address, uint8_t *data, uint8_t size)
{
    esp_err_t ret;
 
    // printf("\nWRITE ");
    i2c_cmd_handle_t a_read = i2c_cmd_link_create();
    i2c_master_start(a_read);
    ret = i2c_master_write_byte(a_read, (MPU_9250_ADDR << 1) | WRITE_BIT, ACK_EN);
    // printf("slave: %X ", ret);
    ret = i2c_master_write_byte(a_read, address, ACK_EN);
    // printf("reg 1: %X ", ret);
    ret = i2c_master_stop(a_read);
    // printf("stop: %X ", ret);
    ret = i2c_master_cmd_begin(i2c_num, a_read, 1000 / portTICK_RATE_MS);
    // printf("begin: %X ", ret);
    i2c_cmd_link_delete(a_read);
 
    // printf("\n");
    vTaskDelay(30 / portTICK_RATE_MS);
    // printf("READ ");
    a_read = i2c_cmd_link_create();
    i2c_master_start(a_read);
    ret = i2c_master_write_byte(a_read, (MPU_9250_ADDR << 1) | READ_BIT, ACK_EN);
    // printf("slave: %X ", ret);
    ret = i2c_master_read_byte(a_read, data, ACK_VAL);
    // printf("data H: %X ", ret);
    ret = i2c_master_read_byte(a_read, (data + 1), ACK_VAL);
    ret = i2c_master_read_byte(a_read, (data + 2), ACK_VAL);
    ret = i2c_master_read_byte(a_read, (data + 3), ACK_VAL);
    ret = i2c_master_read_byte(a_read, (data + 4), ACK_VAL);
    ret = i2c_master_read_byte(a_read, (data + 5), NACK_VAL);
    // printf("data L: %X ", ret);
    ret = i2c_master_stop(a_read);
    // printf("stop: %X ", ret);
    ret = i2c_master_cmd_begin(i2c_num, a_read, 1000 / portTICK_RATE_MS);
    // printf("begin: %X \n", ret);
    i2c_cmd_link_delete(a_read);
    return ret;
}

extern "C" void I2C_Init()
{
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_EXAMPLE_MASTER_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = I2C_EXAMPLE_MASTER_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_EXAMPLE_MASTER_FREQ_HZ;
    i2c_param_config(I2C_EXAMPLE_MASTER_NUM  , &conf);
    i2c_driver_install(I2C_EXAMPLE_MASTER_NUM  , conf.mode,
                       I2C_EXAMPLE_MASTER_RX_BUF_DISABLE,
                       I2C_EXAMPLE_MASTER_TX_BUF_DISABLE, 0);
}

extern "C" void BNO055_Init()
{   
    esp_err_t ret;
    uint8_t data_h,data_l;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (BNO055_ADDR << 1) | WRITE_BIT, true);
    i2c_master_write_byte(cmd, BNO055_OPR_MODE, true);
    i2c_master_write_byte(cmd, 0x0C, true); //writing xxxx1100b to set NDFO mode
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
    printf("begin: %X\t", ret);
    i2c_cmd_link_delete(cmd);

    vTaskDelay(30 / portTICK_RATE_MS);
    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (BNO055_ADDR << 1) | READ_BIT, ACK_CHECK_EN);
    i2c_master_read_byte(cmd, &data_h, ACK_VAL);
    i2c_master_read_byte(cmd, &data_l, NACK_VAL);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
    printf("begin: %X \n", ret);
    i2c_cmd_link_delete(cmd);
    printf("ret %i, data_h %i, data_l %i \n", ret, data_h, data_l);
  
}

extern "C" esp_err_t BNO055_ReadData(int Reg_Value, uint8_t* data)
{
    esp_err_t ret;
    //selecting register 
    // printf("%i\n", Reg_Value );
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (BNO055_ADDR << 1) | WRITE_BIT, true);
    i2c_master_write_byte(cmd, Reg_Value, true);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
    printf("begin: %X\t", ret);
    i2c_cmd_link_delete(cmd);

    vTaskDelay(30 / portTICK_RATE_MS);
    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (BNO055_ADDR << 1) | READ_BIT, ACK_CHECK_EN);
    i2c_master_read_byte(cmd, data, NACK_VAL);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
    printf("begin: %X\t", ret);
    i2c_cmd_link_delete(cmd);
    return ret;
}

extern "C" int16_t BNO055_GetHeading()
{
    uint8_t data_h;
    uint8_t data_l;
    BNO055_ReadData(BNO055_HEADING_MSB,&data_h);
    BNO055_ReadData(BNO055_HEADING_LSB,&data_l);
    uint16_t final_value = ((uint16_t) data_h << 8) | (uint16_t) data_l;
    return final_value;
}

extern "C" int16_t BNO055_GetRoll()
{
    uint8_t data_h;
    uint8_t data_l;
    BNO055_ReadData(BNO055_ROLL_MSB,&data_h);
    BNO055_ReadData(BNO055_ROLL_LSB,&data_l);
    uint16_t final_value = ((uint16_t) data_h << 8) | (uint16_t) data_l;
    return final_value;
}

extern "C" int16_t BNO055_GetPitch()
{
    uint8_t data_h;
    uint8_t data_l;
    BNO055_ReadData(BNO055_PITCH_MSB,&data_h);
    BNO055_ReadData(BNO055_PITCH_LSB,&data_l);
    uint16_t final_value = ((uint16_t) data_h << 8) | (uint16_t) data_l;
    return final_value;
}

void Gimbalinit(){
// print_mux = xSemaphoreCreateMutex();
    esp_err_t ret;
    i2c_example_master_init();
i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, BNO055_SENSOR_ADDR << 1 | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, OPR_MODE, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, 0x0C, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd,BNO055_SENSOR_ADDR << 1 | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, UNIT_SEL, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, 0x00, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

}
int GimbalgetData(){
uint8_t data[6];
int16_t X, Y, Z;
int16_t thetaX, thetaY, thetaZ;
esp_err_t ret;
    i2c_cmd_handle_t a_read;
    


    //printf("\nWRITE ");
    a_read = i2c_cmd_link_create();
    i2c_master_start(a_read);
    ret = i2c_master_write_byte(a_read, BNO055_SENSOR_ADDR<< 1 | WRITE_BIT, ACK_EN);
    //printf("slave: %X ", ret);
    ret = i2c_master_write_byte(a_read, 0x1A, ACK_EN); //starting address for euler angles
    //printf("reg 1: %X ", ret);
    ret = i2c_master_stop(a_read);
   // printf("stop: %X ", ret);
    ret = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, a_read, 1000 / portTICK_RATE_MS);
    //printf("begin: %X ", ret);
    i2c_cmd_link_delete(a_read);
 
    // printf("\n");
 
    //printf("READ ");
    a_read = i2c_cmd_link_create();
    i2c_master_start(a_read);
    ret = i2c_master_write_byte(a_read, BNO055_SENSOR_ADDR<< 1 | READ_BIT, ACK_EN);
    //printf("slave: %X ", ret);
    ret = i2c_master_read_byte(a_read, data, ACK_VAL);
   // printf("data H: %X ", ret);
    ret = i2c_master_read_byte(a_read, (data + 1), ACK_VAL);
   // printf("data L: %X ", ret);
    ret = i2c_master_read_byte(a_read, (data + 2), ACK_VAL);
    //printf("data H: %X ", ret);
    ret = i2c_master_read_byte(a_read, (data + 3), ACK_VAL);
   // printf("data L: %X ", ret);
    ret = i2c_master_read_byte(a_read, (data + 4), ACK_VAL);
    //printf("data H: %X ", ret);
    ret = i2c_master_read_byte(a_read, (data + 5), NACK_VAL);
    //printf("data L: %X ", ret);
    ret = i2c_master_stop(a_read);
    //printf("stop: %X ", ret);
    ret = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, a_read, 1000 / portTICK_RATE_MS);
    //printf("begin: %i \n", ret);
    i2c_cmd_link_delete(a_read);    
    

    X = ((int16_t)data[1] << 8) | data[0];
Y = ((int16_t)data[3] << 8) | data[2];
Z = ((int16_t)data[5] << 8) | data[4];
thetaX = X/16;      //euler angles = 1 degree per 16 ticks
thetaY = Y/16;
thetaZ = Z/16;
printf("x: %i\ty: %i\tz: %i\n", thetaX, thetaY, thetaZ);
return thetaX;     
}

void i2c_example_master_init()
{
    int i2c_master_port = I2C_EXAMPLE_MASTER_NUM;
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = static_cast<gpio_num_t>(I2C_EXAMPLE_MASTER_SDA_IO);
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = static_cast<gpio_num_t>(I2C_EXAMPLE_MASTER_SCL_IO);
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_EXAMPLE_MASTER_FREQ_HZ;
    i2c_param_config(static_cast<i2c_port_t>(i2c_master_port), &conf);
    i2c_driver_install(static_cast<i2c_port_t>(i2c_master_port), conf.mode,
                       I2C_EXAMPLE_MASTER_RX_BUF_DISABLE,
                       I2C_EXAMPLE_MASTER_TX_BUF_DISABLE, 0);
}