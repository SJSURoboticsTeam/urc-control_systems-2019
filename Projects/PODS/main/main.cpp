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

QueueHandle_t xQueueISR, xQueueTerminateTask;//, xQueueLid;
TaskHandle_t task0, task1, task2, task3, task4, task5, task6;
ParamsStruct params;
AsyncWebServer server(80);

extern "C" void app_main()
{
    Serial.begin(115200);
    initArduino();
    initServer(&server, &params);
    for(int x = 0; x <= 6; x++)
    {
    	resetString(x);
    }
    xQueueISR = xQueueCreate(10, sizeof(int));// queue to pass interupt id
    xQueueTerminateTask = xQueueCreate(1, sizeof(int));
   // xQueueLid = xQueueCreate(2, sizeof(int));
   // sealPODS(0, false);
    //xTaskCreate(vGygerTask, "gyger0 data", 4060, (void*)0, 1, NULL);
    //xTaskCreate(vLidTask, "toggle lid" , 4060, NULL, 2, NULL);
 
	
}
