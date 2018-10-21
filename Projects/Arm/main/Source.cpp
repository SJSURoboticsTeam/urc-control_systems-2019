#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Source.h"
extern "C" {
#ifndef DEBUG
   #include "Wifi.h"
   #endif
#include "Structure.h"
}


/*-------------------------Put your code here---------------*/

extern "C" void Arm_Init()
{
   //change access point name for ap mode
   set_wifi_ap("Arm", 4);
   //set static ip address
   set_static_ip("192.168.0.5");
   //initilize Wifi
   create_wifi_task();
   //Initializing struct to 0
   memset(&send_data, 0, sizeof(send_data));
   memset(&read_data, 0, sizeof(read_data));
   //Setting element in send data
   send_data.data[send_rotunda]=0; 
   send_data.data[send_shoulder]=0;
   send_data.data[send_elbow]=0; 
   send_data.data[send_wrist_pitch]=0; 
   send_data.data[send_wrist_rotation]=0; 
   // send_data.data[send_cam]=10; 
   // vTaskDelay(500);
   //Sending data in send_data to mission control
   send_data_wifi(SEND_ITEM_SIZE);

   // GPIO INPUT

   gpio_set_direction(SHOULDER_MAG,    GPIO_MODE_INPUT);
   // gpio_set_direction(WRIST_MAG,       GPIO_MODE_INPUT);
   gpio_set_direction(WRIST_ENCODER_A, GPIO_MODE_INPUT);
   gpio_set_direction(WRIST_ENCODER_B, GPIO_MODE_INPUT);

   // GPIO OUTPUT

    // gpio_config_t io_conf;
    // //disable interrupt
    // io_conf.intr_type = GPIO_INTR_DISABLE;
    // //set as output mode
    // io_conf.mode = GPIO_MODE_OUTPUT;
    // //bit mask of the pins that you want to set,e.g.GPIO18/19
    // io_conf.pin_bit_mask = SHOULDER_DIR_PIN;
    // //disable pull-down mode
    // io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    // //disable pull-up mode
    // io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    // //configure GPIO with the given settings
    // gpio_config(&io_conf);

   gpio_set_direction(SHOULDER_DIR_PIN, GPIO_MODE_OUTPUT);
   // gpio_set_direction(GEARBOX_1_IN_1,  GPIO_MODE_OUTPUT);
   // gpio_set_direction(GEARBOX_1_IN_2,  GPIO_MODE_OUTPUT);
   // gpio_set_direction(GEARBOX_2_IN_1,  GPIO_MODE_OUTPUT);
   // gpio_set_direction(GEARBOX_2_IN_2,  GPIO_MODE_OUTPUT);
   // gpio_set_direction(LEADSCREW_IN_1,  GPIO_MODE_OUTPUT);
   // gpio_set_direction(LEADSCREW_IN_2,  GPIO_MODE_OUTPUT);
   gpio_set_direction(CAM_MUX_SEL,     GPIO_MODE_OUTPUT);
}

// extern "C" inline long map(int16_t x, int16_t in_min, int16_t in_max, long out_min, long out_max)
// {
//   return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
// }

void SetShoulderAngle(Motor_t motor, uint16_t pwm, bool dir, ledc_mode_t mode)
{
   if(dir == false)
   {
      gpio_set_level(SHOULDER_DIR_PIN, REVERSE);
   }
   else if(dir == true)
   {
      gpio_set_level(SHOULDER_DIR_PIN, FORWARD);
   }

   // (pwm < 0) ? gpio_set_level(SHOULDER_DIR_PIN, REVERSE) : gpio_set_level(SHOULDER_DIR_PIN, FORWARD);
   ledc_set_duty(mode, motor.channel, pwm);
   ledc_update_duty(mode, motor.channel);
}

void SetDirection(gpio_num_t pin, uint8_t direction)
{

}

void SetWrist(Motor_t m1, Motor_t m2, int16_t pwm, int16_t current, ledc_mode_t mode)
{
   if(pwm > 50)
   {  
      ledc_set_duty(mode, m1.channel, MAX);
      ledc_set_duty(mode, m2.channel, (MAX - abs(pwm)));
   }
   else if(pwm < -50)
   {
      ledc_set_duty(mode, m2.channel, MAX);
      ledc_set_duty(mode, m1.channel, (MAX - abs(pwm)));
   }
   else
   {
      ledc_set_duty(mode, m1.channel, MAX);
      ledc_set_duty(mode, m2.channel, MAX);

   }
   ledc_update_duty(mode, m1.channel);
   ledc_update_duty(mode, m2.channel);
}

void RotateWrist(Motor_t m1, Motor_t m2, int16_t pwm, int16_t current, ledc_mode_t mode)
{
   // uint16_t pwm = 200;
   // if(dir == LEFT)
   // {  
   //    ledc_set_duty(mode, m1.channel, MAX);
   //    ledc_set_duty(mode, m2.channel, pwm);
   // }
   // else if(dir == RIGHT)
   // {
   //    ledc_set_duty(mode, m2.channel, MAX);
   //    ledc_set_duty(mode, m1.channel, pwm);
   // }
   // else
   // {
   //    ledc_set_duty(mode, m1.channel, MAX);
   //    ledc_set_duty(mode, m2.channel, MAX);

   // }
   // ledc_update_duty(mode, m1.channel);
   // ledc_update_duty(mode, m2.channel);

   if(pwm > 40)
   {  
      ledc_set_duty(mode, m1.channel, MAX);
      ledc_set_duty(mode, m2.channel, (MAX - abs(pwm)));
   }
   else if(pwm < -40)
   {
      ledc_set_duty(mode, m2.channel, MAX);
      ledc_set_duty(mode, m1.channel, (MAX - abs(pwm)));
   }
   else
   {
      ledc_set_duty(mode, m1.channel, MAX);
      ledc_set_duty(mode, m2.channel, MAX);

   }
   ledc_update_duty(mode, m1.channel);
   ledc_update_duty(mode, m2.channel);
}

void ClawControl(Motor_t m1, Motor_t m2, uint8_t ctrl, uint16_t pwm, ledc_mode_t mode)
{
   if(ctrl == OPEN)
   {  
      ledc_set_duty(mode, m1.channel, MAX);
      ledc_set_duty(mode, m2.channel, pwm);
   }
   else if(ctrl == CLOSE)
   {
      ledc_set_duty(mode, m2.channel, MAX);
      ledc_set_duty(mode, m1.channel, pwm);
   }
   else
   {
      ledc_set_duty(mode, m1.channel, MAX);
      ledc_set_duty(mode, m2.channel, MAX);

   }
   ledc_update_duty(mode, m1.channel);
   ledc_update_duty(mode, m2.channel);

}

uint16_t MagEncOffset(uint16_t in, int16_t offset, uint16_t min, uint16_t max)
{
   uint16_t out;
   out = in + offset;
   if(out < min)
   {
      out = out + max;
   }
   else if(out > max)
   {
      out = out - min;
   }
   return out;
}

void QuadEnc(int16_t *count, uint8_t *quad)
{
   *quad <<= 2;
   *quad |= gpio_get_level(WRIST_ENCODER_A) & 0x01;
   *quad |= (gpio_get_level(WRIST_ENCODER_B) << 1) & 0x02;
   const int8_t state[16] = {
                                 0,    // 00 00
                                 -1,   // 00 01
                                 1,    // 00 10
                                 0,    // 00 11
                                 1,    // 01 00
                                 0,    // 01 01
                                 0,    // 01 10
                                 -1,   // 01 11
                                 -1,   // 10 00
                                 0,    // 10 01
                                 0,    // 10 10
                                 1,    // 10 11
                                 0,    // 11 00
                                 1,    // 11 01
                                 -1,   // 11 10
                                 0     // 11 11
                              };
   printf("quad: %X state: %i ", *quad, (*quad & 0x0F));
   *count += state[(*quad & 0x0F)];
}

///////////////////////////////////////////////////////////////
//////                       PID                      /////////
///////////////////////////////////////////////////////////////

void Compute(PID_t *pid)
{
   printf("auto: %i ", pid->inAuto);
   // if(!pid->inAuto) return;
   unsigned long now = esp_log_timestamp();
   printf("now: %li ", now);
   int16_t timeChange = (now - pid->lastTime);
   printf("lastTime: %li timeChange: %i ", pid->lastTime, timeChange);
   if(timeChange >= pid->SampleTime)
   {
      /*Compute all the working error variables*/
      double error = pid->Setpoint - pid->Input;
      printf("error: %f ", error);

      pid->ITerm += (pid->ki * error);
      printf("ITerm: %f ", pid->ITerm);
      
      if(pid->ITerm > pid->outMax) 
      {
         pid->ITerm = pid->outMax;
      }
      else if(pid->ITerm < pid->outMin)
      {
         pid->ITerm = pid->outMin;
      }
      double dInput = (pid->Input - pid->lastInput);
      printf("dInput: %f ", dInput);

      double P = pid->kp * error;
      double I = pid->ITerm;
      double D = pid->kd * dInput;

      /*Compute PID Output*/
      pid->Output = pid->kp * error + pid->ITerm - pid->kd * dInput;
      if(pid->Output > pid->outMax) 
      {
         pid->Output = pid->outMax;
      }
      else if(pid->Output < pid->outMin) 
      {
         pid->Output = pid->outMin;
      }
      printf("Output: %f ", pid->Output);

      /*Remember some variables for next time*/
      pid->lastInput = pid->Input;
      pid->lastTime = now;
   }
   printf("\n");
}
  
void SetTunings(PID_t *pid, double Kp, double Ki, double Kd)
{
   if (Kp < 0 || Ki < 0|| Kd < 0) return;
 
  double SampleTimeInSec = ((double)pid->SampleTime)/1000;
   pid->kp = Kp;
   pid->ki = Ki * SampleTimeInSec;
   pid->kd = Kd / SampleTimeInSec;
 
  if(pid->controllerDirection == PID_REVERSE)
   {
      pid->kp = (0 - pid->kp);
      pid->ki = (0 - pid->ki);
      pid->kd = (0 - pid->kd);
   }
}
  
void SetSampleTime(PID_t *pid, int16_t NewSampleTime)
{
   if (NewSampleTime > 0)
   {
      double ratio  = (double)NewSampleTime
                      / (double)pid->SampleTime;
      pid->ki *= ratio;
      pid->kd /= ratio;
      pid->SampleTime = (unsigned long)NewSampleTime;
   }
}
  
void SetOutputLimits(PID_t *pid, double Min, double Max)
{
   if(Min > Max) return;
   pid->outMin = Min;
   pid->outMax = Max;
 
   if(pid->Output > pid->outMax) pid->Output = pid->outMax;
   else if(pid->Output < pid->outMin) pid->Output = pid->outMin;
 
   if(pid->ITerm > pid->outMax) pid->ITerm = pid->outMax;
   else if(pid->ITerm < pid->outMin) pid->ITerm = pid->outMin;
}
  
void SetMode(PID_t *pid, int16_t Mode)
{
   bool newAuto = (Mode == AUTOMATIC);
   if(newAuto == !pid->inAuto)
   {  /*we just went from manual to auto*/
      PidInit(pid);
   }
   pid->inAuto = newAuto;
   printf("auto: %i \n", pid->inAuto);
}
  
void PidInit(PID_t *pid)
{
   pid->lastInput = pid->Input;
   pid->ITerm = pid->Output;
   if(pid->ITerm > pid->outMax) pid->ITerm = pid->outMax;
   else if(pid->ITerm < pid->outMin) pid->ITerm = pid->outMin;
}
  
// void SetControllerDirection(PID_t *pid, int Direction)
// {
//    pid->controllerDirection = Direction;
// }

double convert(double current, uint16_t time)
{
   if(current < 0)
   {
      return -1;
   }
   return (current / 3600000 ) * time;
}


/*------------------------Code end here-------------------*/
