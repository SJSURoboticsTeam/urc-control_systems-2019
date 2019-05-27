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
#include "vector.h"

extern "C" void vReadAxisTask(void *pvParameters) {
    ParamsStruct* params = (ParamsStruct*) pvParameters;

    // System Calibration Data
    //uint8_t system_cal = 0;
    //uint8_t gyro_cal = 0;
    //uint8_t accel_cal = 0;
    //uint8_t mag_cal = 0;

    sensors_event_t event[4];

    //Adafruit_BNO055 imuRotunda (0,IMU_ADDRESS_ROTUNDA);
    Adafruit_BNO055 imuShoulder(0,IMU_ADDRESS_SHOULDER);
    //Adafruit_BNO055 imuElbow   (2,IMU_ADDRESS_ELBOW);
    //Adafruit_BNO055 imuWrist   (1,IMU_ADDRESS_WRIST);

    //Scan I2C Bus for Device with specified Slave Address and then initialize device to IMU
    while( !i2cScanAndInit(IMU_ADDRESS_SHOULDER) );

    //initIMU(IMU_ADDRESS_ROTUNDA , Adafruit_BNO055::OPERATION_MODE_IMUPLUS);
    //initIMU(IMU_ADDRESS_SHOULDER, Adafruit_BNO055::OPERATION_MODE_IMUPLUS);
    //initIMU(IMU_ADDRESS_ELBOW   , Adafruit_BNO055::OPERATION_MODE_IMUPLUS);
    //initIMU(IMU_ADDRESS_WRIST   , Adafruit_BNO055::OPERATION_MODE_IMUPLUS);

    while(1){
	// Read Euler Angles (Relative Position) from IMUs
        imuShoulder.getEvent(&event[0]);
        //imuWrist.getEvent(&event[1]);

	for ( int i = 0; i < 1; i++ )
	{
        	params->yaw[i]   = event[i].orientation.x;
		params->roll[i]  = event[i].orientation.y;
		params->pitch[i] = event[i].orientation.z;
	}

	// Get Calibration Info for ACCEL, GYRO, and MAG
	//imuShoulder.getCalibration(&system_cal, &gyro_cal, &accel_cal, &mag_cal);
	//printf("Calibration Data: SYS: %i\tGYR: %i\tACC: %i\tMAG: %i\n",system_cal,gyro_cal,accel_cal,mag_cal);

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

extern "C" void vArmTask(void *pvParameters)
{
    ParamsStruct *params = (ParamsStruct*) pvParameters;
    imu::Vector<3> shoulder_imu;
    imu::Vector<3> wrist_imu;

    while(1)
    {
	shoulder_imu = scanMPU6050(MPU6050_ADDR0);
	wrist_imu    = scanMPU6050(MPU6050_ADDR1);

	//Calculate Shoulder Pitch and Roll
	params->pitch[0] = calculatePitch( shoulder_imu.x(), shoulder_imu.y(), shoulder_imu.z() );
	params->roll[0]  = calculateRoll ( shoulder_imu.x(), shoulder_imu.y(), shoulder_imu.z() );

	//Calculate Wrist Pitch and Roll
	params->pitch[1] = calculatePitch( wrist_imu.x(), wrist_imu.y(), wrist_imu.z() );
	params->roll[1]  = calculateRoll ( wrist_imu.x(), wrist_imu.y(), wrist_imu.z() );

	printf("Shoulder...PITCH: %i\tROLL: %i\n",params->pitch[0], params->roll[0] );
	printf("Wrist......PITCH: %i\tROLL: %i\n\n",params->pitch[1], params->roll[1] );
	//scanMPU6050();
	vTaskDelay(100);
    }
    
}

extern "C" void vMPU6050Task(void *pvParameters)
{
    ParamsStruct *params = (ParamsStruct*) pvParameters;
    imu::Vector<3> accel;
    imu::Vector<3> gyro;

    while(1)
    {
	accel = scanAccel(MPU6050_ADDR0);
	gyro  = scanGyro(MPU6050_ADDR0);

        printf("ACCEL...X: %01f\tY: %01f\tZ: %01f\n", accel.x(), accel.y(), accel.z());
	printf("GYRO....X: %01f\tY: %01f\tZ: %01f\n", gyro.x() , gyro.y() , gyro.z());
	vTaskDelay(100);
    }
}
