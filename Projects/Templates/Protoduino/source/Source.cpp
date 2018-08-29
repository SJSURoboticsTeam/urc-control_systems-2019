#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Source.h"
extern "C" {
#ifndef DEBUG
   #include "Wifi.h"
   #endif
#include "Structure.h"
}


/*-------------------------Put your code here---------------*/

extern "C" void Protolobe_Init()
{
   //change access point name for ap mode
   set_wifi_ap("O_O",3);
   //static ip test, view resul in monitor mode
   set_static_ip("192.168.1.14");
   //initialize Wifi
   create_wifi_task();
   //Initializing struct to 0
   memset(&send_data, 0, sizeof(send_data));
   memset(&read_data, 0, sizeof(read_data));
   //Setting element in send data
   send_data.data[send_data_name_1]=-100; 
   send_data.data[send_data_name_2]=2000000;
   // vTaskDelay(500);
   //Sending data in send_data to mission control
   send_data_wifi(SEND_ITEM_SIZE);
}

double convert(double current, uint16_t time)
{
   if(current < 0)
   {
      return -1;
   }
   return (current / 3600000 ) * time;
}

/*------------------------Code end here-------------------*/
