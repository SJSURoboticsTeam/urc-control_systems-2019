#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "Arduino.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "RTOStasks.h"
#include "PODS.h"
#include "constants.h"

extern "C" void app_main()
{
    initArduino();
    pinMode(4, OUTPUT);
    digitalWrite(4, HIGH);

    initInteruptPins();	
	TaskHandle_t xGyger0, xGyger1, xGyger2, xGyger3, xGyger4, xGyger5, xGyger6;

	//xTaskCreate(vSealPODS, "seal pod", 4060, NULL, 1, NULL);

	xTaskCreate(vGygerTask, "gyger0 data", 4060, (void*)0, 1, &xGyger0); 
	xTaskCreate(vGygerTask, "gyger1 data", 4060, (void*)1, 2, &xGyger1); 
	xTaskCreate(vGygerTask, "gyger2 data", 4060, (void*)2, 3, &xGyger2); 
	xTaskCreate(vGygerTask, "gyger3 data", 4060, (void*)3, 4, &xGyger3); 
	xTaskCreate(vGygerTask, "gyger4 data", 4060, (void*)4, 5, &xGyger4); 
	xTaskCreate(vGygerTask, "gyger5 data", 4060, (void*)5, 6, &xGyger5); 
	xTaskCreate(vGygerTask, "gyger6 data", 4060, (void*)6, 7, &xGyger6); 

	xTaskCreate(vHandleData, "transmit/recieve gyger data", 4060, NULL, 8, NULL);

		
  	// xTaskCreate(vTest, "test servo and motor", 4060, NULL, 1, NULL);
	//xTaskCreate(vGygerTask, "gyger data", 4060, NULL, 1, NULL); 
	
}
