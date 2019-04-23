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
    Adafruit_BNO055 imuRotunda (0,IMU_ADDRESS_ROTUNDA);
    Adafruit_BNO055 imuShoulder(1,IMU_ADDRESS_SHOULDER);
    Adafruit_BNO055 imuElbow   (2,IMU_ADDRESS_ELBOW);
    Adafruit_BNO055 imuWrist   (3,IMU_ADDRESS_WRIST);
    i2c_scanner();
    i2c_scanner();
    initIMU(IMU_ADDRESS_ROTUNDA , Adafruit_BNO055::OPERATION_MODE_IMUPLUS);
    initIMU(IMU_ADDRESS_SHOULDER, Adafruit_BNO055::OPERATION_MODE_IMUPLUS);
    initIMU(IMU_ADDRESS_ELBOW   , Adafruit_BNO055::OPERATION_MODE_IMUPLUS);
    initIMU(IMU_ADDRESS_WRIST   , Adafruit_BNO055::OPERATION_MODE_IMUPLUS);

    while(1){
	// Euler Angles (Relative Position)
        imuShoulder.getEvent(&event[1]);
        params->yaw[1]   = event[1].orientation.x;
	params->roll[1]  = event[1].orientation.y;
	params->pitch[1] = event[1].orientation.z;

  	printf("YAW: %.2f\tROLL: %.2f\tPITCH: %.2f\n",params->yaw[1],params->roll[1],params->pitch[1]);

	// Get Calibration Info for ACCEL, GYRO, and MAG
	imuShoulder.getCalibration(&system_cal, &gyro_cal, &accel_cal, &mag_cal);
	printf("Calibration Data: SYS: %i\tGYR: %i\tACC: %i\tMAG: %i\n",system_cal,gyro_cal,accel_cal,mag_cal);

        vTaskDelay(100);
    }
}

extern "C" void vI2CScannerTask(void *pvParameters)
{
    ParamsStruct *params = (ParamsStruct*) pvParameters;
    while(1)
    {
        i2c_scanner();
	//printf("NewYaw: %i\nNewPitch: %i\nNewRoll: %i\n\n",params->yaw[0],params->pitch[0],params->roll[0]);
	vTaskDelay(100);
    }
}
