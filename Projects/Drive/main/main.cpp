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

extern "C" void app_main() {

    // Initialize peripheralsand services
    Serial.begin(115200);
    initArduino();
    initServer(&server, &params);
    initComponents();
    //delay(100);

    //Create freeRTOS tasks.
    xTaskCreate(vMoveTask, "Move", 4096, (void *) &params, 1, NULL);
    xTaskCreate(vLiDarTask, "Lidar", 4096, (void *) &params, 1, NULL);

}
