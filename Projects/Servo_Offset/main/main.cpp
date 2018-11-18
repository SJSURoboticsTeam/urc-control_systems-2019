#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "Arduino.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "RTOStasks.h"

extern "C" void app_main()
{   

    initArduino();
    pinMode(4, OUTPUT);
    digitalWrite(4, HIGH);
    
    //xTaskCreate(vAccelerometerTask, "Read Accelerometer", 4096, NULL, 1, NULL);
    xTaskCreate(vServoTask, "Set Servo", 4096, NULL, 2, NULL);
	xTaskCreate(vUpdateTarget, "Mission_Control", 4096, NULL, 2, NULL);

}