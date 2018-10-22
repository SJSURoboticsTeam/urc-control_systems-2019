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
#include "ArmDefines.h"
#include <WiFi.h>
#include <FS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>


constexpr uint8_t kLedBuiltin = 2;
const char kSsid[] = "MyESP32AP";
const char kPassword[] = "testpassword";

AsyncWebServer server(80);

void Arm_claw_task(void *pvParameters);

void Arm_elbow_task(void *pvParameters);

void Arm_cam_task(void *pvParameters);

void Arm_wrist_task(void *pvParameters);

void Arm_shoulder_task(void *pvParameters);

void Gimbal_task(void *pvParameters);

void Arm_rotunda_task(void *pvParameters);
 	
void gpio_init();

extern "C" void Wifi_init();

extern "C" void app_main()
{
   //Wifi Server test Code
    Wifi_init();

    //i2c_example_master_init();
    gpio_init();


    //xTaskCreate(error_task, "Error_handler", 2048, NULL, 2, NULL);
    xTaskCreate(vReadDataTask   , "vReadDataTask"      , 2048, NULL, 1, NULL);
    xTaskCreate(Gimbal_task, "Gimbal", 2048, NULL, 1, NULL);
    xTaskCreate(Arm_rotunda_task, "Arm_rotunda_task", 2048, NULL, 1, NULL);
    xTaskCreate(Arm_shoulder_task, "Arm_shoulder_task", 2048, NULL, 1, NULL);
    xTaskCreate(Arm_wrist_task, "Arm_wrist_task", 2048, NULL, 1, NULL);
    xTaskCreate(Arm_cam_task, "Arm_cam_task", 2048, NULL, 1, NULL);
    xTaskCreate(Arm_elbow_task, "Arm_elbow_task", 2048, NULL, 1, NULL);
    //    xTaskCreate(Arm_claw_task, "Arm_claw_task", 2048, NULL, 1, NULL);

    vTaskDelay(200 / portTICK_RATE_MS);
}

extern "C" void Wifi_init(){
    Serial.begin(115200);

    pinMode(kLedBuiltin, OUTPUT);

    if(!WiFi.softAPConfig(IPAddress(192, 168, 5, 1), IPAddress(192, 168, 5, 1), IPAddress(255, 255, 255, 0))){
      Serial.println("AP Config Failed");
    }  

    WiFi.softAP(kSsid, kPassword);
    Serial.println();
    Serial.print("IP address: ");
    Serial.println(WiFi.softAPIP());

    server.on("/hello", HTTP_GET, [](AsyncWebServerRequest *request){
        digitalWrite(kLedBuiltin, !digitalRead(kLedBuiltin));
        request->send(200, "text/plain", "Hello World");
    });
    
    server.begin();

//    //change access point name for ap mode
//    set_wifi_ap("Arm", 4);
//    //set static ip address
//    set_static_ip("192.168.0.5");
//    //initilize Wifi
//    create_wifi_task();
//    //Initializing struct to 0
//    memset(&send_data, 0, sizeof(send_data));
//    memset(&read_data, 0, sizeof(read_data));
//    //Read Data Initialization
//    read_data.data[read_shoulder] = 90;
//    //Setting element in send data
//    send_data.data[send_rotunda]=0; 
//    send_data.data[send_shoulder]=1023;
//    send_data.data[send_elbow]=500; 
//    send_data.data[send_wrist_pitch]=200; 
//    send_data.data[send_wrist_rotation]=800; 
//    send_data.data[send_cam]=10; 
//    send_data_wifi(SEND_ITEM_SIZE);
}

void vTaskOne( void * parameter )
{
    bool task1_flag = false;
    xSemaphoreTake(xSemaphore, portMAX_DELAY);
    server.on("/task1", HTTP_GET, [&task1_flag](AsyncWebServerRequest *request){
        digitalWrite(kLedBuiltin, !digitalRead(kLedBuiltin));
        task1_flag = digitalRead(kLedBuiltin);
        Serial.println("XHR - task 1");
        request->send(200, "text/plain", "Hello From Task 1.");
    });
    xSemaphoreGive(xSemaphore);

    while(true)
    {
        printf("Hello from task 1: flag = %d\n", task1_flag);
        delay(1000);
    }

    Serial.println("Ending task 1");
    vTaskDelete(NULL);
}

void vTaskTwo( void * parameter)
{
    bool task2_flag = false;

    xSemaphoreTake(xSemaphore, portMAX_DELAY);
    server.on("/task2", HTTP_GET, [&task2_flag](AsyncWebServerRequest *request){
        digitalWrite(kLedBuiltin, !digitalRead(kLedBuiltin));
        task2_flag = digitalRead(kLedBuiltin);
        Serial.println("XHR - task 2");
        request->send(200, "text/plain", "Hello From Task 2.");
    });
    xSemaphoreGive(xSemaphore);

    while(true)
    {
        printf("Hello from task 2: flag = %d\n", task2_flag);
        delay(1000);
    }

    Serial.println("Ending task 2");
    vTaskDelete(NULL);
}
