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
#include <string>
#include "driver/adc.h"

extern "C" void vFPITask(void *pvParameters)
{
    ParamsStruct *params = (ParamsStruct*) pvParameters;
    params->fpi_pin = 0;

    //Initialize ADC Pins
    static const adc1_channel_t FPI_PIN  = ADC1_CHANNEL_0; //GPIO 36
    adc1_config_channel_atten(FPI_PIN , ADC_ATTEN_DB_11);

    while(1)
    {
	params->fpi_pin = adc1_get_raw(FPI_PIN);
	vTaskDelay(50);
    }
}

