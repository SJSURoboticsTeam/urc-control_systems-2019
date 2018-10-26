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
#include "driver/gpio.h"

#include "tcpip_adapter.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/api.h"
#include "lwip/tcp.h"
#include "lwip/priv/tcp_priv.h"
#include "lwip/priv/api_msg.h"
#include "lwip/err.h"
#include "Structure.h"
//===============================
// MACRO Constants
//===============================
#define LED_BUILTIN 16
#define MAX_CONNECTIONS 4
#define SPECIAL_STRING_LENGTH 256
#define EXTRACT_URL_TEMPLATE_SIZE 128
//===============================
// MACRO functions
//===============================
#define delay(ms) (vTaskDelay(ms/portTICK_RATE_MS))
//===============================
// Constants Field
//===============================
const static char* TAG = "webserver-see";

const static char http_html_hdr[] = "HTTP/1.1 200 OK\r\n"
                                    "Access-Control-Allow-Origin: *\r\n"
                                    "Content-type: text/html\r\n"
                                    "\r\n";
const static char http_sse_hdr[]  = "HTTP/1.1 200 OK\r\n"
                                    "Access-Control-Allow-Origin: *\r\n"
                                    "Content-Type: text/event-stream\r\n"
                                    "Cache-Control: no-cache\r\n"
                                    "Connection: keep-alive\r\n"
                                    "\r\n";

const static char http_index_html[] = "<!DOCTYPE html>"
                                      "<html>\n"
                                      "<head>\n"
                                      "<title>HELLO ESP32</title>\n"
                                      "</head>\n"
                                      "<body>\n"
                                      "<h1>Hello World, from ESP32!</h1>\n"
                                      "</body>\n"
                                      "</html>\n";
const static char http_sse_html[]  = "<!DOCTYPE html>\n"
                                    "<html>\n"
                                    "<head>\n"
                                    "   <title>ESP32 SSE Example</title>\n"
                                    "   <meta name='viewport' content='width=device-width, initial-scale=1'>\n"
                                    "   <style type='text/css'>\n"
                                    "       html, body { height: 100%; background: #f9f9f9; }\n"
                                    "       body { font-family: 'Courier New', Courier, monospace; }\n"
                                    "       #container\n"
                                    "       {\n"
                                    "           position: absolute;\n"
                                    "           top: 50%;\n"
                                    "           left: 50%;\n"
                                    "           transform: translate(-50%, -50%);\n"
                                    "           width: 600px;\n"
                                    "           height: 600px;\n"
                                    "           background: white;\n"
                                    "           border-radius: 10px;\n"
                                    "           display: flex;\n"
                                    "           align-items: center;\n"
                                    "           justify-content: center;\n"
                                    "           flex-direction: column;\n"
                                    "           max-height: 100%;\n"
                                    "           max-width: 100%;\n"
                                    "       }\n"
                                    "       #container div\n"
                                    "       {\n"
                                    "           text-align: center;\n"
                                    "           width: 100%;\n"
                                    "       }\n"
                                    "       #container div button\n"
                                    "       {\n"
                                    "           text-align: center;\n"
                                    "           width: calc(100% / 3.2);\n"
                                    "           border-radius: 5px;\n"
                                    "           border: 1px solid #ccc;\n"
                                    "           background: white;\n"
                                    "           height: 50%;\n"
                                    "       }\n"
                                    "       #container div button:hover\n"
                                    "       {\n"
                                    "           background: #ccc;\n"
                                    "           cursor: pointer;\n"
                                    "       }\n"
                                    "       #container div input\n"
                                    "       {\n"
                                    "           border-radius: 5px;\n"
                                    "           height: 50%;\n"
                                    "           border: 1px solid #ccc;\n"
                                    "       }\n"
                                    "       #container div p { color: red; }\n"
                                    "       #server-data\n"
                                    "       {\n"
                                    "           height: 92.5%;\n"
                                    "           width:90%;\n"
                                    "           border-radius: 5px;\n"
                                    "           border: 1px solid #ccc;\n"
                                    "       }\n"
                                    "   </style>\n"
                                    "</head>\n"
                                    "<body>\n"
                                    "   <div id='container'>\n"
                                    "       <div style='height: 7.5%;margin-top: 2.5%;'>\n"
                                    "           <button onclick='sendData()'>AJAX Send</button>\n"
                                    "           <button onclick='recieveData(this)'>Start Server Side Events</button>\n"
                                    "           <button onclick='toggleInterval(this)'>Toggle Interval</button>\n"
                                    "           <input name='client-data' id='client-data' type='text' style='width:90%' />\n"
                                    "       </div>\n"
                                    "       <div style='height: 3.5%;'> <p id='response'> response </p> </div>\n"
                                    "       <div style='height: 90%; margin-top: 2.5%;'>\n"
                                    "           <textarea id='server-data' ></textarea>\n"
                                    "       </div>\n"
                                    "   </div>\n"
                                    "</body>\n"
                                    "<script type='text/javascript'>\n"
                                    "    var source_sta, source_ap;\n"
                                    "    const URL = 'http://192.168.4.1';\n"
                                    "    // const URL = 'http://192.168.0.104';\n"
                                    "    var response = document.querySelector('#response');\n"
                                    "    var busy_counter = 0;\n"
                                    "    function recieveData(obj)\n"
                                    "    {\n"
                                    "       if(source_ap)\n"
                                    "       {\n"
                                    "           obj.style.background = 'red';\n"
                                    "           response.innerHTML = 'Resetting Event Source';\n"
                                    "           source_ap.close();\n"
                                    "           source_ap = null;\n"
                                    "       }\n"
                                    "       else\n"
                                    "       {\n"
                                    "           obj.style.background = 'limegreen';\n"
                                    "           response.innerHTML = 'Connecting...';\n"
                                    "           source_ap = new EventSource(URL);\n"
                                    "           source_ap.onopen = function()\n"
                                    "           {\n"
                                    "               response.innerHTML = 'Connected';\n"
                                    "           };\n"
                                    "           source_ap.onmessage = function(event)\n"
                                    "           {\n"
                                    "               console.log(event);\n"
                                    "               var text_area = document.querySelector('#server-data');\n"
                                    "               text_area.innerHTML = `id = ${event.lastEventId} :: data = ${event.data}\n` + text_area.innerHTML;\n"
                                    "           };\n"
                                    "           source_ap.onerror = function(event)\n"
                                    "           {\n"
                                    "               response.innerHTML = `failed to connect to ${URL}`;\n"
                                    "               source_ap.close();\n"
                                    "               source_ap = null;\n"
                                    "           };\n"
                                    "       }\n"
                                    "    }\n"
                                    "    function sendData()\n"
                                    "    {\n"
                                    "       var oReq = new XMLHttpRequest();\n"
                                    "       var value = document.querySelector('#client-data').value.replace(/ /g, '_');\n"
                                    "       oReq.open('GET', `${URL}?data=${value}`);\n"
                                    "       oReq.send();\n"
                                    "    }\n"
                                    "    var counter = 0;\n"
                                    "    var interval;\n"
                                    "    function toggleInterval(obj)\n"
                                    "    {\n"
                                    "       // console.log(interval);\n"
                                    "       console.log(obj);\n"
                                    "       if(interval)\n"
                                    "       {\n"
                                    "           clearInterval(interval);\n"
                                    "           interval = null;\n"
                                    "           obj.style.background = 'red';\n"
                                    "       }\n"
                                    "       else\n"
                                    "       {\n"
                                    "           obj.style.background = 'limegreen';\n"
                                    "           interval = setInterval(() =>\n"
                                    "           {\n"
                                    "               var oReq = new XMLHttpRequest();\n"
                                    "               var value = `testing_${counter++}`;\n"
                                    "               console.log(value);\n"
                                    "               oReq.open('GET', `${URL}?data=${value}`);\n"
                                    "               oReq.send();\n"
                                    "           }, 40);\n"
                                    "       }\n"
                                    "    }\n"
                                    "</script>\n"
                                    "</html>\n";

const static char success[]         = "success";
const static char failure[]         = "failure";
const static char notfound_404[]    = "404 not found!";
//===============================
// Structures and Enumerations
//===============================

wifi_config_t sta_config =
{
    .sta = 
    {
        .ssid = "SJSURoboticsNET",
        .password = "chimera2018",
        .bssid_set = false
    }
};
wifi_config_t ap_config =
{
    .ap = 
    {
        .ssid = "SSE-ESP32-EXAMPLE",
        .ssid_len = 0,
        .password = "testing1234",
        .channel = 3,
        .authmode = WIFI_AUTH_WPA2_PSK,
        .beacon_interval = 500,
        .max_connection = 16,
    }
};
//===============================
// Global Variables
//===============================
//// WiFi
static EventGroupHandle_t wifi_event_group;
const int CONNECTED_BIT = BIT0;
struct netconn * clients[MAX_CONNECTIONS] = { NULL };
int HEARTBEAT_TIMESTAMP; //timestamp for data from mission control
int STATIC_IP_FLAG = 0;  // flag to set static ip mode on 
char url[256] = { 0 }; 
char special_string[SPECIAL_STRING_LENGTH] = "special";
char SYSTEM_DEBUG_MSG[50]                  = {"NONE"}; //debug message
char DEVICE_IP[]                           = "192.168.1.2";
const static char DEVICE_GW[]              = "192.168.5.1";
const static char DEVICE_NETMASK[]         = "255.255.255.0";
SemaphoreHandle_t xSemaphore;
SemaphoreHandle_t xSemaphore_url;

//===============================
// Function Fields
//===============================
static esp_err_t event_handler(void *ctx, system_event_t *event)
{
    switch (event->event_id)
    {
        case SYSTEM_EVENT_STA_START:
            esp_wifi_connect();
            break;
        case SYSTEM_EVENT_STA_GOT_IP:
            xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
            printf("got ip\n");
            printf("ip: " IPSTR "\n", IP2STR(&event->event_info.got_ip.ip_info.ip));
            printf("netmask: " IPSTR "\n", IP2STR(&event->event_info.got_ip.ip_info.netmask));
            printf("gw: " IPSTR "\n", IP2STR(&event->event_info.got_ip.ip_info.gw));
            printf("\n");
            fflush(stdout);
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            /* This is a workaround as ESP32 WiFi libs don't currently
                 auto-reassociate.
                But for this project's purposes, do not reconnect!
            */
            // esp_wifi_connect();
            xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
            break;
        case SYSTEM_EVENT_AP_STADISCONNECTED:

            break;
        default:
            break;
    }
    return ESP_OK;
}

/*static*/ void initialise_wifi(void)
{
    tcpip_adapter_init();
    if(STATIC_IP_FLAG == 1)
    {
        tcpip_adapter_dhcpc_stop(TCPIP_ADAPTER_IF_STA); // Don't run a DHCP client
        tcpip_adapter_ip_info_t ipInfo;
        inet_pton(AF_INET, DEVICE_IP, &ipInfo.ip);
        inet_pton(AF_INET, DEVICE_GW, &ipInfo.gw);
        inet_pton(AF_INET, DEVICE_NETMASK, &ipInfo.netmask);
        tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_STA, &ipInfo);
    }
    wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_APSTA) );
    ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_STA, &sta_config) );
    ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_AP, &ap_config) );
    ESP_ERROR_CHECK( esp_wifi_start() );
    ESP_ERROR_CHECK( esp_wifi_connect() );
}

bool extract_url_variable(const char * variable,
                          const char * url,
                          uint32_t url_length,
                          char * dest,
                          uint32_t dest_length)
{
    bool success = false;
    char template[EXTRACT_URL_TEMPLATE_SIZE] = { 0 };
    char * variable_found = strstr(url, variable);
    if (variable_found != NULL)
    {
        snprintf(template, EXTRACT_URL_TEMPLATE_SIZE, "%s=%%%d[^&\\ \n\r]", variable, dest_length);
        sscanf(variable_found, (const char *)template, dest);
        //("%s\n",template);
        success = true;
    }
    return success;
}

bool add_sse_client(struct netconn * conn)
{
    bool success = false;
    bool semaphore_taken = false;
    semaphore_taken = xSemaphoreTake( xSemaphore, ( TickType_t ) 10 );
    if(semaphore_taken == pdTRUE)
    {
        for (int i = 0; i < MAX_CONNECTIONS; ++i)
        {
            if (clients[i] == NULL)
            {
                //// 75 seconds
                // conn->so_options |= SOF_KEEPALIVE;
                // conn->keep_intvl = 75000;
                clients[i] = conn;
                printf("Added SSE to Channel (%d)\n", i);
                success = true;
                break;
            }
        }
    }
    if (!success || semaphore_taken != pdTRUE)

    {
        printf("Refused! SSE Channels full!\n");
    }
    xSemaphoreGive(xSemaphore);
    return success;
}

/*static*/ void http_server_netconn_serve(void *pvParameters)
{
    struct netconn *conn = (struct netconn *)pvParameters;
    struct netbuf *inbuf;
    char *buf;
    uint16_t buflen;
    bool close_flag = true;
    bool semaphore_taken = false;
    //char url[256] = { 0 };
    char * is_event_stream = NULL;
    err_t err;

    /* Read the data from the port, blocking if nothing yet there.
     We assume the request (the part we care about) is in one netbuf */
    err = netconn_recv(conn, &inbuf);

    if (err == ERR_OK)
    {
        netbuf_data(inbuf, (void**)&buf, &buflen);
        //// NULL Terminate end of buffer
        buf[buflen] = '\0';
        //// Attempt to find "Accept: text/event-stream" string within request
        //// Adding the first and last "\r\n" means that this is not the first or last line
        is_event_stream = strstr(buf, "\r\nAccept: text/event-stream\r\n");
        //// Output captured http request information
        // printf("buf = %.*s\n", buflen, buf);
        // printf("ACCEPT = 0x%X\n", is_event_stream);
        if(is_event_stream != NULL)
        {
            if (add_sse_client(conn))
            {
                netconn_write(conn, http_sse_hdr, sizeof(http_sse_hdr) - 1, NETCONN_NOCOPY);
                close_flag = false;
            }
        }
        else
        {
            //Lock url 
            semaphore_taken = xSemaphoreTake( xSemaphore_url, ( TickType_t ) 10 );
            //check mutex lock
            if(semaphore_taken == pdTRUE){
                sscanf(buf, "POST %255s HTTP/1.1\n", url);
                printf("url = %s\n", url);
                //// Beyond this point, I am only sending html data back!
                netconn_write(conn, http_html_hdr, sizeof(http_html_hdr) - 1, NETCONN_NOCOPY);
                //// More cases
                if (strstr(url, "/?") != NULL)
                {
                    bool success_flag = extract_url_variable(
                        "data",
                        url,
                        strlen(url),
                        special_string,
                        SPECIAL_STRING_LENGTH
                    );
                    printf("%d)%s\n", success_flag, special_string);
                    if (success_flag)
                    {
                        netconn_write(conn, success, sizeof(success) - 1, NETCONN_NOCOPY);
                    }
                    else
                    {
                        netconn_write(conn, failure, sizeof(failure) - 1, NETCONN_NOCOPY);
                    }
                }
                else if (strstr(url, "/sse") != NULL)
                {
                    netconn_write(conn, http_sse_html, sizeof(http_sse_html) - 1, NETCONN_NOCOPY);
		    printf("/sse\n");
		}
                else if (strstr(url, "/") != NULL || strstr(url, "/index") != NULL)
                {
                    netconn_write(conn, http_index_html, sizeof(http_index_html) - 1, NETCONN_NOCOPY);
		    printf("/\n");
                }
                else
                {
                    netconn_write(conn, notfound_404, sizeof(notfound_404) - 1, NETCONN_NOCOPY);
                }
            }
        xSemaphoreGive(xSemaphore_url);
        }
    }
    else
    {
        printf("err = %d \n", err);
    }

    netbuf_delete(inbuf);

    if (close_flag)
    {
        netconn_close(conn);
        if(err == ERR_OK)
        {
            netconn_free(conn);
        }
        else
        {
            printf("DID NOT FREE NETCONN BECAUSE err != ERR_OK => %d\n", err);
        }
    }

    /* Delete this thread, it is done now. Sleep precious child. */
    vTaskDelete(NULL);
}

inline char int_to_ascii_hex(uint32_t n)
{
    char c = '0';
    if(n < 10)
    {
        c = '0'+n;
    }
    else
    {
        c = 'A'+(n-10);
    }
    return c;
}

/*static*/ void handle_sse()
{
    char sse_buffer[128] = { 0 };
    uint32_t sse_id = 0;
    while (true)
    {
        // printf("ram=%d\n",esp_get_free_heap_size());
        sprintf(sse_buffer, "id: %08X\ndata: (_) :: %s \n\n\r\n", sse_id,special_string);

        for (int i = 0; i < MAX_CONNECTIONS; ++i)
        {
            if (clients[i] != NULL)
            {
                sse_buffer[20] = int_to_ascii_hex(i);
                err_t error = netconn_write(clients[i], sse_buffer, strlen(sse_buffer) - 1, NETCONN_COPY);
                //printf(sse_buffer);
                if (error != ERR_OK)
                {
                    printf("connection #%d shows error %d\n", i, error);
                    netconn_close(clients[i]);
                    netconn_delete(clients[i]);
                    clients[i] = NULL;
                }
            }
        }

        sse_id++;
        vTaskDelay(200);
    }
}

/*static*/ void http_server(void *pvParameters)
{
    struct netconn *conn, *newconn;
    err_t err;
    conn = netconn_new(NETCONN_TCP);
    netconn_bind(conn, NULL, 80);
    netconn_listen(conn);
    do
    {
        err = netconn_accept(conn, &newconn);
        uint32_t tasks = uxTaskGetNumberOfTasks();
        if (err == ERR_OK)
        {
            // printf("new conn+task :: %d :: %d :: %d\n", tasks, esp_get_free_heap_size());
            xTaskCreate(http_server_netconn_serve, "http_server_netconn_serve", 2048,  newconn, 5, NULL);
        }
    }
    while (err == ERR_OK);
    /* Delete Listening Server */
    netconn_close(conn);
    netconn_delete(conn);
}

int read_data_wifi(int item_size)
{
   char input[256];
   int timestamp=0;
   char* timestamp_variable,debug_variable;
   char* temp[item_size];
   char* temp2[4];
   strcpy(input,url);
   if(input[7] == '{')
   {
       char* token=strtok(input,",");
       int i=0;
       while(token)
       {
           temp[i++]=token;
           token=strtok(NULL,",");
       }
       for(int j=0;j<item_size;j++){
           token=strtok(temp[j],":");
           i=0;
           while(token)
           {
               temp2[i++]=token;
               token=strtok(NULL,":");
           }
           //long int num = strtol(temp2[1], 0, 10);
           int num = atoi(temp2[1]);
           read_data.data[j]=num;
       }
       timestamp_variable = strstr(url, "timestamp");
       if(timestamp_variable != NULL )
       {
            timestamp_variable = strstr(timestamp_variable, ":");
            timestamp_variable += 1;
            timestamp = atoi(timestamp_variable);
	    HEARTBEAT_TIMESTAMP=timestamp;

       }
   }
   return timestamp;
}

void send_data_wifi(int item_size)
{
    char buffer[10];
    char final[256];
    strcpy(final,"{");
    for(int i=0; i<item_size;i++)
    {
        if (i == 0)
        {
            sprintf(final, "{%s:%d,", send_data_label[i], send_data.data[i]);
        }
        else if(i!=item_size-1 && i != 0)
        {
            sprintf(final+strlen(final), "%s:%d,", send_data_label[i], send_data.data[i]);
        }
        else
        {
            sprintf(final+strlen(final), "%s:%d,%s:%s}", send_data_label[i], send_data.data[i],"debug",SYSTEM_DEBUG_MSG);
        }
    }
    strcpy(special_string,final);
};

int set_wifi_ap(char name[],int name_size)
{
  int error_flag = 0;
  if(name_size<34)
  {
    for(int i = 0; i < name_size; i++)
    {
       ap_config.ap.ssid[i]=name[i];
    }
    ap_config.ap.ssid[name_size]='\0';
    // for(int i = name_size; i < 34; i++)
    // {
    //    ap_config.ap.ssid[i]='\0';
    // }  
  }
  else 
  {
    error_flag = 1;
  } 
  return error_flag;
}

void set_static_ip(char ip[])
{
    STATIC_IP_FLAG = 1;
    strcpy(DEVICE_IP,ip);
} 

void create_wifi_task(void)
{
  nvs_flash_init();
  xSemaphore = xSemaphoreCreateMutex();
  xSemaphore_url = xSemaphoreCreateMutex();

  initialise_wifi();

  xTaskCreate(handle_sse, "handle_sse", 4096, NULL, 1, NULL);
  xTaskCreate(http_server, "http_server", 4096, NULL, 1, NULL);
}
