#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "DriveSystem.h"
#include "Servo.hpp"
#include "Motor.hpp"
extern "C" {
#include "Wifi.h"
#include "Structure.h"
}

const uint32_t motor_gpio[4] = {32,33,2,27};
const uint32_t motor_frequency = 40000;
const uint32_t motor_channel[4] = {0,1,2,3};
const uint32_t motor_max_rpm = 150;
const uint32_t motor_direction_pin[4] = {23,1,5,16};
const uint32_t motor_brake_pin[4] = {22,3,18,17};
const uint32_t motor_hall_pin[4] = {36,38,34,35};

const uint32_t servo_gpio[4] = {25,4,0,26};
const uint32_t servo_frequency = 50;
const uint32_t servo_channel[4] = {4,5,6,7};
const uint32_t servo_max_pulse = 2500;
const uint32_t servo_min_pulse = 510;
const uint32_t servo_max_degree = 180; 
uint32_t current_timestamp;



 Motor motor[4] = 
  {      
    {motor_gpio[2],motor_frequency,motor_channel[2],motor_max_rpm,motor_direction_pin[2],motor_brake_pin[2]},
    {motor_gpio[3],motor_frequency,motor_channel[3],motor_max_rpm,motor_direction_pin[3],motor_brake_pin[3]},
    {motor_gpio[1],motor_frequency,motor_channel[1],motor_max_rpm,motor_direction_pin[1],motor_brake_pin[1]},
    {motor_gpio[0],motor_frequency,motor_channel[0],motor_max_rpm,motor_direction_pin[0],motor_brake_pin[0]}
  };
  Servo servo[4] =
  {
    {servo_gpio[0],servo_frequency,servo_channel[0],servo_max_pulse,servo_min_pulse,servo_max_degree},
    {servo_gpio[1],servo_frequency,servo_channel[1],servo_max_pulse,servo_min_pulse,servo_max_degree},
    {servo_gpio[2],servo_frequency,servo_channel[2],servo_max_pulse,servo_min_pulse,servo_max_degree},
    {servo_gpio[3],servo_frequency,servo_channel[3],servo_max_pulse,servo_min_pulse,servo_max_degree}
  };


extern "C" void vBrakeTask(void *pvParameters)
{
  while(1)
  {
    printf(" Brake : %i \n",read_data.data[BRAKE]);
    if (read_data.data[BRAKE] == 1 )
    {
      printf("MANUAL BRAKE ASSERT\n");
      motor[0].Motor_set_brake(0);
      motor[1].Motor_set_brake(0);
      motor[2].Motor_set_brake(0);
      motor[3].Motor_set_brake(0); 
    }
    // else
    // {
    //   printf("MANUAL BRAKE DISENGAGE\n");
    //   motor[0].Motor_set_brake(1);
    //   motor[1].Motor_set_brake(1);
    //   motor[2].Motor_set_brake(1);
    //   motor[3].Motor_set_brake(1);
    // }
    vTaskDelay(1);
  }  
}


extern "C" void vWriteTask(void *pvParameters)
{
  while(1)
  {
    send_data_wifi(SEND_ITEM_SIZE);
    strncpy(SYSTEM_DEBUG_MSG, "SENDING", 49);
    vTaskDelay(1);
  }  
}

extern "C" void vReadTask(void *pvParameters)
{
  while(1)
  {
    //printf("read_task\n");
    current_timestamp = read_data_wifi(READ_ITEM_SIZE);
    vTaskDelay(1);
  }  
}

extern "C" void vDriveTask(void *pvParameters)
{
  uint32_t previous_timestamp=0;
  bool initial_flag = true;
  read_data.data[0] = read_data.data[0]+1;
 
  uint32_t counter = 0;
  while(1)
  {
    //current_timestamp = read_data_wifi(READ_ITEM_SIZE);
    printf("%i\n",current_timestamp );
    if(initial_flag == false)
    {
      //check for heartbeat, if no heartbeat go to idle.
      if(current_timestamp != previous_timestamp)
      {
        //printf("Drive Execution \n");  
        DriveSystem_mode(read_data.data[M],motor,servo);
        counter=0;
      }
      else if(counter >= 200)
      {
        printf("IDLE\n");
        DriveSystem_idle(motor);
      }
      else
      {
        //printf("It HIT\n");
        DriveSystem_mode(read_data.data[M],motor,servo);
        counter++;
      }
    }
    else  
    {
      DriveSystem_mode(read_data.data[M],motor,servo);
    }
    initial_flag = false;
    previous_timestamp = current_timestamp;
    vTaskDelay(1);
  }
}

