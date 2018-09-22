#include <stdio.h>
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "Arduino.h"
#include "EEPROM.h"

#define EEPROM_SIZE 0x64
extern "C" void app_main()
{
/*    initArduino();
    pinMode(4, OUTPUT);
    digitalWrite(4, HIGH);

    EEPROM.begin(0x64);
*/
    if (!EEPROM.begin(EEPROM_SIZE))
    {
        printf("failed to initialize EEPROM\n");
    	vTaskDelay(1000);
    }
    
    printf("Hello, world1\n");
    uint16_t reboots = EEPROM.read(0);
    printf("This esp32 has rebooted %d times.\n", reboots);
    reboots++;
    EEPROM.write(0, reboots);
    EEPROM.commit();
    vTaskDelay(500 / portTICK_PERIOD_MS);
    
    for (int i = 10; i>=0; i--)
    {
	    
        printf("Restarting in %d seconds...\n", i);
	vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    printf("Restarting now.\n");
    fflush(stdout);
    esp_restart();
}
