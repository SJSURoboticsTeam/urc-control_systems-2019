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

    // Drive MODE variables //
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
    

    // Crab/Spin MODE variables //
    double wheel_arr[6] = {0};
    double cam_offset = 0;
    double heading_x = 0;
    double heading_y = 0;
    double heading = 0;
    double target_heading = 0;
    double previous_heading = 0;
    double wheel_position = 0;
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
        while(Params->MODE == DEBUG)
        {
            if (Params->MODE != DEBUG)
            {
                continue;
            }
            if(previous_mode != DEBUG)
            {
                previous_mode = DEBUG;
                applyBrakes(0);    
            }
            printf("Debug MODE\n");
            // Convert AXIS_X to 0% - 100% scale
            target_heading = (1 + Params->AXIS_X) * 50;
            heading = target_heading;
            // Exponential Moving Average
            //heading = (target_heading * 0.5) + (heading * (1 - 0.5));
            // Calculate speed based on AXIS_Y% of THROTTLE
            target_speed = 100 * (Params->BRAKES) * Params->THROTTLE * Params->T_MAX;
            // Exponential Moving Average
            speed = (target_speed * 0.5) + (speed * (1 - 0.5));
            if (heading != previous_heading)
            {
                if (Params->WHEEL_A)
                {
                    servo_A.SetPositionPercent(heading);
                    printf("Wheel A set to: %f deg.\n", heading/100 * 300);
                }
                if (Params->WHEEL_B)
                {
                    servo_B.SetPositionPercent(heading);
                    printf("Wheel B set to: %f deg.\n", heading/100 * 300);                
                }
                if (Params->WHEEL_C)
                {
                    servo_C.SetPositionPercent(heading);
                    printf("Wheel C set to: %f deg.\n", heading/100 * 300);
                }
                previous_heading = heading;
            }
            if (speed != previous_speed)
            {
                if (Params->WHEEL_A)
                {
                    motor_A.SetSpeed(fabs(speed));
                    printf("Wheel A speed set to %f percent\n", speed);
                    motor_A.SetDirection((Params->REVERSE) ? 0:1);
                    printf("Wheel A direction: %d\n", (Params->REVERSE) ? 0:1);
                }
                if (Params->WHEEL_B)
                {
                    motor_B.SetSpeed(fabs(speed));
                    printf("Wheel B speed set to %f percent\n", speed);
                    motor_B.SetDirection((Params->REVERSE) ? 0:1);
                    printf("Wheel B direction: %d\n", (Params->REVERSE) ? 0:1);
                }
                if (Params->WHEEL_C)
                {
                    motor_C.SetSpeed(fabs(speed));
                    printf("Wheel C speed set to %f percent\n", speed);
                    motor_C.SetDirection((Params->REVERSE) ? 0:1);
                    printf("Wheel C direction: %d\n", (Params->REVERSE) ? 0:1);
                }
                previous_speed = speed;
            }
            if (Params->TRIGGER)
            {
                if (Params->WHEEL_A)
                {
                    motor_A.Brake(0);
                }
                else
                {
                    motor_A.Brake(1);
                }
                if (Params->WHEEL_B)
                {
                    motor_B.Brake(0);
                }
                else
                {
                    motor_B.Brake(1);
                }
                if (Params->WHEEL_C)
                {
                    motor_C.Brake(0);
                }
                else
                {
                    motor_C.Brake(1);
                }
            }
            else
            {
                applyBrakes(0);
            }
            // Delay 50 ms
            vTaskDelay(50);

        }

        while(Params->MODE == CRAB)
        {
            if (Params->MODE != CRAB)
            {
                continue;
            }
            if(previous_mode != CRAB)
            {
                printf("Crab MODE\n");
                applyBrakes(0);
                cam_offset = Params->MAST_POSITION;
                SetForward(cam_offset, wheel_arr);
                wheel_A_heading = wheel_arr[0];
                wheel_A_dir = wheel_arr[1];
                //printf("Wheel A: %f, dir: %d\n", wheel_A_heading, wheel_A_dir);
                wheel_B_heading = wheel_arr[2];
                wheel_B_dir = wheel_arr[3];
                //printf("Wheel B: %f, dir: %d\n", wheel_B_heading, wheel_B_dir);
                wheel_C_heading = wheel_arr[4];
                wheel_C_dir = wheel_arr[5];
                //printf("Wheel C: %f, dir: %d\n", wheel_C_heading, wheel_C_dir);
                wheel_position = 0;
                previous_mode = CRAB;
            }
            //printf("Crab MODE\n");
            if (Params->TRIGGER != current_brakes)
            {
                if(!Params->TRIGGER)
                {
                    while(speed != 0 && speed >= 0.000001)
                    {
                        // Exponential Moving Average
                        speed = speed * (1 - 0.2);
                        setSpeedAllWheels(speed);
                        //printf("speed: %f\n", speed);
                    }
                }
                applyBrakes(Params->TRIGGER);
                current_brakes = Params->TRIGGER;
            }
            // Calculate current speed
            if ((abs(Params->AXIS_X) == 1) | (abs(Params->AXIS_Y) == 1))
            {
                target_speed = Params->THROTTLE;
            }
            else
            {
                target_speed = sqrt(pow(Params->AXIS_X, 2) + pow(Params->AXIS_Y, 2))/1.414124 * Params->THROTTLE * Params->BRAKES;
            }
            // Exponential Moving Average
            speed = ((target_speed * 0.5) + (speed * (1 - 0.5))) * Params->T_MAX;
            // Adjust parameters for new instance of crab MODE
            if (cam_offset != Params->MAST_POSITION)
            {
                cam_offset = Params->MAST_POSITION;
                SetForward(cam_offset, wheel_arr);
                wheel_A_heading = wheel_arr[0];
                wheel_A_dir = wheel_arr[1];
                //printf("Wheel A: %f, dir: %d\n", wheel_A_heading, wheel_A_dir);
                wheel_B_heading = wheel_arr[2];
                wheel_B_dir = wheel_arr[3];
                //printf("Wheel B: %f, dir: %d\n", wheel_B_heading, wheel_B_dir);
                wheel_C_heading = wheel_arr[4];
                wheel_C_dir = wheel_arr[5];
                //printf("Wheel C: %f, dir: %d\n", wheel_C_heading, wheel_C_dir);
                wheel_position = 0;
            }
            // Update parameters for new heading from mission control
            heading_y = Params->AXIS_X;
            heading_x = 0 - Params->AXIS_Y;
            target_heading = atan2(heading_y, heading_x) * 180/3.1416; 
            
            if (current_heading != target_heading)
            {
                // Exponential Moving Average
                //heading = (target_heading * 0.5) + (heading * (1 - 0.5));
                heading = target_heading;
                //printf("heading: %f\n", heading);
                wheel_A_heading = wheel_A_heading - (current_heading - heading);
                //printf("wheel_A_heading: %f\n", wheel_A_heading);
                wheel_B_heading = wheel_B_heading - (current_heading - heading);
                //printf("wheel_B_heading: %f\n", wheel_B_heading);
                wheel_C_heading = wheel_C_heading - (current_heading - heading);
                //printf("wheel_C_heading: %f\n", wheel_C_heading);
                wheel_position = wheel_position - (current_heading - heading);
                //printf("wheel_position: %f\n", wheel_position);
                current_heading = heading;

                // If wheels hit boundaries, flip them 180 degrees and switch 
                // direction they rotate.
                if (wheel_position < MIN_ROTATION)
                {
                    wheel_A_heading = wheel_A_heading + 180;
                    wheel_A_dir = !wheel_A_dir;
                    wheel_B_heading = wheel_B_heading + 180;
                    wheel_B_dir = !wheel_B_dir;
                    wheel_C_heading = wheel_C_heading + 180;
                    wheel_C_dir = !wheel_C_dir;
                    wheel_position = wheel_position + 180;
                    motor_A.SetDirection(wheel_A_dir);
                    motor_B.SetDirection(wheel_B_dir);
                    motor_C.SetDirection(wheel_C_dir);
                }
                else if (wheel_position > MAX_ROTATION)
                {
                    wheel_A_heading = wheel_A_heading - 180;
                    wheel_A_dir = !wheel_A_dir;
                    wheel_B_heading = wheel_B_heading - 180;
                    wheel_B_dir = !wheel_B_dir;
                    wheel_C_heading = wheel_C_heading - 180;
                    wheel_C_dir = !wheel_C_dir;
                    wheel_position = wheel_position - 180;
                    motor_A.SetDirection(wheel_A_dir);
                    motor_B.SetDirection(wheel_B_dir);
                    motor_C.SetDirection(wheel_C_dir);
                }

                // Update servo positions and speed
                servo_A.SetPositionPercent(100 * (wheel_A_heading/300));
                servo_B.SetPositionPercent(100 * (wheel_B_heading/300));
                servo_C.SetPositionPercent(100 * (wheel_C_heading/300));
                //printf("Wheel A angle: %f\n", 100 *(wheel_A_heading + 90)/(2 * MAX_ROTATION));
                //printf("Wheel B angle: %f\n", 100 * (wheel_B_heading + 90)/(2 * MAX_ROTATION));
                //printf("Wheel C angle: %f\n", 100 *(wheel_C_heading + 90)/(2 * MAX_ROTATION));
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

        while(Params->MODE == SPIN)
        {
            if (Params->MODE != SPIN)
            {
                continue;
            }
            if(previous_mode != SPIN)
            {
                applyBrakes(0);
                initSpinMode(0);
                previous_mode = SPIN;
            }
            printf("Spin MODE\n");
            //initSpinMode(0);
            target_speed = 100 * Params->BRAKES * Params->THROTTLE * Params->YAW * Params->T_MAX;
            if (target_speed != current_speed)
            {
                
                //target_speed = 100 * Params->AXIS_X * Params->THROTTLE;
                // Exponential Moving Average
                current_speed = (target_speed * 0.5) + (current_speed * (1 - 0.5));
                printf("current speed: %f\n", current_speed);
                setDirectionAllWheels((current_speed > 0) ? 0:1);
                motor_A.SetSpeed(fabs(current_speed));
                motor_B.SetSpeed(fabs(current_speed));
                motor_C.SetSpeed(fabs(current_speed));
            }
            if (Params->TRIGGER != current_brakes)
            {
                if(!Params->TRIGGER)
                {
                    while(current_speed != 0 && current_speed >= 0.001)
                    {   
                        // Exponential Moving Average
                        current_speed = current_speed * (1 - 0.7);
                        setSpeedAllWheels(current_speed);
                        printf("speed: %f\n", speed);
                    }
                }
                applyBrakes(Params->TRIGGER);
                current_brakes = Params->TRIGGER;
            }
            // Delay 10 ms
            vTaskDelay(10);
        }

        while(Params->MODE == DRIVE)
        {
            if (Params->MODE != DRIVE)
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
            printf("Drive MODE\n");
            // Set the brakes
            if (current_brakes != Params->TRIGGER)
            {
                // use an exponential moving average to "slowly" apply the brakes
                if(!Params->TRIGGER)
                {
                    bool condition_a = false;
                    bool condition_b = false;
                    if(speed_left >= 0.00001 || speed_back >= 0.0000001 || speed_right >= 0.0000001)
                    {
                        condition_a = true;
                    }
                    if(speed_left != 0 || speed_right != 0 || speed_back != 0)
                    {
                        condition_b = true;
                    }
                    while(condition_a && condition_b)
                    {
                        // Exponential Moving Average
                        speed_left = speed_left * (1 - 0.2);
                        speed_right = speed_right * (1 - 0.2);
                        speed_back = speed_back * (1 - 0.2);
                        printf("speed left: %f\n", speed_left);
                        printf("speed right: %f\n", speed_right);
                        printf("speed back: %f\n", speed_back);

                        // Set Speeds
                        left_motor.SetSpeed(100 * speed_left);
                        right_motor.SetSpeed(100 * speed_right);
                        back_motor.SetSpeed(100 * speed_back);

                        if(speed_left <= 0.0000001 || speed_back <= 0.0000001 || speed_right <= 0.0000001)
                        {
                            condition_a = false;
                        }
                        if(speed_left == 0 || speed_right == 0 || speed_back == 0)
                        {
                            condition_b = false;
                        }
                    }
                }
                applyBrakes(Params->TRIGGER);
                current_brakes = Params->TRIGGER;
            }

            // Update the front of the rover
            if (Params->WHEEL_A)
            {
                front = 1;
            }
            else if (Params->WHEEL_B)
            {
                front = 2;
            }
            else if (Params->WHEEL_C)
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
            
            target_speed = Params->BRAKES * Params->THROTTLE * Params->T_MAX;
            // Exponential Moving Average
            current_speed = (target_speed * 0.5) + (current_speed * (1 - 0.5));

            target_angle = driveModeMapping(Params->AXIS_X, Params->AXIS_Y, Params->REVERSE);
            // Exponential Moving Average
            //current_angle = (target_angle * 0.5) + (current_angle * (1 - 0.5));
            current_angle = target_angle;
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
                printf("Right angle: %f\n", angle_right);
                printf("Left angle: %f\n", angle_left);
                printf("Back angle: %f\n", angle_back);

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
                printf("Right angle: %f\n", angle_right);
                printf("Left angle: %f\n", angle_left);
                printf("Back angle: %f\n", angle_back);


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

                    left_motor.SetDirection((Params->REVERSE > 0) ? 1:0);
                    right_motor.SetDirection((Params->REVERSE > 0) ? 1:0);
                    back_motor.SetDirection((Params->REVERSE > 0) ? 0:1);
                    if(current_speed == 0)
                    {
                        applyBrakes(0);
                        current_brakes = 0;
                    }
                    else if(current_speed != 0)
                    {
                        applyBrakes(current_brakes);
                        setSpeedAllWheels(100 * fabs(current_speed));

                    }
                    previous_speed = current_speed;
                }
                // following angle values for actual motors //
                angle_left = 210;
                angle_right = 90;
                angle_back = 150;
                
            }
            else if ((current_heading != Params->AXIS_X) || 
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
                else if(Params->AXIS_X < 0)
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

                left_motor.SetDirection((Params->REVERSE > 0) ? 1:0);
                right_motor.SetDirection((Params->REVERSE > 0) ? 1:0);
                back_motor.SetDirection((Params->REVERSE > 0) ? 0:1);
                

                if(current_speed == 0)
                {
                    applyBrakes(0);
                    current_brakes = 0;
                }
                else
                {
                    applyBrakes(current_brakes);
                    left_motor.SetSpeed(100 * speed_left);
                    //printf("speed left after adjustment: %f\n", speed_left);
                    right_motor.SetSpeed(100 * speed_right);
                    //printf("speed right after adjustment: %f\n", speed_right);
                    back_motor.SetSpeed(100 * speed_back);
                    //printf("speed back after adjustment: %f\n", speed_back);
                }
                /*
                if(current_speed == 0 && previous_speed != 0)
                {
                    applyBrakes(0);
                }
                else if(current_speed != 0 && previous_speed == 0)
                {
                    applyBrakes(current_brakes);
                }*/
                printf("current_speed: %f\n", current_speed);
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

