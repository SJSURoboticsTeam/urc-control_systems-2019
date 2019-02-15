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

// Parameters to be updated by mission control
ParamsStruct params;

// Server used to listen for XHRs, and send SSEs.
AsyncWebServer server(80);

// Create the Steering Servos
Servo servo_A;
Servo servo_B;
Servo servo_C; 

// Create the Drive Motors
Motor motor_A;
Motor motor_B;
Motor motor_C;

// Create task handlers
TaskHandle_t xDriveHandle;
TaskHandle_t xCrabHandle;
TaskHandle_t xSpinHandle;
TaskHandle_t xDebugHandle;

extern "C" void app_main() {
    
    Serial.begin(115200);
    initArduino();
    initServer(&server, &params);
    initComponents();
    //delay(100);
    
    //Create freeRTOS tasks.
    xTaskCreate(vDebugTask, "Debug", 4096, (void *) &params, 1, &xDebugHandle);
    //xTaskCreate(vCarTask, "Drive", 4096, (void *) &params, 1, &xDriveHandle);
    //xTaskCreate(vCrabTask, "Crab", 4096, (void *) &params, 1, &xCrabHandle);
    //xTaskCreate(vSpinTask, "Spin", 4096, (void *) &params, 1, &xSpinHandle);
    //xTaskCreate(vModeTaskHandler, "TaskHandler", 4096, (void *) &params, 2, NULL);

}