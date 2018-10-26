#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/event_groups.h"
extern "C"{
#include "Wifi.h"
#include "Structure.h"
}
//#include "utilities.h"
#include "RTOS_Tasks.h"


const Servo_t test_high = { ROTUNDA,   SERVO_LOW_SPEED_FREQ, LEDC_CHANNEL_0, HIGH_SPEED_MODE, HIGH_SPEED_TIMER, LEDC_TIMER_12_BIT };

const Servo_t test_low 	= { ELBOW,     SERVO_LOW_SPEED_FREQ, LEDC_CHANNEL_0, LOW_SPEED_MODE,  LOW_SPEED_TIMER,  LEDC_TIMER_12_BIT };

const Servo_t test_low2 = { CAM_SERVO, SERVO_LOW_SPEED_FREQ, LEDC_CHANNEL_1, LOW_SPEED_MODE,  LOW_SPEED_TIMER,  LEDC_TIMER_12_BIT };

const Servo_t rotunda 	= { ROTUNDA,   SERVO_LOW_SPEED_FREQ, LEDC_CHANNEL_0, LOW_SPEED_MODE,  LOW_SPEED_TIMER,  LEDC_TIMER_12_BIT };

const Servo_t elbow 	= { ELBOW,     SERVO_LOW_SPEED_FREQ, LEDC_CHANNEL_1, LOW_SPEED_MODE,  LOW_SPEED_TIMER,  LEDC_TIMER_12_BIT };

const Servo_t cam 		= { CAM_SERVO, 50, LEDC_CHANNEL_2, LOW_SPEED_MODE,  LOW_SPEED_TIMER,  LEDC_TIMER_10_BIT };

Motor_t shoulder 				= { SHOULDER,       MOTOR_FREQ, LEDC_CHANNEL_0, 100, 4, HIGH_SPEED_MODE, HIGH_SPEED_TIMER, LEDC_TIMER_10_BIT }; // keep non-const because of direction member var

Motor_t claw1 					= { LEADSCREW_IN_1, MOTOR_FREQ, LEDC_CHANNEL_1, 100, 4, HIGH_SPEED_MODE, HIGH_SPEED_TIMER, LEDC_TIMER_10_BIT }; // keep non-const because of direction member var

Motor_t claw2 					= { LEADSCREW_IN_2, MOTOR_FREQ, LEDC_CHANNEL_2, 100, 4, HIGH_SPEED_MODE, HIGH_SPEED_TIMER, LEDC_TIMER_10_BIT }; // keep non-const because of direction member var

Motor_t wrist_pitch1 			= { GEARBOX_1_IN_1, MOTOR_FREQ, LEDC_CHANNEL_3, 100, 4, HIGH_SPEED_MODE, HIGH_SPEED_TIMER, LEDC_TIMER_10_BIT }; // keep non-const because of direction member var

Motor_t wrist_pitch2 			= { GEARBOX_1_IN_2, MOTOR_FREQ, LEDC_CHANNEL_4, 100, 4, HIGH_SPEED_MODE, HIGH_SPEED_TIMER, LEDC_TIMER_10_BIT }; // keep non-const because of direction member var

Motor_t wrist_rotation1 		= { GEARBOX_2_IN_1, MOTOR_FREQ, LEDC_CHANNEL_5, 100, 4, HIGH_SPEED_MODE, HIGH_SPEED_TIMER, LEDC_TIMER_10_BIT }; // keep non-const because of direction member var

Motor_t wrist_rotation2 		= { GEARBOX_2_IN_2, MOTOR_FREQ, LEDC_CHANNEL_6, 100, 4, HIGH_SPEED_MODE, HIGH_SPEED_TIMER, LEDC_TIMER_10_BIT }; // keep non-const because of direction member var

current_pos_t current_pos;

current_pos_t feedback;

/*extern "C"*/ void vReadDataTask(void *pvParameters){
	read_data.data[read_cam_id] = 0;
	read_data.data[read_cam_shoulder] = 0;
	read_data.data[read_cam_elbow] = 0;
	read_data.data[read_gimbal] = 0;
	read_data.data[read_base] = 0;
	read_data.data[read_shoulder] = 90;
	read_data.data[read_elbow] = -700;
	read_data.data[read_wrist] = 0;
	read_data.data[read_wrist_rot] = 0;
	read_data.data[read_claw_motion] = 0;
	read_data.data[read_claw_torque] = 0;
   while(1){
     vTaskDelay(100 / portTICK_RATE_MS);
     //read data from mission control. 
     read_data_wifi(READ_ITEM_SIZE);
     // printf("READ DATA CAM\n");
     // printf("cam id: %i\t", 					read_data.data[read_cam_id]);
     printf("cam shoulder: %i\t", 				read_data.data[read_cam_shoulder]);
     printf("cam elbow: %i\t", 					read_data.data[read_cam_elbow]);
     printf("gimbal: %i\t", 					read_data.data[read_gimbal]);
     printf("rotunda: %i\t", 					read_data.data[read_base]);
     printf("shoulder: %i\t", 					read_data.data[read_shoulder]);
     printf("elbow: %i\t", 						read_data.data[read_elbow]);
     printf("wrist pitch: %i\t", 				read_data.data[read_wrist]);
     printf("wrist rotation: %i\t", 			read_data.data[read_wrist_rot]);
     printf("Claw: %i\t", 		read_data.data[read_claw_motion]);
     printf("wrist rot right button: %i\n", 	read_data.data[read_claw_torque]);
     // printf("shoulder current: %i\n", current_pos.shoulder);
   }
}

/*extern "C"*/ void vSendDataTask(void *pvParameters)
{
	while(1)
	{
		send_data.data[send_rotunda] = current_pos.base;
		send_data.data[send_shoulder] = current_pos.shoulder;
		send_data.data[send_elbow] = current_pos.elbow;
		send_data.data[send_wrist_pitch] = current_pos.wrist_pitch;
		send_data.data[send_wrist_rotation] = current_pos.wrist_rotation;
		send_data_wifi(SEND_ITEM_SIZE);
		vTaskDelay(100 / portTICK_RATE_MS);
	}
}

void vI2CTask(void *pvParameters)
{
	I2C_master_init(I2C_EXAMPLE_MASTER_SDA_IO, I2C_EXAMPLE_MASTER_SCL_IO);
	vTaskDelay(1000 / portTICK_RATE_MS);
	BNO_accel_opr_setup(I2C_EXAMPLE_MASTER_NUM, BNO055_NDOF_MODE);
	uint8_t mag_data[2];
	static esp_err_t mag_ret;
	static esp_err_t BNO_ret;
	static esp_err_t MPU_ret;
	uint8_t accel_data[6];
	static uint16_t mag_read_temp = 0;
	static int16_t BNO_X_temp = 0;
	static int16_t BNO_Y_temp = 0;
	static int16_t BNO_Z_temp = 0;
	static int16_t MPU_X_temp = 0;
	static int16_t MPU_Y_temp = 0;
	static int16_t MPU_Z_temp = 0;
	// vTaskDelay(1000 / portTICK_RATE_MS);

	while(1)
	{
		mag_ret = I2C_mag_read_multi(I2C_EXAMPLE_MASTER_NUM, AS5600_RAW_HIGH, mag_data, 2);
		// vTaskDelay(100 / portTICK_RATE_MS);
		BNO_ret = BNO_accel_read_multi(I2C_EXAMPLE_MASTER_NUM, 0x1A, accel_data, 6);

		BNO_X_temp = (accel_data[1] << 8) | accel_data[0];
		BNO_Y_temp = (accel_data[3] << 8) | accel_data[2];
		BNO_Z_temp = (accel_data[5] << 8) | accel_data[4];

		MPU_ret = MPU_accel_read_multi(I2C_EXAMPLE_MASTER_NUM, MPU_9250_X_HIGH, accel_data, 6);

		// accel_ret = MPU_accel_read_multi(I2C_EXAMPLE_MASTER_NUM, 0x43, accel_data, 6);

		MPU_X_temp = (accel_data[0] << 8) | accel_data[1];
		MPU_Y_temp = (accel_data[2] << 8) | accel_data[3];
		MPU_Z_temp = (accel_data[4] << 8) | accel_data[5];

		mag_read_temp = (mag_data[0] << 8) | mag_data[1];
		
		// printf("mag raw: %i\n", mag_read_temp);
		
		// if(mag_read_temp >= MAG_ENC_MIN && mag_read_temp <= MAG_ENC_MAX)
		// {
		if(mag_ret == ESP_OK)
		{
			feedback.shoulder = mag_read_temp;
		}
		else
		{
			feedback.shoulder = map(read_data.data[read_shoulder], SHOULDER_MIN, SHOULDER_MAX, MAG_ENC_MIN, MAG_ENC_MAX);
		}
		if(BNO_ret == ESP_OK)
		{
			feedback.wrist_pitch = BNO_Z_temp / 16;
		}	
		else
		{
			feedback.wrist_pitch = read_data.data[read_wrist];
		}
		if(MPU_ret == ESP_OK)
		{
			feedback.wrist_pitch = MPU_Z_temp / 182;
		}	
		else
		{
			feedback.wrist_pitch = read_data.data[read_wrist];
		}
		// }
		
		// printf("shoulder mag: %i\n", feedback.shoulder);
		// printf("accel ret: %X\taccel: %i\taccel_data5: %i\taccel_data4: %i\taccel_data3: %i\taccel_data2: %i\taccel_data1: %i\taccel_data0: %i\n", accel_ret, feedback.wrist_pitch, accel_data[5], accel_data[4], accel_data[3], accel_data[2], accel_data[1], accel_data[0]);

		// current_pos.shoulder = map(feedback.shoulder, MAG_ENC_MIN, MAG_ENC_MAX, SHOULDER_MIN, SHOULDER_MAX);
		current_pos.wrist_pitch = feedback.wrist_pitch;
		vTaskDelay(100 / portTICK_RATE_MS);


		// I2C_gimbal_write(I2C_EXAMPLE_MASTER_NUM, CTRL_REG1, 0x3F);
		// I2C_mag_write(I2C_EXAMPLE_MASTER_NUM, CTRL_REG1, 0x3F);
	}
}

void vIMUTask(void *pvParameters)
{
  uint16_t heading,roll,pitch;
  I2C_Init();
  BNO055_Init();
  //BNO055_Cabliration(); //use for calibration  
  //BNO055_ReadOffset(); //use for calibration 
  //BNO055_WriteOffset(); //use for calibration
  while(1)
  {
    heading = BNO055_GetHeading();
    roll = BNO055_GetRoll();
    pitch = BNO055_GetPitch();
    printf("\nheading:  %i, pitch: %i, roll: %i\n", heading/16,pitch,roll );
    // send_data.data[heading]=heading;
    // send_data.data[pitch]=pitch;
    // send_data.data[roll]=roll;
    // //printf("heading:  %i \n",heading);
  }
}

void vBIMUTask(void *pvParameters)
{
	Gimbalinit();
	int x;
	while(1)
	{
		x = GimbalgetData();
		printf("X: %i\n", x);
		vTaskDelay(100);
	}
}

// Servo Tasks

void vRotundaTask(void *pvParameters) // duty range 20% to 100%
{
	ServoTimerConfig(rotunda);
	ServoChannelConfig(rotunda);
	static uint16_t rotunda_pwm = 2400;
	static uint16_t rotunda_ema = 2400;
	SetServoAngle(rotunda, rotunda_ema, rotunda.speed_mode);
	vTaskDelay(10000 / portTICK_RATE_MS);

	// static uint16_t angle = 0;
	while(1)
	{
		// if(angle > 270)
		// {
		// 	angle = 0;
		// 	vTaskDelay(1000 / portTICK_RATE_MS);
		// }
		// else
		// {
		// 	angle += 30;
		// }
		// rotunda_pwm = map(read_data.data[read_base], 0, 270, 205, 1015);
		rotunda_pwm = map(read_data.data[read_base], ROTUNDA_MIN, ROTUNDA_MAX, SERVO_PWM_12_BIT_MIN, SERVO_PWM_12_BIT_MAX);
		// rotunda_ema = EMA_uint(rotunda_pwm, rotunda_ema, ROTUNDA_ALPHA);

		if(50 > rotunda_ema - rotunda_pwm)
		{
			rotunda_ema += 10;
		}
		else if(-50 < rotunda_ema - rotunda_pwm)
		{
			rotunda_ema -= 10;
		}
		else 
		{
			rotunda_ema = rotunda_ema;
		}
		constrain_uint(&rotunda_ema, ROTUNDA_PWM_12_BIT_MIN, ROTUNDA_PWM_12_BIT_MAX);
		// printf("r_ema: %i\n", rotunda_ema);
	        printf("rotunda: %i \n", rotunda_pwm);
		current_pos.base = map(rotunda_ema, SERVO_PWM_12_BIT_MIN, SERVO_PWM_12_BIT_MAX, ROTUNDA_MIN, ROTUNDA_MAX);
		SetServoAngle(rotunda, rotunda_pwm, rotunda.speed_mode);
		vTaskDelay(100 / portTICK_RATE_MS);
	}
}

void vElbowTask(void *pvParameters)	// duty range 20% to 100%
{
	ServoTimerConfig(elbow);
	ServoChannelConfig(elbow);
	/*
	static uint16_t elbow_pwm = 34;
	static uint16_t elbow_ema = 34;
	SetServoAngle(elbow, elbow_ema, elbow.speed_mode);
	vTaskDelay(3000 / portTICK_RATE_MS);
        */
        static int16_t elbow_init = -70;
	static uint16_t elbow_pwm = map(elbow_init, ELBOW_MIN, ELBOW_MAX, SERVO_PWM_12_BIT_MIN, SERVO_PWM_12_BIT_MAX);
	static uint16_t elbow_ema = 4095 / 2;
	SetServoAngle(elbow, elbow_pwm, elbow.speed_mode);
	vTaskDelay(13000 / portTICK_RATE_MS);
	elbow_pwm = map(-80, ELBOW_MIN, ELBOW_MAX, SERVO_PWM_12_BIT_MIN, SERVO_PWM_12_BIT_MAX);
	SetServoAngle(elbow, elbow_pwm, elbow.speed_mode);
	vTaskDelay(4000 / portTICK_RATE_MS);
	while(1)
	{
		// if(elbow_pwm > 1024)
		// {
		// 	elbow_pwm = 205;
		// 	// SetServoAngle(elbow, elbow_pwm, elbow.speed_mode);
		// 	vTaskDelay(1000 / portTICK_RATE_MS);
		// }
		// else
		// {
		// 	elbow_pwm += 100;
		// }

		elbow_pwm = map(read_data.data[read_elbow], ELBOW_MIN, ELBOW_MAX, SERVO_PWM_12_BIT_MIN, SERVO_PWM_12_BIT_MAX);
		elbow_ema = EMA_uint(elbow_pwm, elbow_ema, ELBOW_ALPHA);
		constrain_uint(&elbow_ema, SERVO_PWM_12_BIT_MIN, SERVO_PWM_12_BIT_MAX);
		current_pos.elbow = map(elbow_ema, SERVO_PWM_12_BIT_MIN, SERVO_PWM_12_BIT_MAX, ELBOW_MIN, ELBOW_MAX);
		SetServoAngle(elbow, elbow_pwm, elbow.speed_mode);
		// printf("elbow: %i\n", elbow_pwm);
		vTaskDelay(100 / portTICK_RATE_MS);

	}
}

void vCamTask(void *pvParameters) // duty range 20% to 100%
{
	ServoTimerConfig(cam);
	ServoChannelConfig(cam);
	static uint16_t angle;
	static uint16_t angle_ema;
	while(1)
	{
		if(read_data.data[read_cam_id] == 1)
		{
			gpio_set_level(CAM_MUX_SEL, 1);
		}
		else
		{
			gpio_set_level(CAM_MUX_SEL, 0);
		}
		angle = map(read_data.data[read_cam_shoulder], CAM_MIN, CAM_MAX, CAM_PWM_MIN, CAM_PWM_MAX);
		angle_ema = EMA_uint(angle, angle_ema, CAM_ALPHA);
		constrain_uint(&angle_ema, 0, 1023);
		SetServoAngle(cam, angle, cam.speed_mode);
		printf("cam_ang: %i\tcam_ema: %i\n", angle, angle_ema);
		vTaskDelay(100 / portTICK_RATE_MS);
	}
}

// Motor Tasks

void vShoulderTask(void *pvParameters)
{
	MotorTimerConfig(shoulder);
	MotorChannelConfig(shoulder);
	static uint16_t angle;
	static uint16_t current;
	static uint16_t current_test;
	static uint8_t data[2];
	static esp_err_t ret;
	static bool direction;
	static int16_t shoulder_offset_duration = 0;
	static double error;
	uint8_t proportion = 3;
	static double pwm_avg = 0;
	static uint16_t pwm_out = 0;
	read_data.data[read_shoulder] = 90;
	// PID_t test_pid;
	// PidInit(&test_pid);
	// SetMode(&test_pid, AUTOMATIC);
	// SetTunings(&test_pid, 10, 10, 0);
	// SetOutputLimits(&test_pid, 0, 270);
	// test_pid.Setpoint = 270;
	vTaskDelay(5000 / portTICK_RATE_MS);
	while(1)
	{

		// PID Placeholder
		// test_pid.Input = feedback.shoulder;
		// Compute(&test_pid);
		angle = map(read_data.data[read_shoulder], SHOULDER_MIN, SHOULDER_MAX, MAG_ENC_MIN, MAG_ENC_MAX);

//////// CLOSED LOOP /////////////////////
		current_test = feedback.shoulder;
		constrain_uint(&current_test, MAG_ENC_MIN, MAG_ENC_MAX);
		current_test = MagEncOffset(current_test, MAG_ENC_OFFSET, MAG_ENC_MIN, MAG_ENC_MAX);
		error = (double)angle - (double)current_test;

		// printf("shoulder offset: %i\t", current_test);

		double pwm_p = error * proportion;
		pwm_avg = EMA_double(pwm_p, pwm_avg, SHOULDER_ALPHA);
		constrain_double(&pwm_avg, SHOULDER_PWM_MIN, SHOULDER_PWM_MAX);
		direction = (error > 0) ? 0 : 1;
		
		current_pos.shoulder = map(feedback.shoulder, MAG_ENC_MIN, MAG_ENC_MAX, SHOULDER_MIN, SHOULDER_MAX);

		if(abs(error) < 50)
		{
			pwm_avg = 0;
			SetShoulderAngle(shoulder, 0, direction, shoulder.speed_mode);
		}
		else
		{
			SetShoulderAngle(shoulder, 300, direction, shoulder.speed_mode);
		}
		// printf("shoulder angle: %i\tshoulder mag: %i\t", angle, feedback.shoulder);
		// printf("error: %f\n", error);

		// debug
		// printf("pwm_avg: %f\tangle: %i\tcurrent: %i\terror: %f\tpwm_p: %f\tdirection: %i\n", pwm_avg, angle, current_test, error, pwm_p, direction);
		
		// SetShoulderAngle(shoulder, abs(pwm_avg), direction, shoulder.speed_mode);


//////// OPEN LOOP ///////////////////////
		// current = 90;
		// current = map(current, 0, 180, MAG_ENC_MIN, MAG_ENC_MAX);
		// error = current - angle;

		// if(error < -300)
		// {
		// 	direction = REVERSE;
		// 	SetShoulderAngle(shoulder, 400, direction, shoulder.speed_mode);
		// 	shoulder_offset_duration++;
		// }
		// else if(error > 300)
		// {
		// 	direction = FORWARD;
		// 	SetShoulderAngle(shoulder, 300, direction, shoulder.speed_mode);
		// 	shoulder_offset_duration++;
		// }
		// else
		// {
		// 	SetShoulderAngle(shoulder, 0, direction, shoulder.speed_mode);
		// 	shoulder_offset_duration = 0;
		// }

//////////////////////////////////////////////////////////////////////////////////

        // printf("DIR: %i\tgpio: %i\tshoulder: %i\t", direction, gpio_get_level(SHOULDER_DIR_PIN), read_data.data[read_shoulder]);
		// printf("SHOULDER MAG: %i\tSHOULDER CURRENT: %i\tSHOULDER ANGLE: %i\tDIFF: %i\n", current_pos.shoulder, current, angle, (current - angle));

		// angle = map(read_data.data[read_shoulder], 0, 180, -100, 100);
		// SetShoulderAngle(shoulder, angle, current, shoulder.speed_mode);
		
		vTaskDelay(100 / portTICK_RATE_MS);
	}
}

void vWristPitchTask(void *pvParameters)
{
	MotorTimerConfig(wrist_pitch1);
	MotorTimerConfig(wrist_pitch2);
	MotorChannelConfig(wrist_pitch1);
	MotorChannelConfig(wrist_pitch2);
	// static int16_t angle;
	
	MotorTimerConfig(shoulder);
	MotorChannelConfig(shoulder);
	static int16_t angle;
	static int16_t current;
	static uint16_t current_test;
	static esp_err_t ret;
	static bool direction;
	static int16_t shoulder_offset_duration = 0;
	static int16_t error;
	uint8_t proportion = 3;
	static double pwm_avg = 0;
//	vTaskDelay(1000 / portTICK_RATE_MS);
        SetWrist(wrist_pitch1, wrist_pitch2, -350, feedback.wrist_pitch, wrist_pitch1.speed_mode); 
        vTaskDelay(3000 / portTICK_RATE_MS);
        SetWrist(wrist_pitch1, wrist_pitch2, 0, feedback.wrist_pitch, wrist_pitch1.speed_mode);
	// PID_t wrist_pid;
	// PidInit(&wrist_pid);
	// SetMode(&wrist_pid);
	while(1)
	{
		
		// PID Placeholder

		// angle = map(read_data.data[read_wrist], PITCH_MIN, PITCH_MAX, PITCH_PWM_MIN, PITCH_PWM_MAX);
		// current_test = feedback.wrist_pitch;

		angle = map(read_data.data[read_wrist], PITCH_MIN, PITCH_MAX, IMU_Z_MIN / 2, IMU_Z_MAX / 2);
		// angle = angle / 16;
////////////////////// CLOSED LOOP SYSTEM ////////////////////////////////////////////
		if(read_data.data[read_gimbal])
		{
			current_test = feedback.wrist_pitch;

			error = angle - current_test;
			// double pwm_p = error * proportion;
			
			// pwm_avg = EMA_double(pwm_p, pwm_avg, PITCH_ALPHA);
			// constrain_double(&pwm_avg, PITCH_PWM_MIN, PITCH_PWM_MAX);
			
			// // debug
			// // printf("pwm_avg: %f\tangle: %i\tcurrent: %i\terror: %f\tpwm_p: %f\tdirection: %i\n", pwm_avg, angle, current_test, error, pwm_p, direction);
			
			if(error > 5)
			{
				SetWrist(wrist_pitch1, wrist_pitch2, 350, feedback.wrist_pitch, wrist_pitch1.speed_mode);
			}
			else if(error < -5)
			{
				SetWrist(wrist_pitch1, wrist_pitch2, -350, feedback.wrist_pitch, wrist_pitch1.speed_mode);
			}
			else
			{
				SetWrist(wrist_pitch1, wrist_pitch2, 0, feedback.wrist_pitch, wrist_pitch1.speed_mode);
			}

			printf("current: %i\terror: %i\n", current_test, error);
		}
//////// OPEN LOOP ///////////////////////
		else
		{		
			current = 0;
			// current = map(current, -90, 90, IMU_X_MIN, IMU_X_MAX);
			error = current - angle;

			// printf("error: %i\n", error);

			if(error < -30)
			{
				SetWrist(wrist_pitch1, wrist_pitch2, -350, feedback.wrist_pitch, wrist_pitch1.speed_mode);
				printf("neg\n");
			}
			else if(error > 30)
			{
				SetWrist(wrist_pitch1, wrist_pitch2, 350, feedback.wrist_pitch, wrist_pitch1.speed_mode);
				printf("pos\n");
			}
			else
			{
				SetWrist(wrist_pitch1, wrist_pitch2, 0, feedback.wrist_pitch, wrist_pitch1.speed_mode);
				printf("still\n");
			}
		}
//////////////////////////////////////////////////////////////////////////////////
		printf("pitch: %i\n", feedback.wrist_pitch);
		vTaskDelay(100 / portTICK_RATE_MS);
	}
}

void vWristRotationTask(void *pvParameters)
{
	MotorTimerConfig(wrist_rotation1);
	MotorTimerConfig(wrist_rotation2);
	MotorChannelConfig(wrist_rotation1);
	MotorChannelConfig(wrist_rotation2);
	// static uint8_t angle;
	// static uint8_t current;
	while(1)
	{
		
		// PID Placeholder

		// rotary control
		if(read_data.data[read_wrist_rot] < 270 && read_data.data[read_wrist_rot] > 180)
		{
			RotateWrist(wrist_rotation1, wrist_rotation2, 800, current_pos.wrist_rotation, wrist_rotation1.speed_mode);
		}
		else if(read_data.data[read_wrist_rot] > 90 && read_data.data[read_wrist_rot] < 180)
		{
			RotateWrist(wrist_rotation1, wrist_rotation2, -800, current_pos.wrist_rotation, wrist_rotation1.speed_mode);
		}
		else
		{
			RotateWrist(wrist_rotation1, wrist_rotation2, 0, current_pos.wrist_rotation, wrist_rotation1.speed_mode);
		}
		//button control

		vTaskDelay(100 / portTICK_RATE_MS);
	}
}

void vClawTask(void *pvParameters)
{
	MotorTimerConfig(claw1);
	MotorTimerConfig(claw2);
	MotorChannelConfig(claw1);
	MotorChannelConfig(claw2);
	static uint8_t ctrl = 0;
	while(1)
	{
		
		// PID Placeholder
		if(read_data.data[read_claw_motion] == OPEN)
		{
			ctrl = OPEN;
		}
		else if(read_data.data[read_claw_motion] == CLOSE)
		{
			ctrl = CLOSE;
		}
		else
		{
			ctrl = IDLE;
		}
		// printf("claw: %i\n", ctrl);
		// ClawControl(claw1, claw2, ctrl, read_data.data[read_claw_torque], claw1.speed_mode);
		ClawControl(claw1, claw2, ctrl, 200, claw1.speed_mode);
		vTaskDelay(100 / portTICK_RATE_MS);
	}
}



///////////////////////////////////////////////////////////////
//                                                           //
//                      Testing Tasks                       //
//                                                          //
//////////////////////////////////////////////////////////////

void vMotorTask(void *pvParameters)
{
	MotorTimerConfig(claw1);
	MotorTimerConfig(claw2);
	MotorChannelConfig(claw1);
	MotorChannelConfig(claw2);
	while(1)
	{

	}
}

void vServoTask(void *pvParameters)
{
	ServoTimerConfig(rotunda);
	ServoTimerConfig(elbow);
	ServoTimerConfig(cam);
	
	ServoChannelConfig(rotunda);
	ServoChannelConfig(elbow);
	ServoChannelConfig(cam);

	static uint16_t rotunda_pwm = 0;
	static uint16_t elbow_pwm = 0;
	static uint16_t cam_pwm = 0;
	while(1)
	{
		rotunda_pwm = read_data.data[read_base];
		elbow_pwm = read_data.data[read_elbow];
		cam_pwm = read_data.data[read_cam_id];
		SetServoAngle(rotunda, rotunda_pwm, rotunda.speed_mode);
		SetServoAngle(elbow, elbow_pwm, elbow.speed_mode);
		SetServoAngle(cam, cam_pwm, cam.speed_mode);
		
	}
}

void vMagRawTest(void *pvParameters)
{
	uint8_t data[2];
	uint16_t angle;
	while(1)
	{
		// printf("reading mag\n");
		I2C_mag_read_multi(I2C_EXAMPLE_MASTER_NUM, AS5600_ANGLE_HIGH, data, 2);
		angle = (data[0] << 8) | data[1];
		// printf("Mag Raw: %X %X Mag Angle: %i\n", data[0], data[1], angle);
	}	
}

void vImuIdRead(void *pvParameters)
{
	uint8_t data[6];
	uint16_t X;
	I2C_master_init(I2C_EXAMPLE_MASTER_SDA_IO, I2C_EXAMPLE_MASTER_SCL_IO);
	BNO_accel_opr_setup(I2C_EXAMPLE_MASTER_NUM, BNO055_NDOF_MODE);
	vTaskDelay(1000 / portTICK_RATE_MS);
	while(1)
	{
		// I2C_accel_opr_setup(I2C_EXAMPLE_MASTER_NUM, BNO055_NDOF_MODE);
		// vTaskDelay(1000 / portTICK_RATE_MS);

		// printf("READING IMU | ");
		BNO_accel_read_multi(I2C_EXAMPLE_MASTER_NUM, BNO055_X_HIGH_ADDR, data, 2);
		X = (data[0] << 8) | data[1];
		// printf("Raw: %X %X X: %i\n", data[0], data[1], X);
		vTaskDelay(1000 / portTICK_RATE_MS);
	}	
}

void vServoTestTask(void *pvParameters)
{
	ServoTimerConfig(test_high);
	ServoChannelConfig(test_high);
	ServoTimerConfig(test_low);
	ServoChannelConfig(test_low);
	ServoTimerConfig(test_low2);
	ServoChannelConfig(test_low2);
	// WRITE_PERI_REG (LEDC_APB_CLK, 0);
	static uint16_t PWM = 1;
	static uint16_t PWM_HIGH = 1;
	while(1)
	{
		// WRITE_PERI_REG (LEDC_LSCH0_DUTY_REG, (PWM * 100 / 100) << 4);
		// WRITE_PERI_REG (LEDC_LSCH0_DUTY_REG, (PWM * 100 / 100) << 4);
		// WRITE_PERI_REG(LEDC_LSCH0_CONF1_REG, ( 1 << 31 ));
		// WRITE_PERI_REG(LEDC_LSCH0_CONF0_REG, ( 1 << 4 ));
		// printf("PWM: %i APB Clock: %li ls pwm: %i low freq: %i\n", PWM, LEDC_APB_CLK_SEL, ledc_get_duty(LOW_SPEED_MODE, LEDC_CHANNEL_0), ledc_get_freq(LOW_SPEED_MODE, LOW_SPEED_TIMER));
		SetServoAngle(test_high, PWM, test_high.speed_mode);
		SetServoAngle(test_low, 500, test_low.speed_mode);
		SetServoAngle(test_low2, 200, test_low2.speed_mode);
		vTaskDelay(100 / portTICK_RATE_MS);
		if(PWM_HIGH > 1000)
		{
			PWM_HIGH += 5;
		}
		else
		{
			PWM_HIGH = 0;
		}
		if(PWM < 250)
		{
		PWM += 5;
		}
		else
		{
			PWM = 1;
		}
	}
}

void vMotorTestTask(void *pvParameters)
{
	MotorTimerConfig(shoulder);
	MotorChannelConfig(shoulder);
	PID_t test_pid;
	int16_t pwm = 0;
	int16_t current = 0;
	PidInit(&test_pid);
	SetMode(&test_pid, AUTOMATIC);
	// printf("auto: %i \n", test_pid.inAuto);
	SetTunings(&test_pid, 10, 10, 0);
	SetOutputLimits(&test_pid, 1000, 1000);
	test_pid.Setpoint = 1000;
	while(1)
	{
		test_pid.Input = current;
		// printf("input: %f Setpoint: %f ", test_pid.Input, test_pid.Setpoint);
		Compute(&test_pid);
		SetShoulderAngle(shoulder, test_pid.Output, current, shoulder.speed_mode);
		if(test_pid.controllerDirection == DIRECT)
		{
			current += 100;
		}
		if(test_pid.controllerDirection == REVERSE)
		{
			current -= 100;
		}
		if(current >= test_pid.Setpoint && test_pid.Setpoint == 1000)
		{
			test_pid.controllerDirection = REVERSE;
			test_pid.Setpoint = 0;
		}
		if(current < test_pid.Setpoint && test_pid.Setpoint == 0)
		{
			test_pid.controllerDirection = DIRECT;
			test_pid.Setpoint = 1000;
		}
		vTaskDelay(100 / portTICK_RATE_MS);
	}
}

void vQuadEncTestTask(void *pvParameters)
{
	static int16_t count = 0;
	static uint8_t quad = 0;
	while(1)
	{
		QuadEnc(&count, &quad);
		// printf("quad: %X state: %i ", quad, (quad & 0x0F));
		// printf("count: %i\n", count);
	}
}

void vAdcTask(void *pvParameters)
{
	static uint32_t voltage;
	esp_adc_cal_characteristics_t cal;
	adc1_config(ADC1_PIN_36, ADC_WIDTH_10Bit, cal);
	while(1)
	{
		voltage = adc1_to_voltage(ADC1_PIN_36, &cal);
		// printf("%d mV\n", voltage);
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}
