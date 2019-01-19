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

// Create the Steering Servos
Servo servo_A;
Servo servo_B;
Servo servo_C; 

// Create the Drive Motors
ServoMotor motor_A;
ServoMotor motor_B;
ServoMotor motor_C;

extern "C" void app_main() {
    
    Serial.begin(115200);
    initArduino();
    initServer(&server, &params);

    initComponents();

    //Create freeRTOS tasks.
    xTaskCreate(vDebugTask, "Debug", 4096, (void *) &params, 1, NULL);
}