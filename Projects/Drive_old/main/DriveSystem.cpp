#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "DriveSystem.h"
#include "RTOS_Tasks.h"
#include "Servo.hpp"
#include "AngleLookup.h"
extern "C" {
#include "Wifi.h"
#include "Structure.h"
}

#define PI 3.14159265

int32_t debug=-100;
uint32_t previous_rpm = 0;
const uint32_t filterLength = 50;
const float rpm_weight = .1;
const uint32_t point = 220;
const uint8_t speed_divider = 3;
uint32_t filter[filterLength] = {0};
uint8_t previous_mode = 1;
uint16_t buffer[point]; 

struct Servo_Buffer{
   uint16_t servo1_buffer[point];
   uint16_t servo2_buffer[point];
   uint16_t servo3_buffer[point];
   uint16_t servo4_buffer[point];
   uint32_t servo1_avg;
   uint32_t servo2_avg;
   uint32_t servo3_avg;
   uint32_t servo4_avg;
}Servo_Avg;


/*-------------------------Put your code here---------------*/

extern "C" void DriveSystem_init()
{
   set_wifi_ap("\xF0\x9F\x98\xBB",4);
   set_static_ip("192.168.0.69");
   //initilize Wifi
   create_wifi_task();
   //Initializing struct to 0
   memset(&send_data, 0, sizeof(send_data));
   memset(&read_data, 0, sizeof(read_data));
   memset(buffer, 0, sizeof(buffer));
   //Setting element in send data
   send_data.data[0]=-100;
   send_data.data[1]=2000000;
   vTaskDelay(500);
   //Sending data in send_data to mission control
   send_data_wifi(SEND_ITEM_SIZE);
}

extern "C" void DriveSystem_mode(int mode, Motor motor_object[], Servo servo_object[])
{
   //printf("Mode %i\n",mode );
   if(previous_mode != mode )
   {
      DriveSystem_idle(motor_object);
   }
   previous_mode = mode;
   switch(mode)
   {
      case 5: DriveSystem_drive(motor_object, servo_object);  break;
      case 2: DriveSystem_rotate(motor_object, servo_object); break;
      case 3: DriveSystem_crab(motor_object, servo_object);   break;
      case 4: DriveSystem_debug(motor_object, servo_object);  break;
      case 1: DriveSystem_tank(motor_object, servo_object);  break;

      default: break;
   }
}
extern "C" void DriveSystem_drive(Motor motor_object[], Servo servo_object[])
{
   printf("Drive\n");
   uint32_t angle  = read_data.data[SA1];//set to SA1 to disable speend increase
   //if(debug != read_data.data[SA3] ){
      //printf("SA3 : %i\n", read_data.data[SA3] );
      if(read_data.data[SA3] <= 45) {  read_data.data[SA3] = 45; }
      if(read_data.data[SA3] >= 135){  read_data.data[SA3] = 135;}
      if(read_data.data[SA3] >= 90)
      // {
      //    servo_object[0].Servo_write_angle(angle_lookup[read_data.data[SA3]][2]);//0
      //    servo_object[1].Servo_write_angle(angle_lookup[read_data.data[SA3]][3]);//1
      //    servo_object[2].Servo_write_angle(angle_lookup[read_data.data[SA3]][0]);//2
      //    servo_object[3].Servo_write_angle(angle_lookup[read_data.data[SA3]][1]);//3
      // }
      // else
      // {
      //    servo_object[0].Servo_write_angle(angle_lookup[read_data.data[SA3]][3]);//0
      //    servo_object[1].Servo_write_angle(angle_lookup[read_data.data[SA3]][2]);//1
      //    servo_object[2].Servo_write_angle(angle_lookup[read_data.data[SA3]][1]);//2
      //    servo_object[3].Servo_write_angle(angle_lookup[read_data.data[SA3]][0]);//3
      // }

      //DriveSytem_AverageServo(servo_object);
      debug = read_data.data[SA3];
   //}
   DriveSystem_slowServoDrive(servo_object);
   DriveSystem_checkRPM(motor_object,angle);
}

extern "C" void DriveSystem_checkRPM(Motor motor_object[], uint8_t angle)
{
   uint32_t current_rpm = abs(read_data.data[0]);
   uint32_t totals = (previous_rpm*(1-rpm_weight)+(current_rpm*rpm_weight));
   previous_rpm = totals;
   int8_t direction = read_data.data[0];
   if(direction < 0){direction = 1;}
   else if(direction > 0){direction = -1; }
   else{};
   //printf("Total %i \n", totals);
   int32_t total = totals;
   // if(angle > 10 && angle < 80)
   // {
   //    motor_object[0].Motor_set_rpm(direction*(total+5));
   //    motor_object[1].Motor_set_rpm(direction*total);
   //    motor_object[2].Motor_set_rpm(direction*total);
   //    motor_object[3].Motor_set_rpm(direction*(total+5)); 
   // }
   // else if(angle > 110 && angle < 170)
   // {
   //    motor_object[0].Motor_set_rpm(direction*total);
   //    motor_object[1].Motor_set_rpm(direction*(total+5));
   //    motor_object[2].Motor_set_rpm(direction*(total+5));
   //    motor_object[3].Motor_set_rpm(direction*total); 
   // }
   // else
   // {
   //    motor_object[0].Motor_set_rpm(direction*total);
   //    motor_object[1].Motor_set_rpm(direction*total);
   //    motor_object[2].Motor_set_rpm(direction*total);
   //    motor_object[3].Motor_set_rpm(direction*total); 
   // }
   //motor_object[0].Motor_set_rpm(direction*(total*.83));//motor 3 
   motor_object[0].Motor_set_rpm(-1*direction*(total));
   motor_object[1].Motor_set_rpm(-1*direction*(total));
   //motor_object[2].Motor_set_rpm(direction*(total*.59));
   motor_object[2].Motor_set_rpm(-1*direction*(total));
   motor_object[3].Motor_set_rpm(-1*direction*(total)); //motor 4
   // if(totals == 0)
   // {
   //    //vTaskDelay(5*total);
   //    motor_object[0].Motor_set_brake(0);
   //    motor_object[1].Motor_set_brake(0);
   //    motor_object[2].Motor_set_brake(0);
   //    motor_object[3].Motor_set_brake(0); 
   // }
   // else
   // {
   //    motor_object[0].Motor_set_brake(1);
   //    motor_object[1].Motor_set_brake(1);
   //    motor_object[2].Motor_set_brake(1);
   //    motor_object[3].Motor_set_brake(1); 
   // }
   DriveSystem_assertBrake(motor_object, total);
}

extern "C" void DriveSystem_idle(Motor motor_object[])
{ 
   read_data.data[0] = 0 ;
   DriveSystem_checkRPM(motor_object,90);

}
extern "C" int DriveSystem_rotate(Motor motor_object[], Servo servo_object[])
{
   uint32_t error_flag = 0;
   // if(read_data.data[SA1] == 45 && read_data.data[SA2] == 135)
   // {
   //    printf("Rotate\n");
      uint32_t current_rpm = (abs(read_data.data[0]))/1;
      uint32_t total = (previous_rpm*(1-rpm_weight)+(current_rpm*rpm_weight));
      previous_rpm = total;
      int totals = total;
      int8_t direction = read_data.data[0];
      uint16_t angle = read_data.data[SA3];


      //motor_object[0].Motor_set_rpm(direction*(total*.83));//motor 3 
      motor_object[0].Motor_set_rpm(-1*direction*(total));
      motor_object[1].Motor_set_rpm(-1*direction*(total));
      //motor_object[2].Motor_set_rpm(direction*(total*.59));


      // if(direction < 0 || angle > 90 )
      // {
      //    motor_object[0].Motor_set_rpm(-1*(totals));//motor 1
      //    motor_object[1].Motor_set_rpm((totals));//motor 0
      //    motor_object[2].Motor_set_rpm(-1*(totals));//motor 2
      //    motor_object[3].Motor_set_rpm((totals));//motor 3
      // }
      // else
      // { 
      //    motor_object[0].Motor_set_rpm(totals);
      //    motor_object[1].Motor_set_rpm(-1*(totals));
      //    motor_object[2].Motor_set_rpm(totals);
      //    motor_object[3].Motor_set_rpm(-1*(totals));
      // }

      if(direction < 0 || angle > 90 )
      {
         motor_object[0].Motor_set_rpm(-1*(totals/5));//motor 1
         motor_object[1].Motor_set_rpm((totals));//motor 0
         motor_object[2].Motor_set_rpm(-1*totals);//motor 2
         motor_object[3].Motor_set_rpm((totals/4));//motor 3
         if(totals == 0)
         {
            vTaskDelay(100);
            motor_object[0].Motor_set_brake(0);
            motor_object[1].Motor_set_brake(0);
            motor_object[2].Motor_set_brake(0);
            motor_object[3].Motor_set_brake(0); 
         }
         else
         {
            motor_object[0].Motor_set_brake(1);
            motor_object[1].Motor_set_brake(1);
            motor_object[2].Motor_set_brake(1);
            motor_object[3].Motor_set_brake(1); 
         }
      }
      else
      { 
         motor_object[0].Motor_set_rpm(totals);
         motor_object[1].Motor_set_rpm(-1*(totals/8));//motor 3 =0  
         motor_object[2].Motor_set_rpm(totals/5);//motor 1 = 0
         motor_object[3].Motor_set_rpm(-1*total);
         if(totals == 0)
         {
            vTaskDelay(100);
            motor_object[0].Motor_set_brake(0);
            motor_object[1].Motor_set_brake(0);
            motor_object[2].Motor_set_brake(0);
            motor_object[3].Motor_set_brake(0); 
         }
         else
         {
            motor_object[0].Motor_set_brake(1);
            motor_object[1].Motor_set_brake(1);
            motor_object[2].Motor_set_brake(1);
            motor_object[3].Motor_set_brake(1); 
         }
      }
      // if(debug != read_data.data[SA3])
      // {
      //    servo_object[1].Servo_write_angle(read_data.data[5]);
      //    servo_object[3].Servo_write_angle(read_data.data[7]);
      //    servo_object[0].Servo_write_angle(read_data.data[4]);
      //    servo_object[2].Servo_write_angle(read_data.data[6]);      
      //    debug = read_data.data[SA3];
      // }
      //DriveSystem_assertBrake(motor_object,total);
   // }
   // else
   // {
   //    error_flag = 1;
   // }
   return error_flag;

}
extern "C" int DriveSystem_crab(Motor motor_object[], Servo servo_object[])
{
   uint32_t error_flag = 0;
   if( read_data.data[SA3] >= 0 && read_data.data[SA3] <= 180)
   {
      printf("Crab\n");
      for(uint32_t i = 0; i < 4 ; i++)
      {
         motor_object[i].Motor_set_rpm(read_data.data[i]);
      }
      if(debug != read_data.data[SA3] ){
         servo_object[0].Servo_write_angle(read_data.data[SA3]);
         servo_object[1].Servo_write_angle(read_data.data[SA3]);
         servo_object[2].Servo_write_angle(read_data.data[SA3]);
         servo_object[3].Servo_write_angle(read_data.data[SA3]);
         //DriveSystem_slowServo(servo_object);
         debug = read_data.data[SA3];
      }
   }
   else
   {
      error_flag = 1;
   }
   return error_flag;

}
extern "C" void DriveSystem_debug(Motor motor_object[], Servo servo_object[])
{   
   uint8_t wheel_num = read_data.data[DW];
   if(wheel_num >= 0 && wheel_num < 4)
   {
      printf("Deug Wheel : %i \n",wheel_num);
      motor_object[(read_data.data[DW])].Motor_set_rpm(read_data.data[wheel_num]);
      // servo_object[(read_data.data[DW])].Servo_write_angle(read_data.data[wheel_num+4]);
      if(debug != read_data.data[wheel_num+4] )
      {
         servo_object[(read_data.data[DW])].Servo_write_angle(read_data.data[wheel_num+4]);
         motor_object[(read_data.data[DW])].Motor_set_rpm(read_data.data[wheel_num]);
         debug = read_data.data[wheel_num+4];
         DriveSystem_assertBrake(motor_object,read_data.data[DW]);
      }

   }
}
/*------------------------Code end here-------------------*/

extern "C" void DriveSystem_slowServo(Servo servo_object[])
{
   uint32_t avg = 0;
   for(int i = point-1; i > 0; i--)
   {
      buffer[i] = buffer[i-1];   
   }
   buffer[0] = read_data.data[SA3];
   for(int i= 0; i < point; i++)
   {
      //printf("Buffer %i : %i \n",i, buffer[i]);
      avg += buffer[i];  
   }
   //printf("Sum %i \n",avg );
   avg = avg/point;
   //printf("SA3 : %i AVG:  %i \n", read_data.data[SA3], avg);
   servo_object[0].Servo_write_angle(avg);
   servo_object[1].Servo_write_angle(avg);
   servo_object[2].Servo_write_angle(avg);
   servo_object[3].Servo_write_angle(read_data.data[SA3]);
}

extern "C" void DriveSystem_slowServoDrive(Servo servo_object[])
{
   Servo_Avg.servo1_avg = 0;
   Servo_Avg.servo2_avg = 0;
   Servo_Avg.servo3_avg = 0;

   for(int i = point-1; i > 0; i--)
   {
      Servo_Avg.servo1_buffer[i] = Servo_Avg.servo1_buffer[i-1];  
      Servo_Avg.servo2_buffer[i] = Servo_Avg.servo2_buffer[i-1];   
      Servo_Avg.servo3_buffer[i] = Servo_Avg.servo3_buffer[i-1];   
   }

   Servo_Avg.servo1_buffer[0] =  angle_lookup[read_data.data[SA3]][2];
   Servo_Avg.servo2_buffer[0] =  angle_lookup[read_data.data[SA3]][3];
   Servo_Avg.servo3_buffer[0] =  angle_lookup[read_data.data[SA3]][0];

   for(int i= 0; i < point; i++)
   {
      //printf("Buffer %i : %i \n",i, buffer[i]);
      Servo_Avg.servo1_avg += Servo_Avg.servo1_buffer[i];
      Servo_Avg.servo2_avg += Servo_Avg.servo2_buffer[i];
      Servo_Avg.servo3_avg += Servo_Avg.servo3_buffer[i];
   }
   //printf("Sum %i \n",avg );
   Servo_Avg.servo1_avg = Servo_Avg.servo1_avg/point;
   Servo_Avg.servo2_avg = Servo_Avg.servo2_avg/point;
   Servo_Avg.servo3_avg = Servo_Avg.servo3_avg/point;   
   //printf("SA1 : %i SA2: %i SA3: %i \n", Servo_Avg.servo1_avg, Servo_Avg.servo2_avg, Servo_Avg.servo3_avg);
   //printf("AVG : %i \n", avg );
   servo_object[0].Servo_write_angle(Servo_Avg.servo1_avg);
   servo_object[1].Servo_write_angle(Servo_Avg.servo2_avg);
   servo_object[2].Servo_write_angle(Servo_Avg.servo3_avg);
   servo_object[3].Servo_write_angle(angle_lookup[read_data.data[SA3]][1]);//3

}


extern "C" void DriveSystem_assertBrake(Motor motor_object[],uint8_t current_speed)
{
   if(current_speed == 0)
   {
      //vTaskDelay(5*total);
      motor_object[0].Motor_set_brake(0);
      motor_object[1].Motor_set_brake(0);
      motor_object[2].Motor_set_brake(0);
      motor_object[3].Motor_set_brake(0); 
   }
   else
   {
      motor_object[0].Motor_set_brake(1);
      motor_object[1].Motor_set_brake(1);
      motor_object[2].Motor_set_brake(1);
      motor_object[3].Motor_set_brake(1); 
   }
}

extern "C" void DriveSystem_tank(Motor motor_object[], Servo servo_object[])
{
   printf("Tank\n");
   DriveSystem_checkRPMTank(motor_object);
}

extern "C" void DriveSystem_checkRPMTank(Motor motor_object[])
{
   uint32_t current_rpm = (abs(read_data.data[0]))/speed_divider;
   uint32_t totals = (previous_rpm*(1-rpm_weight)+(current_rpm*rpm_weight));
   uint32_t angle = read_data.data[SA3];
   previous_rpm = totals;
   int8_t direction = read_data.data[0];
   if(direction < 0){direction = -1;}
   else if(direction > 0){direction = 1; }
   else{};
   int32_t total = totals;
   //motor_object[0].Motor_set_rpm(direction*(total*.83));//motor 3 
   //motor_object[0].Motor_set_rpm(-1*direction*(total));
   //motor_object[1].Motor_set_rpm(-1*direction*(total));
   //motor_object[2].Motor_set_rpm(direction*(total*.59));
   //motor_object[2].Motor_set_rpm(-1*direction*(total));
   //motor_object[3].Motor_set_rpm(-1*direction*(total)); //motor 4

   if(angle > 20 && angle < 70)
   {
      motor_object[0].Motor_set_rpm(direction*(total)); //motor 2
      motor_object[1].Motor_set_rpm(direction*total);
      motor_object[2].Motor_set_rpm(direction*(total)); //motor 1
      motor_object[3].Motor_set_rpm(direction*(total)); 
   }
   else if(angle > 110 && angle < 160)
   {
      motor_object[0].Motor_set_rpm(direction*(total));
      motor_object[1].Motor_set_rpm(direction*(total));
      motor_object[2].Motor_set_rpm(direction*(total));
      motor_object[3].Motor_set_rpm(direction*(total)); 
   }
   else
   {
      motor_object[0].Motor_set_rpm(direction*total*.90);
      motor_object[1].Motor_set_rpm(direction*total);
      motor_object[2].Motor_set_rpm(direction*total);
      motor_object[3].Motor_set_rpm(direction*total); 
   }

   DriveSystem_assertBrake(motor_object, total);
}
