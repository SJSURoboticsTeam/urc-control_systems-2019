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


extern "C" void vElbowTask(void *pvParameters)
{
    printf("Entered Elbow Task\n");
    ParamsStruct* myParams = (ParamsStruct*) pvParameters;

    double currentTarget = myParams->ElbowTarget;
    double currentAngle = kElbowStartPos;   //Feedback?

    //(pin, Chanel, Timer, Freq, Max, Min)
    Servo Elbow(kElbowPin, 2, 0, kElbowFreq, kElbowPWMMax, kElbowPWMMin);
    Elbow.SetPositionPercent((kElbowStartPos / kElbowRange) * 100);
    printf("Elbow's Starting Position: %f\n", kElbowStartPos);
    printf("Elbow's Starting Duty: %f\n", (kElbowStartPos / kElbowRange) * 100);


    while(1)
    {
        if(currentTarget != myParams->ElbowTarget)
        {
            //update currentTarget
            currentTarget = myParams->ElbowTarget;
            printf("Elbow's currentTarget: %f\n", currentTarget);

            //check to make sure Target is within bounds
            if((currentTarget > kElbowLimitMin) && (currentTarget < kElbowLimitMax)) //Need to set this to our mechanical constraints
            { 
                //update duty
                currentAngle = currentTarget;
                Elbow.SetPositionPercent((currentAngle / kElbowRange) * 100);
                printf("Setting Duty to %f\n", (currentAngle / kElbowRange)*100);
            }   

            printf("current Angle: %f\n\n\n", currentAngle);
        }
        vTaskDelay(500);
    }
}


extern "C" void vRotundaTask(void *pvParameters)
{
    // Notes:
    // MS sends a command of 0 to 360 OR -180 to 180
    //     if the latter, just add 180 to target
    printf("Entered Rotunda Task\n");

    ParamsStruct* params = (ParamsStruct*) pvParameters;

    double current_position = kRotundaStartPos;
    double prev_target = params->RotundaTarget;
    double current_position_translate, rotunda_target_translate;
    double leftDistance, rightDistance;
    double dutyPercent;

    //(pin, Chanel, Timer, Freq, Max, Min)
    Servo myServo(kRotundaPin, 0, 0, kRotundaFreq, kRotundaPWMMax, kRotundaPWMMin);  
    myServo.SetPositionPercent(kRotundaStartDuty);
    
    printf("Rotunda Starting Duty: %f\n", kRotundaStartDuty);
    printf("Rotunda Start Position: %f\n %f\n",kRotundaStartPos, (kRotundaStartPos / kRotundaPosmax)*100);

    while(1)
    {
        if(prev_target != params->RotundaTarget)  //remember: range is -180 to 180
        {
            printf("RotundaTarget: %f\n", params->RotundaTarget);
            printf("current_position: %f\n", current_position);

            current_position_translate = std::fmod(current_position, 360); //translate from 0-3600 into 0-360
            rotunda_target_translate = params->RotundaTarget + 180;

            printf("current position translated: %f\n", current_position_translate);
            printf("rotunda target translated: %f\n", rotunda_target_translate);

            //Compare current_position_translate and params->RotundaTarget
            //  & figure out if distance to target is smaller going down or Up
            //  Ex: if current = 200, and new_target = 10, its more efficient
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
                        current_position += rightDistance;
                        printf("Bottom limit reached, adding by %f instead\n", rightDistance);
                    }
                    else    //we can go the proper way
                    {
                        current_position -= leftDistance;
                        printf("Decreasing by %f\n",leftDistance);
                    }
                }

                else if(leftDistance > rightDistance)  
                {
                    if((current_position + rightDistance) >= kRotundaPosmax)
                    {   //Need to reduce since adding is out of bounds
                        printf("Top limit reached, need to reduce by %f instead\n", leftDistance );
                        current_position -= leftDistance;
                    }
                    else
                    {
                        //its okay if we increase
                        printf("Increasing by %f\n", rightDistance);
                        current_position += rightDistance;
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
                        printf("Top limit reached, need to decrease by %f instead\n", leftDistance);
                        current_position -= leftDistance;
                    }else
                    {
                        printf("Increasing by %f\n",rightDistance);
                        current_position += rightDistance;
                    }
                }
                else if(rightDistance > leftDistance)
                {
                    if((current_position - leftDistance) <= kRotundaPosMin)
                    {
                        printf("Bottom limit reached, adding by %f instead\n", rightDistance);
                        current_position += rightDistance;

                    }
                    else
                    {
                        printf("Decreasing by %f\n", leftDistance);
                        current_position -= leftDistance;
                    }
                }  
            }

            //Update the duty cycle accordingly
            dutyPercent = (current_position/kRotundaPosmax) * 100;
            printf("Rotunda Duty: %f\n\n\n", dutyPercent);
            myServo.SetPositionPercent(dutyPercent);

            //STILL NEED TO SET THE SERVO DUTY!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            prev_target = params->RotundaTarget;
        }
        vTaskDelay(500);   
    }
}

extern "C" void vShoulderTask(void *pvParameters)
{
    printf("Entered Shoulder Task\n");
    ParamsStruct* myParams = (ParamsStruct*) pvParameters;

    Motor shoulder;
                        //Sig, Break, Dir, s_channel, timer, freq, min, max
                        //Break Pin is a dummy number, not present to our subsystem
    shoulder.InitMotor(kShoulderSigPin, 19, kShoulderDirPin, 3, 3, 
                        kMotorFreq, kShoulderEnablePWMMin, kShoulderEnablePWMMax);

    double duration;

    printf("Init finished?\n");
    while(1)
    {
        if(myParams->ShoudlerDuration_ms != 0 )
        {
            duration = myParams->ShoudlerDuration_ms;
            printf("Starting to move the thing!\n");
            printf("Time: %f\n", duration);

            //assert motor @ 50%?
            shoulder.SetSpeedAndDirection(50, duration > 0 ? true : false);
            //delay that duration
            vTaskDelay(abs(duration ) / 10);
            printf("Finished moving the thing!\n\n\n");
            //deassrt the motor
            shoulder.SetSpeed(0);

            myParams->ShoudlerDuration_ms = 0;
        }
        // printf("Looped\n");
        vTaskDelay(500);
    }
}


    /*
        If motors moving in opposite directions -> Wrist rotates
            However, motors are mounted in opposites already, so if
            Dir is the same, the wrist will rotate. 

        If motors are moving in same directions -> Pitch Adjustment
            Our configuration is if motors in the opposite dirs
            if Dir pin is diff, pitch will adjust
            But which way??
            Dir1 = 0 and Dir2 = 1, will gearbox rise or fall?

    */
extern "C" void vDiffGearboxTask(void *pvParameters)
{
    //XHR Parameters:
        //Wrist_dimension;
        //Wrist_delta;   

        //Use a semaphore to see if anything has changed?
            //xWristSemaphore
    printf("Gearbox Task Entered\n");
    vTaskDelay(200);
    ParamsStruct* params = (ParamsStruct*) pvParameters;
    double Delta;

    Motor Wrist_Left, Wrist_Right;

    ledc_fade_func_install(ESP_INTR_FLAG_LEVEL1);

    printf("Starting Init\n");
                        // pin_sig,  pin_brake,dir, s_channel, timer, freq, min, max
    Wrist_Left.InitMotor(kWristLeftSigPin, 19, kWristLeftDirPin, 1, 3, 
                        5000, kShoulderEnablePWMMin, kShoulderEnablePWMMax);

    printf("Init Left Fin\n");
    Wrist_Right.InitMotor(kWristRightSigPin, 19, kWristRightDirPin, 3, 3, 
                        5000, kShoulderEnablePWMMin, kShoulderEnablePWMMax);
    printf("Init Right Fin\n");

    while(1)
    {

        //if command from MS is diff;   the semaphore
        if(xSemaphoreTake(params->xWristSemaphore, portMAX_DELAY))
        {
            printf("Wrist command Received: %i\n", params->Wrist_Dimension);
            Delta = params->Wrist_Delta;
            //Check which dimension needs to be changed
            
            // Pitch:
            if(params->Wrist_Dimension == 0)
            {

                printf("Updating Pitch:\n");
                printf("Delta: %f\n", Delta);
                // Ignore delta for now, we need feedback first to relate it
                // Need to determine if Dir1=0 && Dir2=1 does it move up or down
                // For now, if delta > 0, Dir1=1 and Dir2 = 0;
                if(Delta > 0)
                {
                    Wrist_Left.SetDirection(true);
                    Wrist_Right.SetDirection(false);
                }
                // if delta < 0, Dir1=0 and Dir2 = 1;
                else if(Delta < 0)
                {
                    Wrist_Left.SetDirection(false);
                    Wrist_Right.SetDirection(true);
                }
                //Then move both sides an equal&constant speed
                // Maybe use Delta as a time to move, like for shoulder now
                //Later we can implement PID on this

                Wrist_Left.SetSpeed(50);
                Wrist_Right.SetSpeed(50);
                vTaskDelay(abs(Delta) / 10);
                Wrist_Left.SetSpeed(0);
                Wrist_Right.SetSpeed(0);
            }
        
            // Roll:
            else if(params->Wrist_Dimension == 1)
            {
                printf("Updating Roll\n");
                printf("Delta: %f\n", Delta);
                // Ignore delta for now, we need feedback first to relate it
                // Look at Delta to determine Dir pins
                // if delta > 0 Dir1 = Dir2 = 1
                if(Delta > 0)
                {
                    Wrist_Left.SetDirection(true);
                    Wrist_Right.SetDirection(true);
                }
                // if delta < 0 Dir1 = Dir2 = 0
                else if(Delta < 0)
                {
                    Wrist_Left.SetDirection(false);
                    Wrist_Right.SetDirection(false);
                }
                // Set equal and constant speed for a const amount of time
                // Maybe use Delta as a time to move, like for shoulder now 
                //Later we can implement PID on this
                Wrist_Left.SetSpeed(50);
                Wrist_Right.SetSpeed(50);
                vTaskDelay(abs(Delta) / 10);
                Wrist_Left.SetSpeed(0);
                Wrist_Right.SetSpeed(0);
            }
        }
        printf("end While\n\n\n");
    }
}


extern "C" void vClawTask(void *pvParameters)
{
    printf("Begin Claw Task...\n");
    ParamsStruct* myParams = (ParamsStruct*) pvParameters;
    bool task_complete = false;
    //Pin, Channel, Timer, Freq, max, min
    Servo Claw(act_ENABLE,1,1,5000,50,0);
    Claw.SetPositionPercent(0); //Set Duty Cycle to 0 at init
    initClaw();
    while(1) {
    if(myParams->current_direction == 0)
    {
        //Do Nothing
    }
    else if(myParams->current_direction == 2)
    {
        Claw.SetPositionPercent(0);
    }
    else if(myParams->current_direction == 1)
    {
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
    printf("PHASE = %i  ENABLE = %i\n", digitalRead(act_PHASE),myParams->actuator_speed);
    vTaskDelay(300);
    }
}