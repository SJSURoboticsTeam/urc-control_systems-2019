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
#include "math.h"
#include <cmath>
#include <string>

/*
    This task demonstrates how to read and write from EEPROM,
    which is non-voltaile memory that we can use to store data on the 
    ESP.
*/
extern "C" void vModeTaskHandler (void *pvParameters)
{
    ParamsStruct* Params = (ParamsStruct *) pvParameters;
    int previous_mode = 4;
    vTaskSuspend(xCarHandle);
    vTaskSuspend(xCrabHandle);
    vTaskSuspend(xSpinHandle);
    vTaskSuspend(xDebugHandle);

    while (1)
    {
        if (Params->mode != previous_mode)
        {
            setSpeedAllWheels(0);
            printf("Previous mode:\n    ");
            switch(previous_mode)
            {
                case 0: 
                    vTaskSuspend(xCarHandle);
                    printf("Car\n");
                    break;
                case 1:
                    vTaskSuspend(xCrabHandle);
                    printf("Crab\n");
                    break;
                case 2:
                    vTaskSuspend(xSpinHandle);
                    printf("Spin\n");
                    break;
                case 3:
                    vTaskSuspend(xDebugHandle);
                    printf("Debug\n");
                    break;
                default: printf("Invalid\n"); break;
            }
            printf("New mode:\n    ");
            switch(Params->mode)
            {
                case 0:
                    vTaskResume(xCarHandle);
                    printf("Car\n");
                    break;
                case 1:
                    initCrabMode();
                    vTaskResume(xCrabHandle);
                    printf("Crab\n");
                    break;
                case 2:
                    initSpinMode(Params->heading_A);
                    vTaskResume(xSpinHandle);
                    printf("Spin\n");
                    break;
                case 3:
                    vTaskResume(xDebugHandle);
                    printf("Debug\n");
                    break;
                default: printf("Invalid\n"); break; 
            }
            previous_mode = Params->mode;
        }
        vTaskDelay(10);
    }
}

extern "C" void vDebugTask(void *pvParameters)
{
    ParamsStruct* Params = (ParamsStruct*) pvParameters;
    setDirection(0, ((Params->speed_A>0)?1:0));
    setSpeed(0, Params->speed_A);
    setHeading(0, Params->heading_A);
    setDirection(1, ((Params->speed_B>0)?1:0));
    setSpeed(1, Params->speed_B);
    setHeading(1, Params->heading_B);
    setDirection(2, ((Params->speed_C>0)?1:0));
    setSpeed(2, Params->speed_C);
    setHeading(2, Params->heading_C);
    ParamsStruct PrevParams; 
    PrevParams.mode = Params->mode;
    PrevParams.speed_A = Params->speed_A;
    PrevParams.heading_A = Params->heading_A;
    PrevParams.speed_B = Params->speed_B;
    PrevParams.heading_B = Params->heading_B;
    PrevParams.speed_C = Params->speed_C;
    PrevParams.brake = Params->brake;

    while(1)
    {
        printf("Debug Mode\n");
        // adjust speed of wheel A if told to
        if (Params->speed_A != PrevParams.speed_A) 
        {
            setDirection(0, ((Params->speed_A > 0) ? 1:0));
            setSpeed(0, abs(Params->speed_A));
            PrevParams.speed_A = Params->speed_A;
            printf("    speed_A: %f \n", Params->speed_A);
        }
        // adjust heading of wheel A if told to
        if (Params->heading_A != PrevParams.heading_A)
        {
            setHeading(0, Params->heading_A);
            PrevParams.heading_A = Params->heading_A;
            printf("    heading_A: %f \n", Params->heading_A);            
        }
        // adjust speed of wheel B if told to
        if (Params->speed_B != PrevParams.speed_B)
        {
            setDirection(1, ((Params->speed_B > 0) ? 1:0));
            setSpeed(1, abs(Params->speed_B));
            PrevParams.speed_B = Params->speed_B;
            printf("    speed_B: %f \n", Params->speed_B);

        }
        // adjust heading of wheel B if told to
        if (Params->heading_B != PrevParams.heading_B)
        {
            setHeading(1, Params->heading_B);
            printf("    heading_B: %f \n", Params->heading_B);
            PrevParams.heading_B = Params->heading_B;

        }
        // adjust speed of wheel C if told to
        if (Params->speed_C != PrevParams.speed_C)
        {
            setDirection(2, ((Params->speed_C > 0) ? 1:0));
            setSpeed(2, abs(Params->speed_C));
            PrevParams.speed_C = Params->speed_C;
            printf("    speed_C: %f \n", Params->speed_C);
        }
        // adjust heading of wheel C if told to
        if (Params->heading_C != PrevParams.heading_C)
        {
            setHeading(2, Params->heading_C);
            printf("    heading_C: %f \n", Params->heading_C);
            PrevParams.heading_C = Params->heading_C;
        }
    // Delay for half a second
    vTaskDelay(500); 
    }
}

extern "C" void vCarTask(void *pvParameters)
{
    double radius_rover = 0;
    double radius_left = 0;
    double radius_right = 0;
    double radius_back = 0;
    
    double angle_left = 0;
    double angle_right = 0;
    double angle_back = 0;

    double speed_left = 0;
    double speed_right = 0;
    double speed_back = 0;
    
    Servo left_servo = servo_A;
    Servo right_servo = servo_B;
    Servo back_servo = servo_C;
    
    /* Test Servos
    ServoMotor left_motor = motor_A;
    ServoMotor right_motor = motor_B;
    ServoMotor back_motor = motor_C;
    */

    /* Real Motors */
    Motor left_motor = motor_A;
    Motor right_motor = motor_B;
    Motor back_motor = motor_C;

    ParamsStruct* Params = (ParamsStruct *) pvParameters;
    
    uint32_t front = (uint32_t) Params->heading_B;
    
    while (1)
    {
        if (front != Params->heading_B)
        {
            setSpeedAllWheels(0);
            vTaskDelay(1000);
            initDriveMode((uint32_t) Params->heading_B);
            switch ((uint32_t) Params->heading_B)
            {
                case 0: //Left = A, Right = B, Back = C
                    left_servo = servo_A;
                    right_servo = servo_B;
                    back_servo = servo_C;
                    left_motor = motor_A;
                    right_motor = motor_B;
                    back_motor = motor_C;
                    break;
                case 1: //Left = B, Right = C, Back = A 
                    left_servo = servo_B;
                    right_servo = servo_C;
                    back_servo = servo_A;
                    left_motor = motor_B;
                    right_motor = motor_C;
                    back_motor = motor_A;
                    break;
                case 2: //Left = C, Right = A, Back = B
                    left_servo = servo_C;
                    right_servo = servo_A;
                    back_servo = servo_B;
                    left_motor = motor_C;
                    right_motor = motor_A;
                    back_motor = motor_B;
                    break;
                default: //Left = A, Right = B, Back = C
                    left_servo = servo_A;
                    right_servo = servo_B;
                    back_servo = servo_C;
                    left_motor = motor_A;
                    right_motor = motor_B;
                    back_motor = motor_C;
                    break;
            }
            /* following angle values for testing rig (hobby servos) 
            angle_left = 150;
            angle_right = 30;
            angle_back = 90;
            */

            /* following angle values for actual servos */
            angle_left = 210;
            angle_right = 90;
            angle_back = 150;
            

            front = Params->heading_B;
        }
        // Calculate parameters for turning left
        if (Params->heading_A < 0)
        {
            // A 1058.355 in radius turns the inner wheel 1 degree
            radius_rover = (255 - abs(Params->heading_A))/255 * MAX_DIST;
            radius_left = sqrt(pow(radius_rover-SIDE/2, 2)+pow(SIDE_2_MID, 2));
            radius_right = sqrt(pow(radius_rover+SIDE/2, 2)+pow(SIDE_2_MID, 2));
            radius_back = sqrt(pow(radius_rover, 2) + pow(CORNER_2_MID, 2));
            
            angle_left = 0 - atan2(radius_rover - SIDE/2, SIDE_2_MID);
            angle_right = 0 - atan2(radius_rover + SIDE/2, SIDE_2_MID);
            angle_back = atan2(radius_rover, CORNER_2_MID);

            // Max rot/s for testing Servos = 0.833
            speed_right = abs(Params->speed_A)/100 * 0.833 *3.1416;
            speed_left = speed_right * (radius_left/radius_rover);
            speed_back = speed_right * (radius_back/radius_rover);
        }
        // Calculate parameters for turning right
        if (Params->heading_A < 0)
        {
            // A 1058.355 in radius turns the inner wheel 1 degree
            radius_rover = (255 - abs(Params->heading_A))/255 * MAX_DIST;
            radius_right = sqrt(pow(radius_rover-SIDE/2, 2)+pow(SIDE_2_MID, 2));
            radius_left = sqrt(pow(radius_rover+SIDE/2, 2)+pow(SIDE_2_MID, 2));
            radius_back = sqrt(pow(radius_rover, 2) + pow(CORNER_2_MID, 2));
            
            angle_right = atan2(radius_rover - SIDE/2, SIDE_2_MID);
            angle_left = atan2(radius_rover + SIDE/2, SIDE_2_MID);
            angle_back = 0 - atan2(radius_rover, CORNER_2_MID);

            // Max rot/s for testing Servos = 0.833
            speed_left = abs(Params->speed_A)/100 * 0.833 * 3.1416;
            speed_right = speed_left * (radius_right/radius_rover);
            speed_back = speed_left * (radius_back/radius_rover);
        }
        left_motor.SetDirection((Params->speed_A > 0) ? 0:1);
        right_motor.SetDirection((Params->speed_A > 0) ? 0:1);
        back_motor.SetDirection((Params->speed_A < 0) ? 0:1);
        // If heading is straight forward, straighten out the rover
        if (Params->heading_A == 0)
        {
            initDriveMode(Params->heading_B);
            setSpeedAllWheels(abs(Params->speed_A));
            /* following angle values for testing rig (hobby servos)
            angle_left = 150;
            angle_right = 30;
            angle_back = 90;
            */
            /* following angle values for actual motors */
            angle_left = 210;
            angle_right = 90;
            angle_back = 150;
            
        }
        else
        {
            // Percentages for testing servos
            left_servo.SetPositionPercent(left_servo.GetPercentage(180,
                                          angle_left + (90 - angle_left)));
            right_servo.SetPositionPercent(right_servo.GetPercentage(180,
                                           angle_right + (90 - angle_right)));
            back_servo.SetPositionPercent(back_servo.GetPercentage(180,
                                          angle_back + (90 - angle_back)));

            left_motor.SetSpeed((((speed_left)/3.1416)/0.833)*100);
            right_motor.SetSpeed((((speed_right)/3.1416)/0.833)*100);
            back_motor.SetSpeed((((speed_back)/3.1416)/0.833)*100);
        }
        vTaskDelay(5);
    }
}

extern "C" void vCrabTask(void *pvParameters)
{
    ParamsStruct *Params = (ParamsStruct *) pvParameters;

    double cam_offset = Params->heading_B;
    double heading = Params->heading_A;
    double wheel_A_heading = Params->heading_A;
    bool wheel_A_dir = 0;
    double wheel_B_heading = Params->heading_A;
    bool wheel_B_dir = 0;
    double wheel_C_heading = Params->heading_A;
    bool wheel_C_dir = 1;
    double speed = 0;

    while (1)
    {
        // If limit speed to 30 % of top speed
        if (abs(Params->speed_A) > 30)
        {
            speed = 30;
        }
        else
        {
            speed = abs(Params->speed_A);
        }

        // Adjust parameters for new instance of crab mode
        if (cam_offset != Params->heading_B)
        {
            cam_offset = Params->heading_B;
            wheel_A_heading = heading + cam_offset + 30;
            wheel_B_heading = heading + cam_offset - 30;
            wheel_C_heading = heading + cam_offset + 180;
        }

        // Update parameters for new heading from mission control 
        if (heading != Params->heading_A)
        {
            wheel_A_heading = wheel_A_heading + (Params->heading_A - heading);
            wheel_B_heading = wheel_A_heading + (Params->heading_A - heading);
            wheel_B_heading = wheel_A_heading + (Params->heading_A - heading);
        }

        // If wheels hit boundaries, flip them 180 degrees and switch 
        // direction they rotate.
        if (wheel_A_heading < MIN_ROTATION)
        {
            wheel_A_heading = wheel_A_heading + 180;
            wheel_A_dir = ~wheel_A_dir;
            motor_A.SetDirection(wheel_A_dir);
        }
        else if (wheel_A_heading > MAX_ROTATION)
        {
            wheel_A_heading = wheel_A_heading - 180;
            wheel_A_dir = ~wheel_A_dir;
            motor_A.SetDirection(wheel_A_dir);
        }
        if (wheel_B_heading < MIN_ROTATION)
        {
            wheel_B_heading = wheel_B_heading + 180;
            wheel_B_dir = ~wheel_B_dir;
            motor_B.SetDirection(wheel_B_dir);
        }
        else if (wheel_B_heading > MAX_ROTATION)
        {
            wheel_B_heading = wheel_B_heading - 180;
            wheel_B_dir = ~wheel_B_dir;
            motor_B.SetDirection(wheel_B_dir);
        }
        if (wheel_C_heading < MIN_ROTATION)
        {
            wheel_C_heading = wheel_C_heading + 180;
            wheel_C_dir = ~wheel_C_dir;
            motor_C.SetDirection(wheel_C_dir);
        }
        else if (wheel_C_heading > MAX_ROTATION)
        {
            wheel_C_heading = wheel_C_heading - 180;
            wheel_C_dir = ~wheel_C_dir;
            motor_C.SetDirection(wheel_C_dir);
        }

        // Update servo positions and speed
        servo_A.SetPositionPercent(100 * wheel_A_heading/MAX_ROTATION);
        servo_B.SetPositionPercent(100 * wheel_B_heading/MAX_ROTATION);
        servo_C.SetPositionPercent(100 * wheel_C_heading/MAX_ROTATION);
        setSpeedAllWheels(speed);

        vTaskDelay(5);
    }
}

extern "C" void vSpinTask(void *pvParameters)
{
    ParamsStruct *Params = (ParamsStruct *) pvParameters;
    while(1)
    {
        initSpinMode(Params->heading_A);
        setDirectionAllWheels((Params->speed_A < 0) ? 0:1);
        setSpeedAllWheels(abs(Params->speed_A));
        vTaskDelay(5);
    }
}