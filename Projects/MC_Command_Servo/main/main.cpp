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
#include "../../../Utilities/Servo_Control.hpp"

ParamsStruct params;
//Server used to listen for XHRs, and send SSEs.
AsyncWebServer server(80);

using namespace std;

extern "C" void app_main() 
{
    params.xWristSemaphore = xSemaphoreCreateBinary();
    
    Serial.begin(115200);
    initArduino();
    initServer(&server, &params);


    //Create freeRTOS tasks.
    // xTaskCreate(vSayHelloTask, "Hello", 4096, &params, 1, NULL);
    xTaskCreate(vElbowTask,   "Elbow",   4096, &params, 1, NULL);
    xTaskCreate(vRotundaTask, "Rotunda", 4096, &params, 1, NULL);
    xTaskCreate(vShoulderTask, "Shoulder", 4096, &params, 1, NULL);
    xTaskCreate(vDiffGearboxTask, "Wrist", 4096, &params, 1, NULL);
    xTaskCreate(vClawTask, "Claw", 4096, &params, 1, NULL);
}

