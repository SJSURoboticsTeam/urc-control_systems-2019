#include <stdio.h>
#include "Arduino.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "Source.h"
#include "RTOS_Tasks.h"


extern "C" void app_main()
{
    initArduino();
    pinMode(4, OUTPUT);
    digitalWrite(4, HIGH);

    printf("Protolobe Testing Wifi \n");
    Protolobe_Init();
    xTaskCreate(vReadDataTask, "vReadDataTask", 4096, NULL, 1, NULL);

}
