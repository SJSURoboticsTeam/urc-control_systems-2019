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
	int past_cpm = 0; 
	int current_cpm = 0 ;
	int *queueID;
	int rst = 100;
	int data;
	int sample_time = 10000; // sample time in (ms)


	printf("gyger task %d initialized \n", id );
	printf("Suspending gyger task %d \n", id );
	vTaskSuspend(NULL);

	printf("Starting gyger task %d \n", id );

/*
	printf("Sealling POD %d \n", id );
	sealPODS(id);

	vTaskDelay(5000/portTICK_PERIOD_MS);

	printf("PODS %d sealed \n", id );
*/
	while(1)
	{
      	
      	//xQueuePeek(xQueue, &queueID, (TickType_t) 0 );
      	//printf("Peeked into queue. ID %d found \n", (int)queueID);
      		
      	if(xQueuePeek(xQueue, &queueID, (TickType_t) 0 ) and (int)queueID == id)
      	{
      		//queueID = &rst;
      		xQueueReceive(xQueue, &queueID, (TickType_t) 0);
      		printf("Task %d \n", id );
      		printf("Item recieved from queue. ID %d retreived. \n", (int)queueID );

      		count++;

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

    		if(xQueuePeek(xQueue, &queueID, (TickType_t) 0 ))
  			{
  				printf("Peeked into queue. ID %d found \n \n", (int)queueID);	
      		}
      		else
      		{
      			printf("Peeked into queue. Queue empty \n \n" );
      			queueID = &rst;
      		}

      	}           	      		

	}


}


extern "C" void vToggleTask(void *pvParameters)
{
	

	int id, state;
	printf("started togggle task\n");
	



	while(1)
	{
		//printf("toggle check\n");
		
		if(xQueuePeek(xTaskQueue, &id, (TickType_t) 0))
		{
			printf("peeked into task Queue \n");
			xQueueReceive(xTaskQueue, &id, (TickType_t) 0);
			printf("id %d \n", id );
			vTaskDelay(10);
			xQueueReceive(xTaskQueue, &state, (TickType_t) 0);
			printf("state %d \n", state );
			if(state == -1)
			{
				switch (id)
				{
					case 0:	vTaskResume(xGyger0);
						break;
				}
			}

			if(state == -2)
			{
				switch(id)
				{
					case 0:  vTaskSuspend(xGyger0);
						break;
				}
			}
		}

		
		//printf("end to toggle task \n" );
		vTaskDelay(1000/portTICK_PERIOD_MS);

	}
}



