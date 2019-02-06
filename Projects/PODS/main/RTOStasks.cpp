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
#include "freertos/semphr.h"


extern "C" void vGygerTask(void *pvParameters)
{

	uint timer = 0;
	int count = 0; 

	while(1)
	{

        if(xSemaphoreTake(xButtonInterruptSemaphore, portMAX_DELAY))
        {
        	count++;
        }

		if(millis() - timer > 2000)
		{

			printf("interupt trigered %d times \n",count);
			timer = millis();
		}

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