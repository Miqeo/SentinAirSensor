#include "esp_wifi.h"
#include "esp_netif.h"
#include "lwip/inet.h"

#include "esp_http_server.h"
#include "dns_server.h"

#include "nvs_flash.h"
#include "esp_mac.h"
#include "esp_log.h"

#define EXAMPLE_ESP_WIFI_SSID CONFIG_ESP_WIFI_SSID
#define EXAMPLE_ESP_WIFI_PASS CONFIG_ESP_WIFI_PASSWORD
#define EXAMPLE_MAX_STA_CONN CONFIG_ESP_MAX_STA_CONN

extern const char root_start[] asm("_binary_root_html_start");
extern const char root_end[] asm("_binary_root_html_end");

static const char *TAG = "wifi_stack";

static void wifi_init_softap(void);
static httpd_handle_t start_webserver(void);
void start_wifi_c_p(void);