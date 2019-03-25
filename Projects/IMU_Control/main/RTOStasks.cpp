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


extern "C" void vReadAxisTask(void *pvParameters) {
    ParamsStruct* params = (ParamsStruct*) pvParameters;
    uint8_t slave_address = 0;
    int16_t yaw = 0, pitch = 0, roll = 0;
    i2c_scanner();
    slave_address = i2c_scanner();
    slave_address = BNO055_ADDR1;
    initIMU(slave_address);

    while(1) {
        yaw = getAxis(slave_address, BNO055_EUL_YAW_MSB);
	yaw = (yaw << 16) | getAxis(slave_address, BNO055_EUL_YAW_LSB);

        params->yaw[0] = convertEuler(yaw, YAW_MIN, YAW_MAX, 0, 360);

	pitch = getAxis(slave_address, BNO055_EUL_PITCH_MSB);
	pitch = (pitch << 16) | getAxis(slave_address, BNO055_EUL_PITCH_LSB);

	params->pitch[0] = convertEuler(pitch, PITCH_MIN, PITCH_MAX, -180, 180);

	roll = getAxis(slave_address, BNO055_EUL_ROLL_MSB);
	roll = (roll << 16) | getAxis(slave_address, BNO055_EUL_ROLL_LSB);

	params->roll[0] = convertEuler(roll, ROLL_MIN, ROLL_MAX, -90, 90);

	//printf("Yaw: %i\nPitch: %i\nRoll: %i\n\n",yaw,pitch,roll);
        vTaskDelay(50);
    }
}

extern "C" void vI2CScannerTask(void *pvParameters)
{
    ParamsStruct *params = (ParamsStruct*) pvParameters;
    uint8_t slave_address;
    while(1) 
    {
        //slave_address = i2c_scanner();
	printf("NewYaw: %i\nNewPitch: %i\nNewRoll: %i\n\n",params->yaw[0],params->pitch[0],params->roll[0]);
	vTaskDelay(100);
    }
}
