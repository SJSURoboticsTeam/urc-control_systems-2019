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
#include <WiFi.h>
#include <FS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

constexpr uint8_t kLedBuiltin = 2;
const char kSsid[] = "MyESP32AP";
const char kPassword[] = "testpassword";

AsyncWebServer server(80);

void vTaskOne( void * parameter );
void vTaskTwo( void * parameter);

SemaphoreHandle_t xSemaphore;

extern "C" void app_main()
{
    //protoModule Config
    initArduino();
    pinMode(4, OUTPUT);
    digitalWrite(4, HIGH);

    if (!initEEPROM()) {
        for (int i = 10; i >= 0; i--) {
	       printf("Restarting in %d seconds...\n", i);
	       usleep(1000);
        }
    	printf("Restarting now.\n");
    	fflush(stdout);
    	esp_restart();
    }

    //Wifi Server test Code
    Serial.begin(115200);

    pinMode(kLedBuiltin, OUTPUT);

    if(!WiFi.softAPConfig(IPAddress(192, 168, 5, 1), IPAddress(192, 168, 5, 1), IPAddress(255, 255, 255, 0))){
      Serial.println("AP Config Failed");
    }  

    WiFi.softAP(kSsid, kPassword);
    Serial.println();
    Serial.print("IP address: ");
    Serial.println(WiFi.softAPIP());

     
    
    xSemaphore = xSemaphoreCreateMutex();

    xTaskCreate(
                vTaskOne,     /* Task function. */
                "TaskOne",   /* String with name of task. */
                10000,        /* Stack size in words. */
                NULL,        /* Parameter passed as input of the task */
                1,           /* Priority of the task. */
                NULL);       /* Task handle. */

    xTaskCreate(
                vTaskTwo,     /* Task function. */
                "TaskTwo",   /* String with name of task. */
                10000,        /* Stack size in words. */
                NULL,        /* Parameter passed as input of the task */
                1,           /* Priority of the task. */
                NULL);       /* Task handle. */

    server.on("/hello", HTTP_GET, [](AsyncWebServerRequest *request){
        digitalWrite(kLedBuiltin, !digitalRead(kLedBuiltin));
        request->send(200, "text/plain", "Hello World");
    });
    server.begin();
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
