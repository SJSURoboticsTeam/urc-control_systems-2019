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
    bool fluid_dump = false; //has fluid beeen poured yet
    int id = (int)pvParameters; // PODS identifier number
    int x = id;

	uint16_t sample_time = 1000;// sample time in milliseconds
	uint32_t total_sample_time = 0;
	int data = 0; // number of data points taken
	volatile int *eCount = NULL;
    uint16_t cutoff = 10;
	unsigned long time = 0;// previous time for timer
	uint count =0;
	int* cpm = NULL;// emmoission counts per minute
	//determin which interupt counter to read from

	std::cout << "starting gyger task \n";

	switch(x)
	{
		case 0: eCount = &eCount0;
				cpm = &cpm0;
				std::cout << "counter #: " << id << "\n";
			break;
		case 1: eCount = &eCount1;
				cpm = &cpm0;
				std::cout << "counter #: " << id << "\n";
			break;
		case 2: eCount = &eCount2;
				cpm = &cpm2;
				std::cout << "counter #: " << id << "\n";
			break;
		case 3: eCount = &eCount3;
				cpm = &cpm3;
				std::cout << "counter #: " << id << "\n";
			break;
		case 4: eCount = &eCount4;
				cpm = &cpm4;
				std::cout << "counter #: " << id << "\n";
			break;
		case 5: eCount = &eCount5;
				cpm = &cpm5;
				std::cout << "counter #: " << id << "\n";
			break;
		case 6: eCount = &eCount6;
				cpm = &cpm6;
				std::cout << "counter #: " << id << "\n";
			break;
		default: 
			break;
	}

	*cpm = -1;
	
	std::cout <<"setup of gyger " << id << " complete \n";
	//std::cout <<"suspending gyger task: " << id << " \n";

	//vTaskSuspend(NULL);//suspend until mission ccontrol says otherwise

   // sealPODS(*id);
	//vTaskDelay(5000/portTICK_PERIOD_MS);
    
    while(1)
    {

    	//*eCount += rand() % 10;

   		if(data > 10 and !fluid_dump)
   		{
			dispenseFluid(id);
			fluid_dump = true;
		}    	

   		if(count <= cutoff and millis() - time > sample_time)
   		{
    		    count += *eCount;
    			std::cout << "new data \n" << eCount0 << "\n";	
    			//std::cout << "analog read \n" << analogRead(gyger0_pin) << "\n";	
    			time = millis();
    			total_sample_time += sample_time;
    			*eCount = 0;
			

   		}

   		else if (count > cutoff)
   		{
    	

    	*cpm = float(count) / total_sample_time;//radiation in emissions/minute
    	count = 0;

   		total_sample_time = 0;


   		std::cout << "cps: " << *cpm / 60 << "\n";
   		std::cout << "cpm: " << *cpm <<  "\n";

   		}

   		vTaskDelay(50/portTICK_PERIOD_MS);
    		
	}

}





/*
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
	//	vTaskDelay(1000);

	}

	

}
*/