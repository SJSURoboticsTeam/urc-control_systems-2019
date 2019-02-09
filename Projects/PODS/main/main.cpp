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

QueueHandle_t xQueue, xTaskQueue;
ParamsStruct params;
AsyncWebServer server(80);

extern "C" void app_main()
{
    Serial.begin(115200);
    initArduino();
    initServer(&server, &params);

    xQueue = xQueueCreate(10, sizeof(int));// queue to pass interupt id
    xTaskQueue = xQueueCreate(2, sizeof(int));
    //xGygerSemaphore0 = xSemaphoreCreateBinary();
  

   /// printf("semaphore created\n");
    
    initInteruptPins();	

	xTaskCreate(vGygerTask, "gyger1 data", 4060, (void*)0, 1, &xGyger0); 
	vTaskDelay(50/portTICK_PERIOD_MS);
	
	xTaskCreate(vGygerTask, "gyger1 data", 4060, (void*)1, 1, &xGyger1); 
	vTaskDelay(50/portTICK_PERIOD_MS);	
	
	xTaskCreate(vGygerTask, "gyger2 data", 4060, (void*)2, 1, &xGyger2); 
	vTaskDelay(50/portTICK_PERIOD_MS);

	xTaskCreate(vGygerTask, "gyger3 data", 4060, (void*)3, 1, &xGyger3); 
	vTaskDelay(50/portTICK_PERIOD_MS);

	xTaskCreate(vGygerTask, "gyger4 data", 4060, (void*)4, 1, &xGyger4); 
	vTaskDelay(50/portTICK_PERIOD_MS);
	
	xTaskCreate(vGygerTask, "gyger5 data", 4060, (void*)5, 1, &xGyger5); 
	vTaskDelay(50/portTICK_PERIOD_MS);
	
	xTaskCreate(vGygerTask, "gyger6 data", 4060, (void*)6, 1, &xGyger6); 
	vTaskDelay(50/portTICK_PERIOD_MS);

	xTaskCreate(vToggleTask, "toggle tasks", 4060, NULL, 2, NULL); 
	vTaskDelay(50/portTICK_PERIOD_MS);

	


		
  	// xTaskCreate(vTest, "test servo and motor", 4060, NULL, 1, NULL);
	//xTaskCreate(vGygerTask, "gyger data", 4060, NULL, 1, NULL); 
	
}
