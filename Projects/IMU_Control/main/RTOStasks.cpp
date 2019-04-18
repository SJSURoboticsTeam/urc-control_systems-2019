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
    uint8_t system = 0, gyro = 0, accel = 0, mag = 0;
    double roll = 0, pitch = 0;
    imu::Quaternion quat[2];
    imu::Vector<3> quat_to_eul[2];
    imu::Vector<3> accelerometer;

    //Adafruit_BNO055 bno0(0,BNO055_ADDRESS_A);
    Adafruit_BNO055 bno1(1,BNO055_ADDRESS_B);
    i2c_scanner();
    i2c_scanner();
    //initIMU(BNO055_ADDRESS_A,Adafruit_BNO055::OPERATION_MODE_NDOF);
    initIMU(BNO055_ADDRESS_B,Adafruit_BNO055::OPERATION_MODE_NDOF);

    while(1){
	// Get Euler Data (Relative Position)

  	sensors_event_t event[2];
  	//bno0.getEvent(&event[0]);
        bno1.getEvent(&event[1]);
  	//printf("YAW0: %.2f\tROLL0: %.2f\tPITCH0: %.2f\n",event[0].orientation.x,event[0].orientation.y,event[0].orientation.z);
	//printf("YAW1: %.2f\tROLL1: %.2f\tPITCH1: %.2f\n",event[1].orientation.x,event[1].orientation.y,event[1].orientation.z);

	// Get Accelerometer Data
	accelerometer = bno1.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
	roll = calculateRoll(accelerometer.x(),accelerometer.y(),accelerometer.z());
	pitch = calculatePitch(accelerometer.x(),accelerometer.y(),accelerometer.z());

	printf("ACCEL: X: %.4f\t Y: %.4f\t Z: %.4f\n",accelerometer.x(),accelerometer.y(),accelerometer.z());
	printf("PITCH: %.4f\t ROLL: %.4f\n", pitch,roll);
	// Get Calibration Info for ACCEL, GYRO, and MAG

	bno1.getCalibration(&system, &gyro, &accel, &mag);
	printf("Calibration Data: SYS: %i\tGYR: %i\tACC: %i\tMAG: %i\n",system,gyro,accel,mag);

	// Get Quaternion Data (Absolute Position)

	/*
	quat[0] = bno0.getQuat();
	quat[1] = bno1.getQuat();
	quat_to_eul[0] = quat[0].toEuler();
	quat_to_eul[1] = quat[1].toEuler();
	printf("Quaternion Data: W: %.6f\t X: %.6f\t Y: %.6f\t Z: %.6f\n",quat.w(),quat.x(), quat.y(), quat.z());
	printf("1.YAW: %.2f\t ROLL: %.2f\t PITCH: %.2f\n",quat_to_eul[0].x(),quat_to_eul[0].y(),quat_to_eul[0].z());
	printf("2.YAW: %.2f\t ROLL: %.2f\t PITCH: %.2f\n",quat_to_eul[1].x(),quat_to_eul[1].y(),quat_to_eul[1].z());
	*/

        vTaskDelay(100);
    }
}

extern "C" void vI2CScannerTask(void *pvParameters)
{
    ParamsStruct *params = (ParamsStruct*) pvParameters;
    while(1)
    {
        //slave_address = i2c_scanner();
	//printf("NewYaw: %i\nNewPitch: %i\nNewRoll: %i\n\n",params->yaw[0],params->pitch[0],params->roll[0]);
	vTaskDelay(100);
    }
}
