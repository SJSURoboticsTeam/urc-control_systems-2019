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

    //xTaskCreate(vIMUTask, "IMU Task", 4096, NULL, 1, NULL);
    xTaskCreate(vServoTask, "Servo Task", 4096, NULL, 1, NULL);
    xTaskCreate(vUpdateTarget, "Update Target", 4096, NULL, 1, NULL);

}
