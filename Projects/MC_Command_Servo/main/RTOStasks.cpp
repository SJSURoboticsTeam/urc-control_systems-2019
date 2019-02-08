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


extern "C" void vElbowTask(void *pvParameters)
{
    printf("Entered Elbow Task\n");
    ParamsStruct* myParams = (ParamsStruct*) pvParameters;

    double currentTarget = myParams->ElbowTarget;
    double currentAngle = kElbowStartPos;   //Feedback?

    //(pin, Chanel, Timer, Freq, Max, Min)
    Servo Elbow(kElbowPin, 1, 0, 50, 12.5, 2.5);
    Elbow.SetPositionPercent((kElbowStartPos / kElbowRange) * 100);
    printf("Elbow's Starting Position: %f\n", kElbowStartPos);
    printf("Elbow's Starting Duty: %f\n", (kElbowStartPos / kElbowRange) * 100);


    while(1)
    {
        if(currentTarget != myParams->ElbowTarget)
        {
            //update currentTarget
            currentTarget = myParams->ElbowTarget;
            printf("currentTarget: %f\n", currentTarget);

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
    Servo myServo(kRotundaPin, 0, 0, 50, 10, 5);  //all this needs to change, check the library
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

    // Motor shoulder;
                //Sig, Dir, Channel, Timer, Freq, min, max
    // shoulder.InitMotor(kShoulderSigPin, kShoudlerDirPin, 2, 2, kShoulderFreq, 0, 50);

    double duration;

    while(1)
    {
        if(myParams->ShoudlerDuration_ms != 0 )
        {
            duration = myParams->ShoudlerDuration_ms;
            myParams->ShoudlerDuration_ms = 0;
            printf("Starting to move the thing!\n");

            //assert motor @ 50%?
            // shoulder.SetSpeedAndDirection(50, duration > 0 ? true : false);
            //delay that duration
            vTaskDelay(abs(duration));
            printf("Finished moving the thing!\n\n\n");
            //deassrt the motor
            // shoulder.setSpeed(0);
        }
    }
}
