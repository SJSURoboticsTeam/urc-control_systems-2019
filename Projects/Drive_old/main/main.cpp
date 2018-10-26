#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "DriveSystem.h"
#include "RTOS_Tasks.h"
#include "Servo.hpp"
#include "Motor.hpp"
using namespace std;

extern "C" void app_main()
{
    uint32_t duty;
    printf("DriveSystem Testing \n");
    DriveSystem_init();
    xTaskCreate(vDriveTask, "vDriveTask", 4096, NULL, 6, NULL);
    xTaskCreate(vReadTask,  "vReadTask" , 4096, NULL, 1, NULL);
    xTaskCreate(vWriteTask, "vWriteTask", 4096, NULL, 1, NULL);
    xTaskCreate(vBrakeTask, "vBrakeTask", 4096, NULL, 1, NULL);


}
