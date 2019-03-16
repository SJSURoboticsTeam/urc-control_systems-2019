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
//    ParamsStruct* params = (ParamsStruct*) pvParameters;
    uint8_t slave_address = 0;
    int16_t Yaw = 0;
    i2c_scanner();
    slave_address = i2c_scanner();
    initIMU(slave_address);

    while(1) {
//        if((slave_address != BNO055_ADDR1) || (slave_address != BNO055_ADDR0))
//        {
//            slave_address = i2c_scanner();
//            initIMU(slave_address);
//        }

        Yaw = getAxis(slave_address, BNO055_EUL_YAW_LSB);
	printf("X Axis: %i\n",Yaw);
        vTaskDelay(100);
    }
}

/*
    This task demonstrates how to read and write from EEPROM,
    which is non-voltaile memory that we can use to store data on the 
    ESP.
*/
extern "C" void vI2CScannerTask(void *pvParameters)
{
    uint8_t slave_address;
    while(1) 
    {
        slave_address = i2c_scanner();
	vTaskDelay(100);
    }
}
