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
#include "freertos/task.h"
#include "RTOStasks.h"
#include <string.h>



AsyncEventSource events("/events");

extern "C" void vGygerTask(void *pvParameters)
{
	
	int id = (int)pvParameters;
	u_long timer = 0;
	u_long task_start = millis();
	int count = 0; 
	int current_cpm = 0 ;
	int *ISR_queue_ID, *terminate;
	bool start = false;
	int *task_data;
	int rst = 100;
	String data;
	int sample_time = 5000; // sample time in (ms)
							//reduce to 1sec for final version
/********************
	intit gpio pin
********************/
	gpio_install_isr_service(ESP_INTR_FLAG_EDGE);
   	gpio_pad_select_gpio((gpio_num_t)gygerPin(id));
   	gpio_set_direction((gpio_num_t)gygerPin(id), static_cast<gpio_mode_t>(GPIO_MODE_INPUT));
   	gpio_set_pull_mode((gpio_num_t)gygerPin(id), GPIO_PULLDOWN_ONLY);
   	gpio_set_intr_type((gpio_num_t)gygerPin(id), GPIO_INTR_POSEDGE);
   	gpio_intr_enable((gpio_num_t)gygerPin(id));
   	gpio_isr_handler_add((gpio_num_t)gygerPin(id), emissionCount, (void *)gygerPin(id));
   	printf("gpio pin %d set up \n", gygerPin(id));

   	printf("gyger %d interrupt pin initialized\n", id );

	printf("gyger task %d initialized \n", id );

	printf("Starting gyger task %d \n", id );

	printf("Sealling POD %d \n", id );
	sealPODS(id, false);

	vTaskDelay(5000/portTICK_PERIOD_MS);
	printf("PODS %d sealed \n", id );


	dispenseFluid(id);
	printf("dispensing fluid \n");

	while(1)
	{
      	
 
      	if(xQueuePeek(xQueueISR, &ISR_queue_ID, (TickType_t) 0 ) //waits to see its ID in ISR queue
      			and (int)ISR_queue_ID == id)
      	{
      		xQueueReceive(xQueueISR, &ISR_queue_ID, (TickType_t) 0);
      		//printf("Task %d \n", id );
      		printf("Item recieved from queue. ID %d retreived. \n", (int)ISR_queue_ID );

      		count++;

      		//printf("count %d \n",  count);
			if(millis() - timer > sample_time)// calculates cpm once every samlpe_time or greater
			{
				printf("Task %d \n", id);
				current_cpm = (float)count * 1000.0 / (float)(millis() - timer) * 60;
				printf("interupt trigered %d times \n",count);
			
				data = writeData(true, id, current_cpm, millis() - task_start); //write to global variable
				printf("recorded cpm of %d  \n \n", current_cpm);
			
				//data = writeData(false, id, -1);//read from global variable
				//printf("cpm: %d  \n \n", current_cpm);

				count = 0;//reset counter
				timer = millis();
				//itoa(data, sse, 10);
				//events.addHeader("podStatus", "ESP Async Web Server");

				
			}

    		if(xQueuePeek(xQueueISR, &ISR_queue_ID, (TickType_t) 0 ))//check if queue is empty
  			{
  				//print ID if queue is not empty ##debuging##
  				//printf("Peeked into queue. ID %d found \n \n", (int)ISR_queue_ID);	
  				ISR_queue_ID = &rst; //set local isr_id variable to -1
      		}
      		else
      		{
      			//printf("Peeked into queue. Queue empty \n \n" );
      			ISR_queue_ID = &rst;//set local isr_id varialbe to -1
      		}

      	}   

      	//terminate if 
      	if(xQueuePeek(xQueueTerminateTask, &terminate, (TickType_t)0) and (int)terminate == id) 
      	{
      		xQueueReceive(xQueueTerminateTask, &terminate, (TickType_t) 0);
      		vTaskDelete(NULL);
      	}       	      		

	}


}


extern "C" void vLidTask(void *pvParameters)
{
	printf("lid task started\n");
	int m = (int) pvParameters;
	//printf("m = %d \n", m );
	int id = m % 100;	

	//printf("id = %d\n", id);
	int state_num = m / 100;
	//printf("state num = %d\n", state_num);
	bool state = NULL;

	if(state_num == 1)
	{
		state = true;
		//printf("state:: true\n");
	}
	else if(state_num == 0)
	{
		state = false;
		//printf("state false\n");
	}

	 
	sealPODS(id, state);
	vTaskDelay(3000/ portTICK_PERIOD_MS);
		while(1)
		{
			//printf("lid task terminated\n");
			vTaskDelete(NULL);
		}
	
}
