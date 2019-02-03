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
#include "freertos/semphr.h"

ParamsStruct params;

AsyncWebServer server(80);

extern "C" void app_main()
{
    Serial.begin(115200);
    initArduino();
    initServer(&server, &params);

    initInteruptPins();	



	xTaskCreate(vGygerTask, "gyger1 data", 4060, (void*)1, 2, &xGyger1); 
	vTaskDelay(50/portTICK_PERIOD_MS);
	
	/*
	xTaskCreate(vGygerTask, "gyger2 data", 4060, (void*)2, 3, &xGyger2); 
	vTaskDelay(50/portTICK_PERIOD_MS);

	xTaskCreate(vGygerTask, "gyger3 data", 4060, (void*)3, 4, &xGyger3); 
	vTaskDelay(50/portTICK_PERIOD_MS);

	xTaskCreate(vGygerTask, "gyger4 data", 4060, (void*)4, 5, &xGyger4); 
	vTaskDelay(50/portTICK_PERIOD_MS);
	
	xTaskCreate(vGygerTask, "gyger5 data", 4060, (void*)5, 6, &xGyger5); 
	vTaskDelay(50/portTICK_PERIOD_MS);
	
	xTaskCreate(vGygerTask, "gyger6 data", 4060, (void*)6, 7, &xGyger6); 
	vTaskDelay(50/portTICK_PERIOD_MS);

	*/


		
  	// xTaskCreate(vTest, "test servo and motor", 4060, NULL, 1, NULL);
	//xTaskCreate(vGygerTask, "gyger data", 4060, NULL, 1, NULL); 
	
}
