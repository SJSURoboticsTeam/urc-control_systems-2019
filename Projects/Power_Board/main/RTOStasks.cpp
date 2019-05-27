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

extern "C" void vPowerBoardTask(void *pvParameters)
{
    ParamsStruct *params = (ParamsStruct*) pvParameters;
    params->ch1_isns = 0;
    params->ch2_isns = 0,
    params->ch3_isns = 0,
    params->ch4_isns = 0;

    //Initialize GPIO Pins
    pinMode(BAT1_SHDN_U11, OUTPUT);
    pinMode(BAT2_SHDN_U12, OUTPUT);
    pinMode(BAT4_SHDN_U15, OUTPUT);
    pinMode(BAT5_SHDN_U9 , OUTPUT);

    pinMode(SHDN_U18, OUTPUT);
    pinMode(SHDN_U16, OUTPUT);
    pinMode(SHDN_U10, OUTPUT);
    pinMode(SHDN_U6 , OUTPUT);
    pinMode(SHDN_U7 , OUTPUT);

    //Initialize ADC Pins
    static const adc1_channel_t CH1_ISNS_U5  = ADC1_CHANNEL_0; //GPIO 36
    static const adc1_channel_t CH2_ISNS_U3  = ADC1_CHANNEL_3; //GPIO 39
    static const adc1_channel_t CH3_ISNS_U8  = ADC1_CHANNEL_6; //GPIO 34
    static const adc1_channel_t CH4_ISNS_U13 = ADC1_CHANNEL_7; //GPIO 35
    //Currently Configured Pins for BAT3 and VBUS are not ADC so idk

    adc1_config_channel_atten(CH1_ISNS_U5 , ADC_ATTEN_DB_11);
    adc1_config_channel_atten(CH2_ISNS_U3 , ADC_ATTEN_DB_11);
    adc1_config_channel_atten(CH3_ISNS_U8 , ADC_ATTEN_DB_11);
    adc1_config_channel_atten(CH4_ISNS_U13, ADC_ATTEN_DB_11);
    while(1)
    {
        i2c_scanner();
	params->ch1_isns = adc1_get_raw(CH1_ISNS_U5);
	params->ch2_isns = adc1_get_raw(CH2_ISNS_U3);
	params->ch3_isns = adc1_get_raw(CH3_ISNS_U8);
	params->ch4_isns = adc1_get_raw(CH4_ISNS_U13);
	vTaskDelay(50);
    }
}

