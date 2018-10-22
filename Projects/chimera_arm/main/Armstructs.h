#ifndef ARMSTRUCTS_H_
#define ARMSTRUCTS_H_
#include <stdlib.h>

using namespace std;
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
//#include "Protolobe.h"
//#include "RTOS_Tasks.h"
#include "ArmDefines.h"
#include "driver/ledc.h"
#include "freertos/queue.h"
#include "esp_attr.h"
#include "soc/rtc.h"
#include "driver/i2c.h"
xQueueHandle queue_handle =  xQueueCreate(6, sizeof(int));
xQueueHandle error_queue  = xQueueCreate(50, sizeof(char));
ledc_timer_config_t ledc_timer1 = {
	speed_mode: LEDC_LOW_SPEED_MODE,           // timer mode
        bit_num: LEDC_TIMER_10_BIT, // resolution of PWM duty
	timer_num: LEDC_TIMER_0,            // timer index
        freq_hz: 25000                      // frequency of PWM signal       
    };
ledc_timer_config_t ledc_timer2 = {
	speed_mode: LEDC_HIGH_SPEED_MODE,           // timer mode
        bit_num: LEDC_TIMER_10_BIT, // resolution of PWM duty
	timer_num: LEDC_TIMER_1,            // timer index
        freq_hz: 400                 // frequency of PWM signal       
    };

ledc_timer_config_t ledc_timer3 = {
	speed_mode: LEDC_HIGH_SPEED_MODE,           // timer mode
        bit_num: LEDC_TIMER_12_BIT, // resolution of PWM duty
	timer_num: LEDC_TIMER_2,            // timer index
        freq_hz: 50                    // frequency of PWM signal       
    };

ledc_channel_config_t BASE_CHANNEL= {
        
		gpio_num : BASE_PWM,
		speed_mode: LEDC_LOW_SPEED_MODE,
            channel: LEDC_CHANNEL_0,
		intr_type: LEDC_INTR_DISABLE,
		timer_sel: LEDC_TIMER_0,
            duty: 0                             
    };
    ledc_channel_config_t ELBOW_CHANNEL= {
        
		gpio_num : ELBOW_PWM,
		speed_mode: LEDC_LOW_SPEED_MODE,
            channel: LEDC_CHANNEL_1,
		intr_type: LEDC_INTR_DISABLE,
		timer_sel: LEDC_TIMER_0,
            duty: 0                             
    };
    ledc_channel_config_t CAM_CHANNEL= {
        
		gpio_num : CAM_PWM,
		speed_mode: LEDC_HIGH_SPEED_MODE,
            channel: LEDC_CHANNEL_0,
		intr_type: LEDC_INTR_DISABLE,
		timer_sel: LEDC_TIMER_2,
            duty: 0                             
    };
    ledc_channel_config_t SHOULDER_CHANNEL= {
        
		gpio_num : SHOULDER_PWM,
		speed_mode: LEDC_HIGH_SPEED_MODE,
            channel: LEDC_CHANNEL_1,
		intr_type: LEDC_INTR_DISABLE,
		timer_sel: LEDC_TIMER_1,
            duty: 0                             
    };
    ledc_channel_config_t CLAW_CHANNEL1= {
        
		gpio_num : CLAWA_PWM,
		speed_mode: LEDC_HIGH_SPEED_MODE,
            channel: LEDC_CHANNEL_2,
		intr_type: LEDC_INTR_DISABLE,
		timer_sel: LEDC_TIMER_1,
            duty: 0                             
    };
    ledc_channel_config_t CLAW_CHANNEL2= {
        
		gpio_num : CLAWB_PWM,
		speed_mode: LEDC_HIGH_SPEED_MODE,
            channel: LEDC_CHANNEL_3,
		intr_type: LEDC_INTR_DISABLE,
		timer_sel: LEDC_TIMER_1,
            duty: 0                             
    };
    ledc_channel_config_t ROT_CHANNEL1= {
        
		gpio_num : ROTA_PWM,
		speed_mode: LEDC_HIGH_SPEED_MODE,
            channel: LEDC_CHANNEL_4,
		intr_type: LEDC_INTR_DISABLE,
		timer_sel: LEDC_TIMER_1,
            duty: 0                             
    };
    ledc_channel_config_t ROT_CHANNEL2= {
        
		gpio_num : ROTB_PWM,
		speed_mode: LEDC_HIGH_SPEED_MODE,
            channel: LEDC_CHANNEL_5,
		intr_type: LEDC_INTR_DISABLE,
		timer_sel: LEDC_TIMER_1,
            duty: 0                             
    };
    ledc_channel_config_t PITCH_CHANNEL1= {
        
		gpio_num : PITCHA_PWM,
		speed_mode: LEDC_HIGH_SPEED_MODE,
            channel: LEDC_CHANNEL_6,
		intr_type: LEDC_INTR_DISABLE,
		timer_sel: LEDC_TIMER_1,
            duty: 0                             
    };
    ledc_channel_config_t PITCH_CHANNEL2= {
        
		gpio_num : PITCHB_PWM,
		speed_mode: LEDC_HIGH_SPEED_MODE,
            channel: LEDC_CHANNEL_7,
		intr_type: LEDC_INTR_DISABLE,
		timer_sel: LEDC_TIMER_1,
            duty: 0                             
    };
static void i2c_example_master_init();
void gpio_init(){
			
 // GPIO INPUT

   //gpio_set_direction(SHOULDER_MAG,    GPIO_MODE_INPUT);
 //  gpio_set_direction(WRIST_MAG,       GPIO_MODE_INPUT);
   gpio_set_direction(WRIST_ENCODER_A, GPIO_MODE_INPUT);
   gpio_set_direction(WRIST_ENCODER_B, GPIO_MODE_INPUT);

   // GPIO OUTPUT

   gpio_set_direction(SHOULDER_DIR_PIN, GPIO_MODE_OUTPUT);
gpio_set_direction(CAM_MUX_SEL,     GPIO_MODE_OUTPUT);

ledc_timer_config(&ledc_timer1);
ledc_timer_config(&ledc_timer2);
ledc_timer_config(&ledc_timer3);
ledc_channel_config(&BASE_CHANNEL);
ledc_channel_config(&ELBOW_CHANNEL);
ledc_channel_config(&CAM_CHANNEL);
ledc_channel_config(&SHOULDER_CHANNEL);
ledc_channel_config(&CLAW_CHANNEL1);
ledc_channel_config(&CLAW_CHANNEL2);
ledc_channel_config(&ROT_CHANNEL1);
ledc_channel_config(&ROT_CHANNEL2);
ledc_channel_config(&PITCH_CHANNEL1);
ledc_channel_config(&PITCH_CHANNEL2);
}
static void i2c_example_master_init()
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

#endif
