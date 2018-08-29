#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/event_groups.h"
#include "esp_log.h" 
extern "C" {
#include "Wifi.h"
#include "Structure.h"
}



extern "C" void vReadDataTask(void *pvParameters){
   while(1){
     vTaskDelay(500);
     //read data from mission control. 
     read_data_wifi(READ_ITEM_SIZE);
     printf("read data 1 : %i\n", read_data.data[1]);
   }
}

