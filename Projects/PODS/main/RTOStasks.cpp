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


extern "C" void vGygerTask(void *pvParameters)
{
	int id = (int)pvParameters;
	u_long timer = 0;
	int count = 0; 
	int current_cpm = 0 ;
	int *ISR_queue_ID, *task_queue_id;
	bool start = false;
	int *task_data;
	int rst = 100;
	int data;
	int sample_time = 5000; // sample time in (ms)
							//reduce to 1sec for final version

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
	//printf("Suspending gyger task %d \n", id );
	//vTaskSuspend(NULL);

	printf("Starting gyger task %d \n", id );

/*
	printf("Sealling POD %d \n", id );
	sealPODS(id);

	vTaskDelay(5000/portTICK_PERIOD_MS);

	printf("PODS %d sealed \n", id );
*/
	while(1)
	{
      	
  

      	if(xQueuePeek(xQueueISR, &ISR_queue_ID, (TickType_t) 0 ) 
      			and (int)ISR_queue_ID == id)
      	{
      		//queueID = &rst;
      		xQueueReceive(xQueueISR, &ISR_queue_ID, (TickType_t) 0);
      		printf("Task %d \n", id );
      		printf("Item recieved from queue. ID %d retreived. \n", (int)ISR_queue_ID );

      		count++;

      		printf("count %d \n",  count);
      		//queueID = &rst;
			if(millis() - timer > sample_time)
			{
				printf("Task %d \n", id);
				current_cpm = (float)count * 1000.0 / (float)(millis() - timer) * 60;
				printf("interupt trigered %d times \n",count);
			
				data = writeData(true, id, current_cpm);
				printf("recorded cpm of %d  \n \n", current_cpm);
			
				data = writeData(false, id, -1);
				printf("cpm: %d  \n \n", data);

				count = 0;
				timer = millis();
			}

    		if(xQueuePeek(xQueueISR, &ISR_queue_ID, (TickType_t) 0 ))
  			{
  				printf("Peeked into queue. ID %d found \n \n", (int)ISR_queue_ID);	
      		}
      		else
      		{
      			printf("Peeked into queue. Queue empty \n \n" );
      			ISR_queue_ID = &rst;
      		}

      	}           	      		

	}


}
