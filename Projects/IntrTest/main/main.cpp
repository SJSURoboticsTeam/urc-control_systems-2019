#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "Arduino.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "RTOStasks.h"
#include "Source.h"
#include "constants.h"
#include "esp_intr_alloc.h"

SemaphoreHandle_t xButtonInterruptSemaphore;
int x = 0;
extern "C" void app_main() {
    printf("entered main\n");
    Serial.begin(115200);
    initArduino();
    printf("arduino initialized\n");


    xButtonInterruptSemaphore = xSemaphoreCreateBinary();
    printf("semaphore created\n");
    
    //Init GPIO pin and interrupts
    gpio_install_isr_service(ESP_INTR_FLAG_EDGE);
    gpio_pad_select_gpio(GPIO_NUM_27);
    gpio_set_direction(GPIO_NUM_27, static_cast<gpio_mode_t>(GPIO_MODE_INPUT));
    gpio_set_pull_mode(GPIO_NUM_27, GPIO_PULLUP_ONLY);
    gpio_set_intr_type(GPIO_NUM_27, GPIO_INTR_POSEDGE);
    gpio_intr_enable(GPIO_NUM_27);
    gpio_isr_handler_add(GPIO_NUM_27, ButtonISR, (void *)x);
    printf("gpio pin set up\n");

    xTaskCreate(vInterruptTask, "Intr", 4096, NULL, 1, NULL);
}

