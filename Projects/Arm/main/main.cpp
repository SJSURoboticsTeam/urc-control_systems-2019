#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "Arduino.h"
#include "EEPROM.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "RTOStasks.h"
#include "Source.h"
#include "constants.h"

extern "C" void app_main()
{
    initArduino();
    pinMode(4, OUTPUT);
    digitalWrite(4, HIGH);

    if (!initEEPROM())
    {
        for (int i = 10; i >= 0; i--)
	{
	    printf("Restarting in %d seconds...\n", i);
	    usleep(1000);
	}
	printf("Restarting now.\n");
	fflush(stdout);
	esp_restart();
    }

    xTaskCreate(vSayHelloTask, "Hello", 4096, NULL, 1, NULL);
    xTaskCreate(vCountTask, "Count", 4096, NULL, 1, NULL);

}
