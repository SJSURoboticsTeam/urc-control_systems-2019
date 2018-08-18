#ifndef WIFI_H_
#define WIFI_H_

#ifdef _cplusplus
extern "C"{
#endif

#include <stdint.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/event_groups.h"

#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "esp_log.h"

#include "tcpip_adapter.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/api.h"
#include "lwip/tcp.h"
#include "lwip/priv/tcp_priv.h"
#include "lwip/priv/api_msg.h"
#include "lwip/err.h"
#include "Structure.h"

//extern variable//
extern char SYSTEM_DEBUG_MSG[];
/*****************/

static esp_err_t event_handler(void *ctx, system_event_t *event);
static void initialise_wifi(void);
bool extract_url_variable(const char * variable,const char * url,uint32_t url_length,char * dest,uint32_t dest_length);
bool add_sse_client(struct netconn * conn);
static void http_server_netconn_serve(void *pvParameters);
inline char int_to_ascii_hex(uint32_t n);
static void handle_sse();
static void http_server(void *pvParameters);
/**
*Initialize wifi
*/
void create_wifi_task(void);
void send_data_wifi(int item_size);
int read_data_wifi(int item_size);
int set_wifi_ap(char name[],int name_size);
void set_static_ip(char ip[]);

#ifdef _cplusplus
}
#endif

#endif /* Wifi */
