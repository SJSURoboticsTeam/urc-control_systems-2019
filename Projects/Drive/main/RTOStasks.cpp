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
    int previous_mode = 0; // Should be never be set to 4 after this
    vTaskSuspend(xCarHandle);
    vTaskSuspend(xCrabHandle);
    vTaskSuspend(xSpinHandle);
    //vTaskSuspend(xDebugHandle);

    while (1)
    {
        if (Params->mode != previous_mode)
        {
            setSpeedAllWheels(0);
            printf("Previous mode:\n    ");
            switch(previous_mode)
            {
                case 0: 
                    vTaskSuspend(xDebugHandle);
                    printf("Debug\n");
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
                    vTaskSuspend(xCarHandle);
                    printf("Car\n");
                    break;
                default: printf("Invalid\n"); break;
            }
            printf("New mode:\n    ");
            switch(Params->mode)
            {
                case 0:
                    vTaskResume(xDebugHandle);
                    printf("Debug\n");
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
                    vTaskResume(xCarHandle);
                    printf("Car\n");
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
    setDirection(0, ((Params->speed_A > 0) ? 1:0));
    setSpeed(0, Params->speed_A);
    setHeading(0, Params->heading_A);
    setDirection(1, ((Params->speed_B > 0) ? 1:0));
    setSpeed(1, Params->speed_B);
    setHeading(1, Params->heading_B);
    setDirection(2, ((Params->speed_C > 0) ? 1:0));
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
            motor_A.SetDirection(((Params->speed_A > 0) ? 1:0));
            motor_A.SetSpeed(abs(Params->speed_A));
            PrevParams.speed_A = Params->speed_A;
        }
        // adjust heading of wheel A if told to
        if (Params->heading_A != PrevParams.heading_A)
        {
            servo_A.SetPositionPercent(Params->heading_A);
            PrevParams.heading_A = Params->heading_A;
            printf("    heading_A: %f \n", Params->heading_A);            
        }
        // adjust speed of wheel B if told to
        if (Params->speed_B != PrevParams.speed_B)
        {
            motor_B.SetDirection(((Params->speed_B > 0) ? 1:0));
            motor_B.SetSpeed(abs(Params->speed_B));
            PrevParams.speed_B = Params->speed_B;
            printf("    speed_B: %f \n", Params->speed_B);

        }
        // adjust heading of wheel B if told to
        if (Params->heading_B != PrevParams.heading_B)
        {
            servo_B.SetPositionPercent(Params->heading_B);
            printf("    heading_B: %f \n", Params->heading_B);
            PrevParams.heading_B = Params->heading_B;

        }
        // adjust speed of wheel C if told to
        if (Params->speed_C != PrevParams.speed_C)
        {
            motor_C.SetDirection(((Params->speed_C > 0) ? 1:0));
            motor_C.SetSpeed(abs(Params->speed_C));
            PrevParams.speed_C = Params->speed_C;
            printf("    speed_C: %f \n", Params->speed_C);
        }
        // adjust heading of wheel C if told to
        if (Params->heading_C != PrevParams.heading_C)
        {
            servo_C.SetPositionPercent(Params->heading_C);
            printf("    heading_C: %f \n", Params->heading_C);
            PrevParams.heading_C = Params->heading_C;
        }
        // adjust brakes pwm if told to
        if (Params->brake != PrevParams.brake)
        {
            applyBrakes(Params->brake);
            printf("    brake: %d \n", Params->brake);
            PrevParams.brake = Params->brake;
        }
    // Delay for half a second
    vTaskDelay(100); 
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

    double current_angle_left = 0;
    double current_angle_right = 0;
    double current_angle_back = 0;

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
    double current_heading = Params->heading_A;
    double current_speed = Params->speed_A;
    double current_brakes = Params->brake;
    
    uint32_t front = (uint32_t) Params->heading_B;
    
    while (1)
    {
        if ((current_brakes != Params->brake) && (Params->brake != 0))
        {
            applyBrakes(1);
        }
        else if ((current_brakes != Params->brake) && (Params->brake == 0))
        {
            applyBrakes(0);
        }
        current_brakes = Params->brake;
        if (front != Params->heading_B)
        {
            setSpeedAllWheels(0);
            vTaskDelay(100);
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
            radius_rover =  (1 - (0 - Params->heading_A) * MAX_DIST);
            radius_left = sqrt(pow(radius_rover-SIDE/2, 2)+pow(SIDE_2_MID, 2));
            radius_right = sqrt(pow(radius_rover+SIDE/2, 2)+pow(SIDE_2_MID, 2));
            radius_back = sqrt(pow(radius_rover, 2) + pow(CORNER_2_MID, 2));
            
            angle_left = 0 - atan2(radius_rover - SIDE/2, SIDE_2_MID) * 180/3.14159;
            angle_right = 0 - atan2(radius_rover + SIDE/2, SIDE_2_MID) * 180/3.14159;
            angle_back = atan2(radius_rover, CORNER_2_MID) * 180/3.14159;

            // Max rot/s for hub motors = 3.6
            speed_right = abs(Params->speed_A)/100 * 3.6 * 3.1416 * 0.45; // The *0.45 to limit speed to 45% max
            speed_left = speed_right * (radius_left/radius_rover);
            speed_back = speed_right * (radius_back/radius_rover);
        }
        // Calculate parameters for turning right
        if (Params->heading_A > 0)
        {
            // A 1058.355 in radius turns the inner wheel 1 degree
            radius_rover = (1 - Params->heading_A) * MAX_DIST;
            radius_right = sqrt(pow(radius_rover-SIDE/2, 2)+pow(SIDE_2_MID, 2));
            radius_left = sqrt(pow(radius_rover+SIDE/2, 2)+pow(SIDE_2_MID, 2));
            radius_back = sqrt(pow(radius_rover, 2) + pow(CORNER_2_MID, 2));
            
            angle_right = atan2(radius_rover - SIDE/2, SIDE_2_MID) * 180/3.14159;
            angle_left = atan2(radius_rover + (SIDE/2), SIDE_2_MID) * 180/3.14159;
            angle_back = 0 - atan2(radius_rover, CORNER_2_MID) * 180/3.14159;

            // Max rot/s for hub motors = 3.6
            speed_left = abs(Params->speed_A)/100 * 3.6 * 3.1416 * 0.45; // The * 0.45 to limit speed to 45% max
            speed_right = speed_left * (radius_right/radius_rover);
            speed_back = speed_left * (radius_back/radius_rover);
        }
        // If heading is straight forward, straighten out the rover
        if (Params->heading_A == 0)
        {
            initDriveMode(Params->heading_B);
            if (current_speed != Params->speed_A)
            {
                setSpeedAllWheels(abs(Params->speed_A));
                current_speed = Params->speed_A;
            }
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
        else if ((current_heading != Params->heading_A) | (current_speed != Params->speed_A))
        {
            printf("current_speed: %f\n Params->speed_A: %f\n", current_speed, Params->speed_A);
            printf("current_heading: %f\n Params->heading_A: %f\n", current_heading, Params->heading_A);

            printf("\nRover Radius:\n    %f in\n", radius_rover);
            fflush(stdout);
            printf("Left Wheel Radius:\n    %f in\n", radius_left);
            fflush(stdout);
            printf("Right Wheel Radius:\n    %f in\n", radius_right);
            fflush(stdout);
            printf("Back Wheel Radius:\n    %f in\n", radius_back);
            fflush(stdout);
            printf("\nLeft Wheel Angle:\n    %f degrees\n", angle_left);
            fflush(stdout);
            printf("Right Wheel Angle:\n    %f degrees\n", angle_right);
            fflush(stdout);printf("Back Wheel Angle:\n    %f degrees\n", angle_back);
            if (Params->heading_A > 0)
            {
                printf("\nLeft Wheel Rotated:\n    %f percent\n", left_servo.GetPercentage(300, (210 + (90 - angle_left))));
                fflush(stdout);
                printf("Right Wheel Rotated:\n    %f percent\n", right_servo.GetPercentage(300, (90 + (90 - angle_left))));
                fflush(stdout);
                printf("Back Wheel Rotated:\n    %f percent\n", back_servo.GetPercentage(300, (150 + angle_back)));
                fflush(stdout);
            }
            else
            {   printf("\nLeft Wheel Rotated:\n    %f percent\n", left_servo.GetPercentage(300, (210 + ((0 - angle_left) - 90))));
                fflush(stdout);
                printf("Right Wheel Rotated:\n    %f percent\n", right_servo.GetPercentage(300, (90 + ((0 - angle_right) - 90))));
                fflush(stdout);
                printf("Back Wheel Rotated:\n    %f percent\n", back_servo.GetPercentage(300, (150 + angle_back)));
                fflush(stdout);
            }
            fflush(stdout);
            printf("\nLeft Wheel Speed:\n    %f percent\n", speed_left);
            fflush(stdout);
            printf("Right Wheel Speed:\n    %f percent\n", speed_right);
            fflush(stdout);
            printf("Back Wheel Speed:\n    %f percent\n", speed_back);
            fflush(stdout);

            if (Params->heading_A > 0)
            {

                left_servo.SetPositionPercent(left_servo.GetPercentage(300,
                                             (210 + (90 - angle_left))));
                right_servo.SetPositionPercent(right_servo.GetPercentage(300,
                                              (90 + (90 - angle_right))));
                back_servo.SetPositionPercent(back_servo.GetPercentage(300,
                                             (150 + angle_back)));
            }
            else
            {
                left_servo.SetPositionPercent(left_servo.GetPercentage(300,
                                             (210 + ((0 - angle_left) - 90))));
                right_servo.SetPositionPercent(right_servo.GetPercentage(300,
                                              (90 + ((0 - angle_right) - 90))));
                back_servo.SetPositionPercent(back_servo.GetPercentage(300,
                                             (150 + angle_back)));
            }

            left_motor.SetDirection((Params->speed_A > 0) ? 0:1);
            right_motor.SetDirection((Params->speed_A > 0) ? 0:1);
            back_motor.SetDirection((Params->speed_A < 0) ? 0:1);
            
            left_motor.SetSpeed((((speed_left)/3.1416)/0.833)*100);
            right_motor.SetSpeed((((speed_right)/3.1416)/0.833)*100);
            back_motor.SetSpeed((((speed_back)/3.1416)/0.833)*100);

            current_speed = Params->speed_A;
            current_heading = Params-> heading_A;

            current_angle_left = angle_left;
            current_angle_right = angle_right;
            current_angle_back = angle_back;
        }
        vTaskDelay(5);
    }
}

extern "C" void vCrabTask(void *pvParameters)
{
    ParamsStruct *Params = (ParamsStruct *) pvParameters;

    double cam_offset = Params->heading_C;
    double heading_x = 0 - Params->heading_A;
    double heading_y = 0 - Params->heading_B;
    double heading = atan2(heading_y, heading_x);
    double current_heading = heading;
    double wheel_A_heading = heading;
    bool wheel_A_dir = 0;
    double wheel_B_heading = heading;
    bool wheel_B_dir = 0;
    double wheel_C_heading = heading;
    bool wheel_C_dir = 1;
    double current_speed = 0;
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
        if (cam_offset != Params->heading_C)
        {
            cam_offset = Params->heading_C;
            wheel_A_heading = heading + cam_offset + 30;
            wheel_B_heading = heading + cam_offset - 30;
            wheel_C_heading = heading + cam_offset + 180;
        }

        // Update parameters for new heading from mission control 
        if ((heading_x != 0 - Params->heading_A) | (heading_y != 0 - Params->heading_B))
        {
            heading_x = 0 - Params->heading_A;
            heading_y = 0 - Params->heading_B;
            heading = atan2(heading_y, heading_x);
            wheel_A_heading = wheel_A_heading + (heading - current_heading);
            wheel_B_heading = wheel_B_heading + (heading - current_heading);
            wheel_B_heading = wheel_C_heading + (heading - current_heading);
            current_heading = heading;
        

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
        }
        if (speed != current_speed)
        {
            setSpeedAllWheels(Params->speed_A);
            current_speed = Params->speed_A;
        }


        vTaskDelay(5);
    }
}

extern "C" void vSpinTask(void *pvParameters)
{
    ParamsStruct *Params = (ParamsStruct *) pvParameters;
    double current_speed = 0;
    double current_brakes = 100;
    bool current_heading = 0;
    while(1)
    {
        if (Params->heading_A != current_heading)
        {
            motor_A.SetSpeed(0);
            motor_B.SetSpeed(0);
            motor_C.SetSpeed(0);
            initSpinMode(Params->heading_A);
            current_heading = Params->heading_A ? 1:0;
            motor_A.SetSpeed(current_speed);
            motor_B.SetSpeed(current_speed);
            motor_C.SetSpeed(current_speed);
        }
        if (Params->speed_A != current_speed)
        {
            setDirectionAllWheels((Params->speed_A < 0) ? 0:1);
            //setSpeedAllWheels(abs(Params->speed_A));
            motor_A.SetSpeed(Params->speed_A);
            motor_B.SetSpeed(Params->speed_A);
            motor_C.SetSpeed(Params->speed_A);
            current_speed = Params->speed_A;
            printf("setting speed");
        }
        if (Params->brake != current_brakes)
        {
            applyBrakes(Params->brake);
            current_brakes = Params->brake;
        }
        vTaskDelay(5);
    }
}