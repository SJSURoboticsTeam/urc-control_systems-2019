#include <stdlib.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "Arduino.h"
#include "EEPROM.h"
#include "Source.h"
#include "constants.h"

extern "C" void vSayHelloTask(void *pvParameters)
{
    while(1)
    {
        hello_world();
	vTaskDelay(500);
    }
}

extern "C" void vCountTask(void *pvParameters)
{
    int count = 0;
    EEPROM.put(BEGINING_ADDR, count);
    EEPROM.commit();

    while(1)
    {
        count = EEPROMCount(BEGINING_ADDR);
	printf("I have said hello %d times!\n\n\n", count);
	vTaskDelay(500);
    }
}
