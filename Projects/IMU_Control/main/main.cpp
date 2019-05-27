#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "Arduino.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "RTOStasks.h"
#include "Source.h"
#include "constants.h"

ParamsStruct params;
//Server used to listen for XHRs, and send SSEs.
AsyncWebServer server(80);

extern "C" void app_main() 
{
    Serial.begin(115200);
    initArduino();
    initServer(&server, &params);

    //Create freeRTOS tasks.
    //xTaskCreate(vReadAxisTask, "AXIS", 4096, &params, 1, NULL);
    //xTaskCreate(vI2CScannerTask, "SCAN", 4096, &params, 1, NULL);
    xTaskCreate(vMPU6050Task   , "MPU" , 4094, &params, 1, NULL);
    //xTaskCreate(vArmTask       , "ARM" , 4094, &params, 1, NULL);
}

