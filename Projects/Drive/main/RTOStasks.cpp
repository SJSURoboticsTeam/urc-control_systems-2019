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

extern "C" void vMoveTask(void *pvParameters)
{
    ParamsStruct *Params = (ParamsStruct *) pvParameters;

    uint32_t previous_mode = 4;

    // Drive Mode variables //
    double radius_rover = 0;
    double radius_left = 0;
    double radius_right = 0;
    double radius_back = 0;
    
    double angle_left = 0;
    double angle_right = 0;
    double angle_back = 0;

    double target_angle = 0;
    double current_angle = 0;
    double current_angle_left = 0;
    double current_angle_right = 0;
    double current_angle_back = 0;

    double speed_left = 0;
    double speed_right = 0;
    double speed_back = 0;
    
    Servo left_servo;
    Servo right_servo;
    Servo back_servo;


    Motor left_motor;
    Motor right_motor;
    Motor back_motor;

    double current_heading = 0;
    double target_speed = 0;
    double current_speed = 0;
    double previous_speed = 0;
    double current_brakes = 1;
    uint32_t front = 0;
    uint32_t previous_front = 3;
    

    // Crab/Spin Mode variables //
    double cam_offset = 0;
    double heading_x = 0;
    double heading_y = 0;
    double heading = 0;
    double previous_heading = 0;
    double wheel_A_heading = 90;
    bool wheel_A_dir = 0;
    double wheel_B_heading = 210;
    bool wheel_B_dir = 0;
    double wheel_C_heading = 150;
    bool wheel_C_dir = 1;
    double speed = 0;
    bool previous_brakes = 1;

    while (1)
    {
        printf("deciding on task...\n");
        while(Params->mode == DEBUG)
        {
            if (Params->mode != DEBUG)
            {
                continue;
            }
            if(previous_mode != DEBUG)
            {
                previous_mode = DEBUG;
                applyBrakes(0);    
            }
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
                    motor_A.SetSpeed(fabs(speed));
                    printf("Wheel A speed set to %f percent\n", speed);
                    motor_A.SetDirection((speed > 0) ? 0:1);
                    printf("Wheel A direction: %d\n", speed ? 0:1);
                }
                if (Params->wheel_B)
                {
                    motor_B.SetSpeed(fabs(speed));
                    printf("Wheel B speed set to %f percent\n", speed);
                    motor_B.SetDirection((speed > 0) ? 0:1);
                    printf("Wheel B direction: %d\n", speed ? 0:1);
                }
                if (Params->wheel_C)
                {
                    motor_C.SetSpeed(fabs(speed));
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
            // Delay 50 ms
            vTaskDelay(50);

        }

        while(Params->mode == CRAB)
        {
            if (Params->mode != CRAB)
            {
                continue;
            }
            if(previous_mode != CRAB)
            {
                applyBrakes(0);
                wheel_A_heading = 90;
                wheel_A_dir = 0;
                wheel_B_heading = 210;
                wheel_B_dir = 0;
                wheel_C_heading = 150;
                wheel_C_dir = 1;
                previous_mode = CRAB;
            }
            printf("Crab Mode\n");
            if (Params->button_0 != current_brakes)
            {
                applyBrakes(Params->button_0);
                current_brakes = Params->button_0;
            }
            // Calculate current speed
            if ((abs(Params->AXIS_X) == 1) | (abs(Params->AXIS_Y) == 1))
            {
                speed = Params->THROTTLE;
            }
            else
            {
                speed = sqrt(pow(Params->AXIS_X, 2) + pow(Params->AXIS_Y, 2))/1.414124 * Params->THROTTLE;
            }
            // Adjust parameters for new instance of crab mode
            if (cam_offset != Params->mast_position)
            {
                cam_offset = Params->mast_position;
                wheel_A_heading = wheel_A_heading + cam_offset;
                wheel_B_heading = wheel_B_heading + cam_offset;
                wheel_C_heading = wheel_B_heading + cam_offset;
            }
            // Update parameters for new heading from mission control 
            if ((heading_y != 0 - Params->AXIS_Y) | (heading_x != 0 - Params->AXIS_X))
            {
                heading_y = Params->AXIS_X;
                heading_x = 0 - Params->AXIS_Y;
                heading = atan2(heading_y, heading_x) * 180/3.1416;
                wheel_A_heading = wheel_A_heading - (current_heading - heading);
                wheel_B_heading = wheel_B_heading - (current_heading - heading);
                wheel_C_heading = wheel_C_heading - (current_heading - heading);
                //printf("Headings:\n");
                //printf("    rover: %f\n    x: %f\n", heading, heading_x);
                //printf("    y: %f\n    wheel A: %f\n", heading_y, wheel_A_heading);
                //printf("    wheel B: %f\n    wheel C:    %f\n", wheel_B_heading, wheel_C_heading);
                current_heading = heading;

                // If wheels hit boundaries, flip them 180 degrees and switch 
                // direction they rotate.
                if (wheel_A_heading < MIN_ROTATION)
                {
                    applyBrakes(0);
                    wheel_A_heading = wheel_A_heading + 180;
                    wheel_A_dir = !wheel_A_dir;
                    //printf("wheel A direction: %d\n", wheel_A_dir);
                    motor_A.SetDirection(wheel_A_dir);
                    //servo_A.SetPositionPercent(100 * wheel_A_heading/MAX_ROTATION);
                    //delay(350);
                    applyBrakes(current_brakes);
                }
                else if (wheel_A_heading > MAX_ROTATION)
                {
                    applyBrakes(0);
                    wheel_A_heading = wheel_A_heading - 180;
                    wheel_A_dir = !wheel_A_dir;
                    //printf("wheel A direction: %d\n", wheel_A_dir);
                    motor_A.SetDirection(wheel_A_dir);
                    //servo_A.SetPositionPercent(100 * wheel_A_heading/MAX_ROTATION);
                    //delay(1000);
                    applyBrakes(current_brakes);
                }
                if (wheel_B_heading < MIN_ROTATION)
                {
                    applyBrakes(0);
                    wheel_B_heading = wheel_B_heading + 180;
                    wheel_B_dir = !wheel_B_dir;
                    //printf("wheel B direction: %d\n", wheel_B_dir);
                    motor_B.SetDirection(wheel_B_dir);
                    //servo_B.SetPositionPercent(100 * wheel_B_heading/MAX_ROTATION);
                    //delay(1000);
                    applyBrakes(current_brakes);
                }
                else if (wheel_B_heading > MAX_ROTATION)
                {
                    applyBrakes(0);
                    wheel_B_heading = wheel_B_heading - 180;
                    wheel_B_dir = !wheel_B_dir;
                    //printf("wheel B direction: %d\n", wheel_B_dir);
                    motor_B.SetDirection(wheel_B_dir);
                    //servo_B.SetPositionPercent(100 * wheel_B_heading/MAX_ROTATION);
                    //delay(1000);
                    applyBrakes(current_brakes);
                }
                if (wheel_C_heading < MIN_ROTATION)
                {
                    applyBrakes(0);
                    wheel_C_heading = wheel_C_heading + 180;
                    wheel_C_dir = !wheel_C_dir;
                    //printf("wheel C direction: %d\n", wheel_C_dir);
                    motor_C.SetDirection(wheel_C_dir);
                    //servo_C.SetPositionPercent(100 * wheel_C_heading/MAX_ROTATION);
                    //delay(1000);
                    applyBrakes(current_brakes);
                }
                else if (wheel_C_heading > MAX_ROTATION)
                {
                    applyBrakes(0);
                    wheel_C_heading = wheel_C_heading - 180;
                    wheel_C_dir = !wheel_C_dir;
                    //printf("wheel C direction: %d\n", wheel_C_dir);
                    motor_C.SetDirection(wheel_C_dir);
                    //servo_C.SetPositionPercent(100 * wheel_C_heading/MAX_ROTATION);
                    //delay(1000);
                    applyBrakes(current_brakes);
                }

                // Update servo positions and speed
                servo_A.SetPositionPercent(100 * wheel_A_heading/MAX_ROTATION);
                servo_B.SetPositionPercent(100 * wheel_B_heading/MAX_ROTATION);
                servo_C.SetPositionPercent(100 * wheel_C_heading/MAX_ROTATION);
            }
            if (speed != current_speed)
            {
                //printf("speed: %f\n", speed);
                setSpeedAllWheels(100 * speed);
                current_speed = speed;
            }
            // Delay 10 ms
            vTaskDelay(10);
        }

        while(Params->mode == SPIN)
        {
            if (Params->mode != SPIN)
            {
                continue;
            }
            if(previous_mode != SPIN)
            {
                applyBrakes(0);
                initSpinMode(0);
                previous_mode = SPIN;
            }
            printf("Spin Mode\n");
            //initSpinMode(0);
            if ((Params->AXIS_X * Params->THROTTLE) != current_speed)
            {
                current_speed = 100 * Params->AXIS_X * Params->THROTTLE;
                setDirectionAllWheels((current_speed > 0) ? 0:1);
                motor_A.SetSpeed(fabs(current_speed));
                motor_B.SetSpeed(fabs(current_speed));
                motor_C.SetSpeed(fabs(current_speed));
            }
            if (Params->button_0 != current_brakes)
            {
                applyBrakes(Params->button_0);
                current_brakes = Params->button_0;
            }
            // Delay 10 ms
            vTaskDelay(10);
        }

        while(Params->mode == DRIVE)
        {
            if (Params->mode != DRIVE)
            {
                continue;
            }
            if (previous_mode != DRIVE)
            {
                previous_mode = DRIVE;
                applyBrakes(0);
                left_servo = servo_A;
                right_servo = servo_B;
                back_servo = servo_C;
                left_motor = motor_A;
                right_motor = motor_B;
                back_motor = motor_C;
                angle_left = 210;
                angle_right = 90;
                angle_back = 150;
            }
            printf("Drive Mode\n");
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

                // following angle values for actual servos //
                angle_left = 210;
                angle_right = 90;
                angle_back = 150;
                
                previous_front = front;
            }
            
            target_speed = (0 - Params->AXIS_Y) * Params->THROTTLE;
            // Exponential Moving Average
            current_speed = (target_speed * 0.5) + (current_speed * (1 - 0.5));

            target_angle = driveModeMapping(Params->AXIS_X, Params->AXIS_Y);
            // Exponential Moving Average
            current_angle = (target_angle * 0.5) + (current_angle * (1 - 0.5));

            // Calculate parameters for turning left
            if (Params->AXIS_X < 0)
            {
                angle_right = current_angle;
                radius_rover = 2 * SIDE_2_MID * tan((90 - current_angle) * 3.1416/180) + SIDE;
                printf("rover radius: %fin\n", radius_rover);
                radius_right = sqrt(pow(radius_rover-S_HALF, 2)+pow(SIDE_2_MID, 2));
                printf("right radius: %fin\n", radius_right);
                radius_left = sqrt(pow(radius_rover+S_HALF, 2)+pow(SIDE_2_MID, 2));
                printf("left radius: %fin\n", radius_left);
                radius_back = sqrt(pow(radius_rover, 2) + pow(CORNER_2_MID, 2));
                printf("back radius: %fin\n", radius_back);
                angle_left = 90 - atan2(radius_rover+S_HALF, 2*SIDE_2_MID)*180/3.1416;
                angle_back = 90 - atan2(radius_rover, CORNER_2_MID) * 180/3.14159;


                // Max rot/s for hub motors = 3.6
                speed_left = fabs(current_speed);
                speed_right = speed_left * (radius_right/radius_rover);
                speed_back = speed_left * (radius_back/radius_rover);

            }

            // Calculate parameters for turning right
            if (Params->AXIS_X > 0)
            {
                angle_left = current_angle;
                radius_rover = 2 * SIDE_2_MID * tan((90 - current_angle) * 3.1416/180) + SIDE;
                printf("rover radius: %fin\n", radius_rover);
                radius_left = sqrt(pow(radius_rover-S_HALF, 2)+pow(SIDE_2_MID, 2));
                printf("left radius: %fin\n", radius_left);
                radius_right = sqrt(pow(radius_rover+S_HALF, 2)+pow(SIDE_2_MID, 2));
                printf("right radius: %fin\n", radius_right);
                radius_back = sqrt(pow(radius_rover, 2) + pow(CORNER_2_MID, 2));
                printf("back radius: %fin\n", radius_back);
                angle_right = 90 - atan2(radius_rover+S_HALF, 2*SIDE_2_MID)*180/3.1416;
                angle_back = 90 - atan2(radius_rover, CORNER_2_MID) * 180/3.14159;


                // Max rot/s for hub motors = 3.6
                speed_right = fabs(current_speed);
                speed_left = speed_right * (radius_left/radius_rover);
                speed_back = speed_right * (radius_back/radius_rover);
            }
            // If heading is straight forward, straighten out the rover
            if (Params->AXIS_X == 0)
            {
                initDriveMode(front);
                if (current_speed != previous_speed)
                {

                    left_motor.SetDirection((Params->AXIS_Y > 0) ? 1:0);
                    right_motor.SetDirection((Params->AXIS_Y > 0) ? 1:0);
                    back_motor.SetDirection((Params->AXIS_Y > 0) ? 0:1);
                    setSpeedAllWheels(100 * fabs(current_speed));
                    previous_speed = current_speed;
                }
                // following angle values for actual motors //
                angle_left = 210;
                angle_right = 90;
                angle_back = 150;
                
            }
            else if ((current_heading != Params->AXIS_X) | 
                    (current_speed != previous_speed))
            {
                
                if (Params->AXIS_X > 0)
                {
                    //printf("turning left\n");
                    left_servo.SetPositionPercent(left_servo.GetPercentage(300,
                                                (90 + fabs(angle_left))));
                    //printf("left angle: %f percent\n", 100 * (210 - 90 + fabs(angle_left))/300);
                    right_servo.SetPositionPercent(right_servo.GetPercentage(300,
                                                (210 + fabs(angle_right))));
                    //printf("right angle: %f percent\n", 100 * (90 - 90 + fabs(angle_right))/300);
                    back_servo.SetPositionPercent(back_servo.GetPercentage(300,
                                                (150 - fabs(angle_back))));
                }
                else
                {
                    //printf("turning right\n");
                    left_servo.SetPositionPercent(left_servo.GetPercentage(300,
                                                (90 - fabs(angle_left))));
                    //printf("left_servo: %f percent\n", 100 * (210 + 90 + fabs(angle_left))/300);
                    right_servo.SetPositionPercent(right_servo.GetPercentage(300,
                                                (210 - fabs(angle_right))));
                    //printf("right_angle: %f percent\n", 100 * (90 + 90 + fabs(angle_right))/300);
                    back_servo.SetPositionPercent(back_servo.GetPercentage(300,
                                                (150 + fabs(angle_back))));
                }

                left_motor.SetDirection((Params->AXIS_Y > 0) ? 1:0);
                right_motor.SetDirection((Params->AXIS_Y > 0) ? 1:0);
                back_motor.SetDirection((Params->AXIS_Y > 0) ? 0:1);
                
                left_motor.SetSpeed(100 * speed_left);
                //printf("speed left after adjustment: %f\n", speed_left);
                right_motor.SetSpeed(100 * speed_right);
                //printf("speed right after adjustment: %f\n", speed_right);
                back_motor.SetSpeed(100 * speed_back);
                //printf("speed back after adjustment: %f\n", speed_back);

                previous_speed = current_speed;

                current_angle_left = angle_left;
                current_angle_right = angle_right;
                current_angle_back = angle_back;

                current_heading = Params->AXIS_X;
            }
            // Delay 10 ms
            vTaskDelay(10);
        }
    }
}

