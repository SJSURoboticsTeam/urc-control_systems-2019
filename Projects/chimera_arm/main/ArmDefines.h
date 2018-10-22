#ifndef ARM_DEFINES_H_
#define ARM_DEFINES_H_
// #include <stdlib.h>

// using namespace std;
// #include <stdio.h>
// #include <string.h>
// #include <math.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "esp_system.h"
// #include "esp_spi_flash.h"
// #include "RTOS_Tasks.h"
// #include "driver/ledc.h"
// #include "freertos/queue.h"
// #include "esp_attr.h"
// #include "soc/rtc.h"
// #include "driver/i2c.h"
// extern "C" {

// #ifndef DEBUG
//    #include "Wifi.h"
//    #endif

// #include "Structure.h"

#define ELBOW_PWM (4)
#define BASE_PWM (16)
#define CAM_PWM (0)
#define SHOULDER_PWM (23)
#define CLAWA_PWM (5)
#define CLAWB_PWM (18)
#define ROTA_PWM (32)
#define ROTB_PWM (33)
#define PITCHA_PWM (25)
#define PITCHB_PWM (26)

#define I2C_EXAMPLE_MASTER_SCL_IO    (19)    /*!< gpio number for I2C master clock */
#define I2C_EXAMPLE_MASTER_SDA_IO    (21)    /*!< gpio number for I2C master data  */
#define I2C_EXAMPLE_MASTER_NUM I2C_NUM_1   /*!< I2C port number for master dev */
#define I2C_EXAMPLE_MASTER_TX_BUF_DISABLE   0   /*!< I2C master do not need buffer */
#define I2C_EXAMPLE_MASTER_RX_BUF_DISABLE   0   /*!< I2C master do not need buffer */
#define I2C_EXAMPLE_MASTER_FREQ_HZ    100000     /*!< I2C master clock frequency */

#define CAM_MUX_SEL GPIO_NUM_2
#define SHOULDER_DIR_PIN GPIO_NUM_22
//#define SHOULDER_MAG GPIO_NUM_18
//#define WRIST_MAG GPIO_NUM_23
#define WRIST_ENCODER_A GPIO_NUM_27
#define WRIST_ENCODER_B GPIO_NUM_17

#define OPTIC_SENSOR_ADDR  0x19    /*!< slave address for optic sensor */
#define BNO055_SENSOR_ADDR  0x29    /*!< slave address for IMU */
#define AS5600_SENSOR_ADDR  0x36    /*!< slave address for AS5600 sensor */
#define OPR_MODE 0x3D         // address for operation mode register
#define UNIT_SEL 0x3B         // address for unit select register
#define WRITE_BIT  I2C_MASTER_WRITE /*!< I2C master write */
#define READ_BIT   I2C_MASTER_READ  /*!< I2C master read */
#define ACK_EN   0x1     /*!< I2C master will check ack from slave*/
#define ACK_CHECK_EN   0x1
#define ACK_DIS  0x0     /*!< I2C master will not check ack from slave */
#define ACK_VAL    0x0         /*!< I2C ack value */
#define NACK_VAL   0x1         /*!< I2C nack value */

#endif

