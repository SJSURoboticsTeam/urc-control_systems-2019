#include <stdlib.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "Arduino.h"
#include "EEPROM.h"
#include "Source.h"
#include "constants.h"
#include <string>
#include <cmath>
#include "../../../Utilities/Servo_Control.hpp"
#include "../../../Utilities/Servo_Control.cpp"
#include "Motor_Control_rev1.hpp"
#include "Adafruit_BNO055.h"
#include "PID.h"


extern "C" void vElbowTask(void *pvParameters)
{
    // printf("Entered Elbow Task\n");
    ParamsStruct* myParams = (ParamsStruct*) pvParameters;

    double currentTarget = myParams->ElbowTarget;
    double currentAngle = kElbowStartPos;   //Feedback?
    constexpr double kAlpha = 0.05;

    //(pin, Chanel, Timer, Freq, Max, Min)
    Servo Elbow(kElbowPin, 0, 0, kElbowFreq, kElbowPWMMax, kElbowPWMMin);
    Elbow.SetPositionPercent((kElbowStartPos / kElbowRange) * 100);
    // printf("Elbow's Starting Position: %f\n", kElbowStartPos);
    // printf("Elbow's Starting Duty: %f\n", (kElbowStartPos / kElbowRange) * 100);


    while(1)
    {
        if(currentTarget != myParams->ElbowTarget)
        {
            //update currentTarget
            currentTarget = myParams->ElbowTarget;
            // printf("Elbow's currentTarget: %f\n", currentTarget);

            //check to make sure Target is within bounds
            if((currentTarget > kElbowLimitMin) && (currentTarget < kElbowLimitMax)) 
            { 
                while(abs(currentAngle-currentTarget) > 0.2)
                {
                    currentAngle = ExpMovingAvg(currentAngle, currentTarget, kAlpha);

                    Elbow.SetPositionPercent((currentAngle / kElbowRange ) * 100);
                    // printf("currentTarget %f\n", currentTarget);
                    // printf("Angle: %f\n", currentAngle);
                    // printf("Duty: %f\n\n", (currentAngle / kElbowRange ) * 100);
                    vTaskDelay(2);
                }
                // printf("Done! Elbow angle = %f\n\n", currentAngle);
            }   
        }
        // printf("\nElbow Done\n");
        vTaskDelay(300);
    }
}

extern "C" void vRotundaTask(void *pvParameters)
{
    // printf("Entered Rotunda Task\n");

    ParamsStruct* params = (ParamsStruct*) pvParameters;

    constexpr double kAlpha = 0.01;
    double current_position = kRotundaStartPos;
    double prev_target = params->RotundaTarget;
    double current_position_translate, rotunda_target_translate, new_target = 0;
    double leftDistance, rightDistance;
    double dutyPercent;
    //(pin, Chanel, Timer, Freq, Max, Min)
    Servo myServo(kRotundaPin, 1, 0, kRotundaFreq, kRotundaPWMMax, kRotundaPWMMin); 
    myServo.SetPositionPercent((kRotundaStartPos/kRotundaPosmax) * 100);
    
    // printf("Rotunda Starting Duty: %f\n", kRotundaStartDuty);
    // printf("Rotunda Start Position: %f\n %f\n",kRotundaStartPos, (kRotundaStartPos / kRotundaPosmax)*100);

    while(1)
    {
        if(prev_target != params->RotundaTarget)  //remember: range is -180 to 180
        {
            // printf("RotundaTarget: %f\n", params->RotundaTarget);
            // printf("current_position: %f\n", current_position);

            current_position_translate = std::fmod(current_position, 360); //translate from 0-3600 into 0-360
            rotunda_target_translate = params->RotundaTarget + 180;

            // printf("current position translated: %f\n", current_position_translate);
            // printf("rotunda target translated: %f\n", rotunda_target_translate);

            //Compare current_position_translate and params->RotundaTarget
            //  & figure out if distance to target is smaller going down or Up
            //  Ex: if current = 200, and Target = 10, its more efficient
            //  to increase 170 degrees, resulting in a 
            //  modulo of 10, rather than decreasing 190 degrees
            if(current_position_translate > rotunda_target_translate)
            {
                leftDistance = abs(current_position_translate - rotunda_target_translate);
                rightDistance = abs(rotunda_target_translate + 360 - current_position_translate);

                if(leftDistance <= rightDistance)  
                {
                    if((current_position - leftDistance) <= kRotundaPosMin) 
                    {   //then we need to go the other way, cannot reduce our duty cycle
                        new_target = current_position + rightDistance;
                        // printf("Bottom limit reached, adding by %f instead\n", rightDistance);
                    }
                    else    //we can go the proper way
                    {
                        new_target = current_position - leftDistance;
                        // printf("Decreasing by %f\n",leftDistance);
                    }
                }

                else if(leftDistance > rightDistance)  
                {
                    if((current_position + rightDistance) >= kRotundaPosmax)
                    {   //Need to reduce since adding is out of bounds
                        // printf("Top limit reached, need to reduce by %f instead\n", leftDistance );
                        new_target = current_position - leftDistance;
                    }
                    else
                    {
                        //its okay if we increase
                        // printf("Increasing by %f\n", rightDistance);
                        new_target = current_position + rightDistance;
                    }
                }
            }
            else if(rotunda_target_translate > current_position_translate)
            {
                leftDistance = abs(current_position_translate + 360 - rotunda_target_translate);
                rightDistance = abs(rotunda_target_translate - current_position_translate);

                if(rightDistance <= leftDistance)  
                {
                    if((current_position + rightDistance) >= kRotundaPosmax)
                    {
                        // printf("Top limit reached, need to decrease by %f instead\n", leftDistance);
                        new_target = current_position - leftDistance;
                    }else
                    {
                        // printf("Increasing by %f\n",rightDistance);
                        new_target = current_position + rightDistance;
                    }
                }
                else if(rightDistance > leftDistance)
                {
                    if((current_position - leftDistance) <= kRotundaPosMin)
                    {
                        // printf("Bottom limit reached, adding by %f instead\n", rightDistance);
                        new_target = current_position + rightDistance;

                    }
                    else
                    {
                        // printf("Decreasing by %f\n", leftDistance);
                        new_target = current_position - leftDistance;
                    }
                }  
            }

            //Update the duty cycle accordingly
            // dutyPercent = (current_position/kRotundaPosmax) * 100;
            // printf("Rotunda Duty: %f\n\n\n", dutyPercent);
            // myServo.SetPositionPercent(dutyPercent);

            printf("current_position: %f\n", current_position);
            printf("new_target: %f\n", new_target);
            
            while(abs(current_position-new_target) > .1)
            {
                current_position = ExpMovingAvg(current_position, new_target , 0.05);
                dutyPercent = (current_position/kRotundaPosmax) * 100;
                myServo.SetPositionPercent(dutyPercent);
                // printf("current_position: %f\n", current_position);
                vTaskDelay(13);
            }
            printf("\nRotunda Done\n");
            prev_target = params->RotundaTarget;
        }

        vTaskDelay(300);   
    }
}

extern "C" void vShoulderTask(void *pvParameters)
{
    ParamsStruct* myParams = (ParamsStruct*) pvParameters;
    constexpr int kError = 3;
    Motor shoulder;
                        //Sig, Break, Dir, s_channel, timer, freq, min, max
                        //Break Pin is a dummy number, not present to our subsystem
    shoulder.InitMotor(kShoulderSigPin, 19, kShoulderDirPin, 2, 2, 
                        kMotorFreq, kShoulderEnablePWMMin, kShoulderEnablePWMMax);
    vTaskDelay(50);

    while(1)
    {
       if((myParams->pitch[0] != 2147483647))
        {
            if( (myParams->ShoulderTarget >= kShoulderLimitMin) 
                && (myParams->ShoulderTarget <= kShoulderLimitMax)) //If target is in accepted range
            {
                if((myParams->pitch[0] + kError) < myParams->ShoulderTarget)
                {
                    shoulder.SetSpeedAndDirection(50, false);
                }
                else if((myParams->pitch[0] - kError) > myParams->ShoulderTarget)
                { 
                    //May have to change direction
                    shoulder.SetSpeedAndDirection(50, true);
                }
                else
                {
                    shoulder.SetSpeed(0);
                }
            }
            else shoulder.SetSpeed(0);
        }
        vTaskDelay(5);
    }
}

extern "C" void vDiffGearboxTask(void *pvParameters)
{
    ParamsStruct* params = (ParamsStruct*) pvParameters;
    constexpr int kError = 3;
    
    Motor Wrist_Left, Wrist_Right;
    ledc_fade_func_install(ESP_INTR_FLAG_LEVEL1);

                        // pin_sig,  pin_brake,dir, s_channel, timer, freq, min, max
    Wrist_Left.InitMotor(kWristLeftSigPin, 19, kWristLeftDirPin, 3, 2, 
                        5000, kWristEnablePWMMin, kWristEnablePWMMax);

    Wrist_Right.InitMotor(kWristRightSigPin, 19, kWristRightDirPin, 4, 2, 
                        5000, kWristEnablePWMMin, kWristEnablePWMMax);
    
    double roll_pid_speed, pitch_pid_speed, rollTarget, WristPitchMirror;
    unsigned long currentTime;
    double rollTime;
    bool reverse;

    // PID(double* Input, double* Output, double* Setpoint,
    //     double Kp, double Ki, double Kd, int ControllerDirection);
    PID roll_pid(&params->pitch[1], &roll_pid_speed, &params->WristPitch,
                1.25, 0.0, 0.0, 0);
    roll_pid.SetMode(1);

    PID pitch_pid(&WristPitchMirror, &pitch_pid_speed, &params->WristPitch,
        0.8, 0.055, 0.01, 0);
    pitch_pid.SetMode(1); //Automatic mode

    while(1)
    {
        vTaskDelay(1);
    /*
        //if command from MS is diff;   the semaphore
        // if(xSemaphoreTake(params->xWristPitchSemaphore, 100))
        {
            // printf("Updating Pitch: %f\n\n", params->WristPitch);
            // if(params->WristPitch > 0)
            {
                //Increase pitch
                Wrist_Left.SetDirection(true);
                Wrist_Right.SetDirection(false);
            }

            // else if(params->WristPitch < 0)
            // {
            //     Wrist_Left.SetDirection(false);
            //     Wrist_Right.SetDirection(true);
            // }

            Wrist_Left.SetSpeed(50);
            Wrist_Right.SetSpeed(50);
            // vTaskDelay(abs(params->WristPitch) / 10);
            // Wrist_Left.SetSpeed(0);
            // Wrist_Right.SetSpeed(0);
        }
        
            // Roll:
        // if(xSemaphoreTake(params->xWristRollSemaphore, 100))
        // {
        //     printf("Updating Roll: %f\n\n", params->WristRoll);
        //     if(params->WristRoll > 0)
        //     {
        //         Wrist_Left.SetDirection(true);
        //         Wrist_Right.SetDirection(true);
        //     }

        //     else if(params->WristRoll < 0)
        //     {
        //         Wrist_Left.SetDirection(false);
        //         Wrist_Right.SetDirection(false);
        //     }
        //     // Set equal and constant speed for a const amount of time
        //     //Later we can implement PID on this
        //     Wrist_Left.SetSpeed(50);
        //     Wrist_Right.SetSpeed(50);
        //     vTaskDelay(abs(params->WristRoll) / 10);
        //     Wrist_Left.SetSpeed(0);
        //     Wrist_Right.SetSpeed(0);
        // }
    */
        int error = abs(params->WristPitch - params->pitch[1]);
        // printf("Error: %i\n", error);

        if((params->pitch[1] != 2147483647) && (params->pitch[0] != params->pitch[1]))
        {

    /*
            if(abs((int) params->WristPitch - params->pitch[1]) > kError) //if pitch needs adjustments 
            {
                // printf("Error larger than threshold\n");

                while(abs(params->WristPitch - params->pitch[1]) > kError)  //while pitch is off
                {
                    if(params->pitch[1] < params->WristPitch) //increase pitch
                    {
                        printf("Increasing pitch\n");
                        // set dir pins to increase pitch
                        Wrist_Left.SetDirection(true);
                        Wrist_Right.SetDirection(false);
                        pitch_pid.SetControllerDirection(0);
                    }   
                    else if (params->pitch[1]  > params->WristPitch)    //decrease pitch
                    {
                        printf("Decresing pitch\n");
                        // set dir pins to decrease pitch
                        Wrist_Left.SetDirection(false);
                        Wrist_Right.SetDirection(true);
                        pitch_pid.SetControllerDirection(1);
                    }

                    WristPitchMirror = (params->pitch[1]);
                    printf("WristPitchMirror : %f", WristPitchMirror);
                    bool status = pitch_pid.Compute();
                    printf("Computing PID, Speed: %f\n", pitch_pid_speed);
                    printf("Compute status: %i\n", status);
                    Wrist_Right.SetSpeed(pitch_pid_speed);
                    Wrist_Left.SetSpeed(pitch_pid_speed);
                    vTaskDelay(3);
                }
                Wrist_Right.SetSpeed(0);
                Wrist_Left.SetSpeed(0);
            }
    */
            
            //ROLL
            if(xSemaphoreTake(params->xWristRollSemaphore, 5))
            {
                currentTime = millis();
                rollTime = abs(params->WristRoll);
                reverse = false;

                if (params->WristRoll > 0)
                {
                    Wrist_Left.SetDirection(true);
                    Wrist_Right.SetDirection(true);
                    roll_pid.SetControllerDirection(0);
                    // reverse = false;
                }
                else if (params->WristRoll < 0)
                {
                    Wrist_Left.SetDirection(false);
                    Wrist_Right.SetDirection(false);
                    roll_pid.SetControllerDirection(1);
                    // reverse = true;
                }

                Wrist_Right.SetSpeed(50);
                while( (millis() - currentTime) < rollTime)
                {
                    bool status = roll_pid.Compute();
                    printf("Status: %i\n", status);

                    // if(reverse)     roll_pid_speed = 0 - roll_pid_speed;
                    Wrist_Left.SetSpeed(50 + roll_pid_speed);
                    printf("roll_pid_speed : %f\n", roll_pid_speed);
                    vTaskDelay(3);
                }
                Wrist_Right.SetSpeed(0);
                Wrist_Left.SetSpeed(0);

            }
        }

    }
}

extern "C" void vClawTask(void *pvParameters)
{
    // printf("Begin Claw Task...\n");
    ParamsStruct* myParams = (ParamsStruct*) pvParameters;
    bool task_complete = false;
    //Pin, Channel, Timer, Freq, max, min
    Servo Claw(act_ENABLE,5,2,5000,30,0);
    Claw.SetPositionPercent(0); //Set Duty Cycle to 0 at init
    initClaw();
    while(1) 
    {

        //Code to always assert motor for testing purposes
        Claw.SetPositionPercent(100);
        task_complete = openClaw();
        if(task_complete) printf("Opening\n");
        task_complete = false;
        myParams->current_direction = 0;

    /*
        if(myParams->current_direction == 0)
        {
            //Do Nothing
        }
        else if(myParams->current_direction == 2)
        {
            Claw.SetPositionPercent(0);
            // printf("Stoping\n");
        }
        else if(myParams->current_direction == 1)
        {
            // printf("Speed: %d", myParams->actuator_speed);
            Claw.SetPositionPercent(myParams->actuator_speed);
            task_complete = openClaw();
            if(task_complete) printf("Opening\n");
            task_complete = false;
            myParams->current_direction = 0;
        }
        else if(myParams->current_direction == -1)
        {
            Claw.SetPositionPercent(myParams->actuator_speed);
            task_complete = closeClaw();
            if(task_complete) printf("Closing\n");
            task_complete = false;
            myParams->current_direction = 0;
        }
        // printf("PHASE = %i  ENABLE = %i\n", digitalRead(act_PHASE),myParams->actuator_speed);
        vTaskDelay(300);
    */
    }
}

/*
extern "C" void vReadAxisTask(void *pvParameters) 
{
    ParamsStruct* params = (ParamsStruct*) pvParameters;

    // System Calibration Data
    uint8_t system_cal = 0;
    uint8_t gyro_cal = 0;
    uint8_t accel_cal = 0;
    uint8_t mag_cal = 0;

    sensors_event_t event[4];
    //Adafruit_BNO055 imuRotunda (0,IMU_ADDRESS_ROTUNDA);
    Adafruit_BNO055 imuShoulder(0,IMU_ADDRESS_SHOULDER);
    //Adafruit_BNO055 imuElbow   (2,IMU_ADDRESS_ELBOW);
    // Adafruit_BNO055 imuWrist   (1,IMU_ADDRESS_WRIST);
    i2c_scanner();
    i2c_scanner();
    //initIMU(IMU_ADDRESS_ROTUNDA , Adafruit_BNO055::OPERATION_MODE_IMUPLUS);
    initIMU(IMU_ADDRESS_SHOULDER, Adafruit_BNO055::OPERATION_MODE_IMUPLUS);
    //initIMU(IMU_ADDRESS_ELBOW   , Adafruit_BNO055::OPERATION_MODE_IMUPLUS);
    // initIMU(IMU_ADDRESS_WRIST   , Adafruit_BNO055::OPERATION_MODE_IMUPLUS);

    while(1){
    // Euler Angles (Relative Position)
        imuShoulder.getEvent(&event[0]);
        // imuWrist.getEvent(&event[1]);
    
    for ( int i = 0; i < 1; i++ )
    {
        params->yaw[i]   = event[i].orientation.x;
        params->roll[i]  = event[i].orientation.y;
        params->pitch[i] = event[i].orientation.z;
        // printf("%i) YAW: %.2f\tROLL: %.2f\tPITCH: %.2f\n",i,params->yaw[i],params->roll[i],params->pitch[i]);
        printf("PITCH: %f\n",params->pitch[i]);
    }

    // Get Calibration Info for ACCEL, GYRO, and MAG
    //imuShoulder.getCalibration(&system_cal, &gyro_cal, &accel_cal, &mag_cal);
    //printf("Calibration Data: SYS: %i\tGYR: %i\tACC: %i\tMAG: %i\n",system_cal,gyro_cal,accel_cal,mag_cal);

        vTaskDelay(50);
    }
}
*/

extern "C" void vMPU6050Task(void *pvParameters)
{
    ParamsStruct *params = (ParamsStruct*) pvParameters;
    imu::Vector<3> shoulder_imu;
    imu::Vector<3> wrist_imu;

    while(1)
    {
    shoulder_imu = scanMPU6050(MPU6050_ADDR1);
    wrist_imu    = scanMPU6050(MPU6050_ADDR0);

    //Calculate Shoulder Pitch and Roll
    params->pitch[0] = calculatePitch( shoulder_imu.x(), shoulder_imu.y(), shoulder_imu.z() );
    params->roll[0]  = calculateRoll ( shoulder_imu.x(), shoulder_imu.y(), shoulder_imu.z() );

    //Calculate Wrist Pitch and Roll
    params->pitch[1] = calculatePitch( wrist_imu.x(), wrist_imu.y(), wrist_imu.z() );
    params->roll[1]  = calculateRoll ( wrist_imu.x(), wrist_imu.y(), wrist_imu.z() );

    printf("Shoulder...PITCH: %f\tROLL: %i\n",params->pitch[0], params->roll[0] );
    printf("Wrist......PITCH: %f\tROLL: %i\n\n",params->pitch[1], params->roll[1] );
    //scanMPU6050();
    vTaskDelay(10);
    }
    
}

extern "C" void vI2CScannerTask(void *pvParameters)
{
    ParamsStruct *params = (ParamsStruct*) pvParameters;
    while(1)
    {
        i2c_scanner();
    for(int i = 0; i < 2; i++)
    {
        // printf("%i) YAW: %.2f\tPITCH: %.2f\tROLL: %.2f\n",i,params->yaw[i],params->pitch[i],params->roll[i]);
    }
    vTaskDelay(100);
    }
}