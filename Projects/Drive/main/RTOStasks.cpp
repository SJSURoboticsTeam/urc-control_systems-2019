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
                    vTaskSuspend(xDriveHandle);
                    printf("Drive\n");
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
                    initSpinMode(0);
                    vTaskResume(xSpinHandle);
                    printf("Spin\n");
                    break;
                case 3:
                    vTaskResume(xDriveHandle);
                    printf("Drive\n");
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
    double heading = 0;
    double previous_heading = 0;
    double speed = 0;
    double previous_speed = 0;
    bool previous_brakes = 0;

    while(1)
    {
        printf("Debug Mode\n");
        // Convert AXIS 0 to 0% - 100% scale
        heading = (1 + Params->AXIS_X) * 50;
        speed = 100 * (0 - Params->AXIS_Y) * Params->THROTTLE;
        if (heading != previous_heading)
        {
            if (Params->wheel_A)
            {
                servo_A.SetPositionPercent(heading);
                printf("Wheel A set to: %f deg.\n", heading/100 * MAX_ROTATION);
            }
            if (Params->wheel_B)
            {
                servo_B.SetPositionPercent(heading);
                printf("Wheel B set to: %f deg.\n", heading/100 * MAX_ROTATION);                
            }
            if (Params->wheel_C)
            {
                servo_C.SetPositionPercent(heading);
                printf("Wheel C set to: %f deg.\n", heading/100 * MAX_ROTATION);
            }
            previous_heading = heading;
        }
        if (speed != previous_speed)
        {
            if (Params->wheel_A)
            {
                motor_A.SetSpeed(abs(speed));
                printf("Wheel A speed set to %f percent\n", speed);
                motor_A.SetDirection((speed > 0) ? 0:1);
                printf("Wheel A direction: %d\n", speed ? 0:1);
            }
            if (Params->wheel_B)
            {
                motor_B.SetSpeed(abs(speed));
                printf("Wheel B speed set to %f percent\n", speed);
                motor_B.SetDirection((speed > 0) ? 0:1);
                printf("Wheel B direction: %d\n", speed ? 0:1);
            }
            if (Params->wheel_C)
            {
                motor_C.SetSpeed(abs(speed));
                printf("Wheel C speed set to %f percent\n", speed);
                motor_C.SetDirection((speed > 0) ? 0:1);
                printf("Wheel C direction: %d\n", speed ? 0:1);
            }
            previous_speed = speed;
        }
        if (previous_brakes != Params->button_0)
        {
            applyBrakes(Params->button_0);
            previous_brakes = Params->button_0;
            printf("Brakes set to %d\n", Params->button_0);
        }
    // Delay for half a second
    vTaskDelay(50); 
    }
}

extern "C" void vDriveTask(void *pvParameters)
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
    double current_heading = Params->AXIS_X;
    double current_speed = 100 * (0 - Params->AXIS_Y) * Params->THROTTLE;
    double current_brakes = 1;
    uint32_t front = 0;
    uint32_t previous_front = 3;
    
    while (1)
    {
        // Set the brakes
        if (current_brakes != Params->button_0)
        {
            applyBrakes(Params->button_0);
            current_brakes = Params->button_0;
        }

        // Update the front of the rover
        if (Params->wheel_A)
        {
            front = 1;
        }
        else if (Params->wheel_B)
        {
            front = 2;
        }
        else if (Params->wheel_C)
        {
            front = 0;
        }
        if (previous_front != front)
        {
            setSpeedAllWheels(0);
            vTaskDelay(100);
            initDriveMode(front);
            switch (front)
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
            

            previous_front = front;
        }
        // Calculate parameters for turning left
        if (Params->AXIS_X < 0)
        {
            // A 1058.355 in radius turns the inner wheel 1 degree
            radius_rover =  (1 - (0 - Params->AXIS_X) * MAX_DIST);
            radius_left = sqrt(pow(radius_rover-SIDE/2, 2)+pow(SIDE_2_MID, 2));
            radius_right = sqrt(pow(radius_rover+SIDE/2, 2)+pow(SIDE_2_MID, 2));
            radius_back = sqrt(pow(radius_rover, 2) + pow(CORNER_2_MID, 2));
            
            angle_left = 0 - atan2(radius_rover-SIDE/2, SIDE_2_MID)*180/3.1416;
            angle_right = 0 - atan2(radius_rover+SIDE/2, SIDE_2_MID)*180/3.1416;
            angle_back = atan2(radius_rover, CORNER_2_MID) * 180/3.14159;

            // Max rot/s for hub motors = 3.6
            speed_right = abs((0 - Params->AXIS_Y)*Params->THROTTLE) * 260 * 13.5;
            speed_left = speed_right * (radius_left/radius_rover);
            speed_back = speed_right * (radius_back/radius_rover);
        }
        // Calculate parameters for turning right
        if (Params->AXIS_X > 0)
        {
            // A 1058.355 in radius turns the inner wheel 1 degree
            radius_rover = (1 - Params->AXIS_X) * MAX_DIST;
            radius_right = sqrt(pow(radius_rover-SIDE/2, 2)+pow(SIDE_2_MID, 2));
            radius_left = sqrt(pow(radius_rover+SIDE/2, 2)+pow(SIDE_2_MID, 2));
            radius_back = sqrt(pow(radius_rover, 2) + pow(CORNER_2_MID, 2));
            
            angle_right = atan2(radius_rover-SIDE/2, SIDE_2_MID) * 180/3.1416;
            angle_left = atan2(radius_rover+(SIDE/2), SIDE_2_MID) * 180/3.1416;
            angle_back = 0 - atan2(radius_rover, CORNER_2_MID) * 180/3.1416;

            // Max rot/s for hub motors = 3.6
            speed_left = abs((0 - Params->AXIS_Y)*Params->THROTTLE) * 260 * 13.5;
            speed_right = speed_left * (radius_right/radius_rover);
            speed_back = speed_left * (radius_back/radius_rover);
        }
        // If heading is straight forward, straighten out the rover
        if (Params->AXIS_X == 0)
        {
            initDriveMode(front);
            if (current_speed != 100 * (0 - Params->AXIS_Y) * Params->THROTTLE)
            {
                current_speed = 100 * (0 - Params->AXIS_Y) * Params->THROTTLE;
                setSpeedAllWheels(abs(current_speed));
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
        else if ((current_heading != Params->AXIS_X) | 
                (current_speed != 100 * (0 - Params->AXIS_Y) * Params->THROTTLE))
        {
            /*// DEBUGGING MESSAGES
            printf("current_speed: %f\n calculated speed: %f\n", current_speed, 100*(0-Params->AXIS_Y*Params->THROTTLE));
            printf("current_heading: %f\n Params->AXIS_X: %f\n", current_heading, Params->AXIS_X);

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
            */

            if (Params->AXIS_X > 0)
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

            left_motor.SetDirection((Params->AXIS_Y > 0) ? 0:1);
            right_motor.SetDirection((Params->AXIS_Y > 0) ? 0:1);
            back_motor.SetDirection((Params->AXIS_Y < 0) ? 0:1);
            
            left_motor.SetSpeed(((speed_left)/26)/13.5);
            right_motor.SetSpeed(((speed_right)/26)/13.5);
            back_motor.SetSpeed(((speed_back)/26)/13.5);

            current_speed = 100 * (0 - Params->AXIS_Y) * Params->THROTTLE;
            current_heading = Params->AXIS_X;

            current_angle_left = angle_left;
            current_angle_right = angle_right;
            current_angle_back = angle_back;
        }
        vTaskDelay(2);
    }
}

extern "C" void vCrabTask(void *pvParameters)
{
    ParamsStruct *Params = (ParamsStruct *) pvParameters;

    double cam_offset = Params->mast_position;
    double heading_x = 0 - Params->AXIS_X;
    double heading_y = 0 - Params->AXIS_Y;
    double heading = atan2(heading_y, heading_x) * 180/3.1416;
    double current_heading = heading;
    double wheel_A_heading = heading;
    bool wheel_A_dir = 0;
    double wheel_B_heading = heading;
    bool wheel_B_dir = 0;
    double wheel_C_heading = heading;
    bool wheel_C_dir = 1;
    double current_speed = 0;
    double speed = 0;
    bool current_brakes= 1;

    while (1)
    {
        speed = sqrt(pow(Params->AXIS_X, 2) + pow(Params->AXIS_Y, 2)) * Params->THROTTLE;
        if (Params->button_0 != current_brakes)
        {
            applyBrakes(Params->button_0);
            current_brakes = Params->button_0;
        }
        // Adjust parameters for new instance of crab mode
        if (cam_offset != Params->mast_position)
        {
            cam_offset = Params->mast_position;
            wheel_A_heading = heading + cam_offset + 30;
            wheel_B_heading = heading + cam_offset - 30;
            wheel_C_heading = heading + cam_offset + 180;
        }

        // Update parameters for new heading from mission control 
        if ((heading_x != 0 - Params->AXIS_X) | 
            (heading_y != 0 - Params->AXIS_Y))
        {
            heading_x = 0 - Params->AXIS_X;
            heading_y = 0 - Params->AXIS_Y;
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
            setSpeedAllWheels(speed);
            current_speed = speed;
        }
        vTaskDelay(2);
    }
}

extern "C" void vSpinTask(void *pvParameters)
{
    ParamsStruct *Params = (ParamsStruct *) pvParameters;
    double current_speed = 0;
    bool current_brakes = 1;
    while(1)
    {
        initSpinMode(0);
        if (Params->AXIS_X != current_speed)
        {
            current_speed = 100 * Params->AXIS_X;
            setDirectionAllWheels(current_speed ? 0:1);
            setSpeedAllWheels(100 * abs(current_speed));
        }
        if (Params->button_0 != current_brakes)
        {
            applyBrakes(Params->button_0);
            current_brakes = Params->button_0;
        }
        vTaskDelay(2);
    }
}