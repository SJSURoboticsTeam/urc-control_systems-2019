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
//AsyncWebServer server(80);

// Create the Steering Servos

extern "C" void app_main() {
    
    //Create freeRTOS tasks.
    xTaskCreate(vGPSTask, "GPS", 4096, (void *) &params, 1, NULL);

}