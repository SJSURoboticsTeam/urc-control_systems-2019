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
#include "Adafruit_BNO055.h"

extern "C" void vReadAxisTask(void *pvParameters) {
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
        //i2c_scanner();
	for(int i = 0; i < 1; i++)
	{
		printf("%i) YAW: %.2f\tPITCH: %.2f\tROLL: %.2f\n",i,params->yaw[i],params->pitch[i],params->roll[i]);
	}
	vTaskDelay(100);
    }
}
