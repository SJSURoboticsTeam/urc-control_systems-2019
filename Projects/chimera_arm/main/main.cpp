#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "Arduino.h"
#include "EEPROM.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "RTOStasks.h"
#include "Source.h"
#include "constants.h"
#include <WiFi.h>
#include <FS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>



#include <stdlib.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/ledc.h"
#include "freertos/queue.h"
#include "esp_attr.h"
#include "soc/rtc.h"
#include "driver/i2c.h"
#include "ArmDefines.h"
//#include "Components.h"
//#include "RTOS_Tasks.h"
extern "C"{
#include "Wifi.h"
#include "Structure.h"
}

extern "C" void vReadDataTask(void *pvParameters);

//extern "C" void error_task(void *pvParameters);

void Arm_claw_task(void *pvParameters);

void Arm_elbow_task(void *pvParameters);

void Arm_cam_task(void *pvParameters);

void Arm_wrist_task(void *pvParameters);

void Arm_shoulder_task(void *pvParameters);

void Gimbal_task(void *pvParameters);

void Arm_rotunda_task(void *pvParameters);
 	
void gpio_init();

extern "C" void Wifi_init();



extern "C" void app_main()
{


 printf("Protolobe Testing Wifi \n");
 Wifi_init();

//i2c_example_master_init();
gpio_init();


    //xTaskCreate(error_task, "Error_handler", 2048, NULL, 2, NULL);
    xTaskCreate(vReadDataTask   , "vReadDataTask"      , 2048, NULL, 1, NULL);
 
xTaskCreate(Gimbal_task, "Gimbal", 2048, NULL, 1, NULL);
   xTaskCreate(Arm_rotunda_task, "Arm_rotunda_task", 2048, NULL, 1, NULL);
   xTaskCreate(Arm_shoulder_task, "Arm_shoulder_task", 2048, NULL, 1, NULL);
    xTaskCreate(Arm_wrist_task, "Arm_wrist_task", 2048, NULL, 1, NULL);
    xTaskCreate(Arm_cam_task, "Arm_cam_task", 2048, NULL, 1, NULL);
    xTaskCreate(Arm_elbow_task, "Arm_elbow_task", 2048, NULL, 1, NULL);
//    xTaskCreate(Arm_claw_task, "Arm_claw_task", 2048, NULL, 1, NULL);
 
//testing
uint8_t data[6];
int16_t X, Y, Z;
int16_t thetaX, thetaY, thetaZ;
esp_err_t ret;
int temp;


i2c_cmd_handle_t a_read; 
//angle setting
i2c_cmd_handle_t cmd;


/*
cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, AS5600_SENSOR_ADDR << 1 | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, 0x01, ACK_CHECK_EN); //ZPOS 12 bit addr
    i2c_master_write_byte(cmd, 0x00, ACK_CHECK_EN); //raw value of desired start angle
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, AS5600_SENSOR_ADDR << 1 | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, 0x02, ACK_CHECK_EN); //12 bit addr
    i2c_master_write_byte(cmd, 0x00, ACK_CHECK_EN); //raw value of desired start angle
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, AS5600_SENSOR_ADDR << 1 | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, 0x03, ACK_CHECK_EN); // MPOS 12 bit addr
    i2c_master_write_byte(cmd, 0x03, ACK_CHECK_EN); //raw value of desired start angle
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, AS5600_SENSOR_ADDR << 1 | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, 0x04, ACK_CHECK_EN); //12 bit addr
    i2c_master_write_byte(cmd, 0xFF, ACK_CHECK_EN); //raw value of desired max angle
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

 
*/
temp = 0;
     

vTaskDelay(200 / portTICK_RATE_MS);
}
extern "C" void Wifi_init(){
   //change access point name for ap mode
   set_wifi_ap("Arm", 4);
   //set static ip address
   set_static_ip("192.168.0.5");
   //initilize Wifi
   create_wifi_task();
   //Initializing struct to 0
   memset(&send_data, 0, sizeof(send_data));
   memset(&read_data, 0, sizeof(read_data));
   //Read Data Initialization
   read_data.data[read_shoulder] = 90;
   //Setting element in send data
   send_data.data[send_rotunda]=0; 
   send_data.data[send_shoulder]=1023;
   send_data.data[send_elbow]=500; 
   send_data.data[send_wrist_pitch]=200; 
   send_data.data[send_wrist_rotation]=800; 
   send_data.data[send_cam]=10; 
   send_data_wifi(SEND_ITEM_SIZE);
}

						//Base(-150:150)
                        //shoulder(0:180)
                        //elbow(-150:150)
                        //wrist_pitch(-150:150)
                        //wrist_rot(0:359)
                        //cam(-90:90)