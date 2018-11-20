#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "Arduino.h"
#include "constants.h"
#include "PODS.h"
#include "Servo_Control.hpp"



extern "C" void vGygerTask(void *pvParameters)
{
    GygerCounter gyger;

    int x = *((int*)pvParameters); // PODS identifier number

	uint16_t sample_time = 500;// sample time in milliseconds
	uint32_t total_sample_time;
	int data = 0; // number of data points taken
	
	volatile int *eCount = NULL;

	//determin which interupt counter to read from
	switch(x)
	{
		case 0: eCount = &eCount0;
			break;
		case 1: eCount = &eCount1;
			break;
		case 2: eCount = &eCount2;
			break;
		case 3: eCount = &eCount3;
			break;
		case 4: eCount = &eCount4;
			break;
		case 5: eCount = &eCount5;
			break;
		case 6: eCount = &eCount6;
			break;
		default: 
			break;
	}


	vTaskSuspend(NULL);//suspend untill mission ccontrol says otherwise

    sealPODS(x);
	vTaskDelay(5000/portTICK_PERIOD_MS);
    
    while(1)
    {
    	uint16_t cutoff = 5;

   		if(data > 10)
   		{
			dispenseFluid(x);
		}
    	

   		while(gyger.count < cutoff)
   		{
    		

    		uint32_t time = 0;
    		while(millis() - time < sample_time)
    		{
    			gyger.count += *eCount;
    			*eCount = 0;
    			time = millis();
			}

			total_sample_time += sample_time;
    		sample_time = sample_time + 500;
   		}
    	gyger.cps = float(gyger.count) / total_sample_time * 1000;//radiation in emissions/sec
    	gyger.cpm = gyger.cps * 60; // emissions/minute

   		gyger.count = 0;
   		sample_time = 500;
   		total_sample_time = 0;


   		std::cout << "cps: " << gyger.cps << "\n";
   		std::cout << "cpm: " << gyger.cpm <<  "\n";
    	//wifi.send(gyger1.rads);
    	//wifi.send(gyger2.rads);
    	//wifi.send(gyger1.rads - gyger2.rads);
    		
	}
}


extern "C" void vHandleData(void *pvParameters)
{


}

extern "C" void vTest(void *pvParameters)
{
	
uint32_t servo1_frequency = 50; //hz
uint32_t servo1_gpio_pin =7;
uint32_t servo1_timer = 0;
uint32_t servo1_channel = 0;
float servo1_min = 5;
float servo1_max = 10;

uint32_t motor1_frequency = 476;//hz
uint32_t motor1_gpio_pin = 6;
uint32_t motor1_timer = 0;
uint32_t motor1_channel = 1;
float motor1_min = 42.8;//percent
float motor1_max = 100;//percent
float motor1_dead_min = 0.47;//percent
float motor1_dead_max = 4.3;//percent


		//servo object for door
	Servo servo1(servo1_gpio_pin,servo1_channel,servo1_timer, 
		servo1_frequency, servo1_max, servo1_min);

	//servo motor object for drill
	ServoMotor servoMotor(motor1_gpio_pin,motor1_timer,motor1_channel, 
		motor1_frequency, motor1_max, motor1_dead_min, motor1_dead_min, motor1_dead_max);

		double percentage = 50;
		int angle = 0;
	while(1)
	{
		

		if(angle > 80)
		{
			angle = -85;
		}
		else
		{
			angle = 85;
		}
		


		servo1.SetPositionPercent(angle);
		//servoMotor.SetSpeedAndDirection( percentage, true);
		printf("Boom \n" );
		vTaskDelay(1000);

	}

	

}
