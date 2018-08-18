#ifndef WIFI_H_
#define WIFI_H_
void create_wifi_task(void);
void send_data_wifi(int item_size);
int read_data_wifi(int item_size);
int set_wifi_ap(char name[],int name_size);
void set_static_ip(char ip[]);
#endif