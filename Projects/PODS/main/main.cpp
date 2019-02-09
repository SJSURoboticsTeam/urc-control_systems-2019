#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <string>
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "Arduino.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "RTOStasks.h"
#include "PODS.h"
#include "constants.h"

QueueHandle_t xQueueISR;
ParamsStruct params;
AsyncWebServer server(80);

extern "C" void app_main()
{
    Serial.begin(115200);
    initArduino();
    initServer(&server, &params);

    xQueueISR = xQueueCreate(10, sizeof(int));// queue to pass interupt id
    
   // initInteruptPins();	

 
	
}
