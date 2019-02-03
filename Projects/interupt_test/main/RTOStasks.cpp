#include <stdlib.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/event_groups.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include "Arduino.h"
#include "EEPROM.h"
#include "Source.h"
#include "constants.h"
#include <string>

extern "C" void vSayHelloTask(void *pvParameters){
    ParamsStruct* params = (ParamsStruct*) pvParameters;

    while(1){
        printf("Hello, %s \n", params -> name);
            vTaskDelay(500);
    }
}


extern "C" void vCountTask(void *pcParameters)
{
    int count = 0;
    EEPROM.put(BEGINING_ADDR, count);
    EEPROM.commit();

    while(1){
        count = EEPROMCount(BEGINING_ADDR);
        printf("I have said hello  %d times!\n\n\n", count );
        vTaskDelay(500);
    }
}


extern "C" void vInterruptTask(void *pvParameters)
{
    printf("interrupt task created" );
    int button_counter = 0;

    while(1)
    {
        printf("interrupt task \n");
        if(xSemaphoreTake(xButtonInterruptSemaphore, portMAX_DELAY))
        {
            button_counter++;
            printf("INterrupt has been triggered %d times. \n", button_counter);
        }
        vTaskDelay(10);

    }
}