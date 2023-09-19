#include "esp_log.h"
#include "nvs_flash.h"
#include "nvs.h"


bool get_wifi_mode(void);
void set_wifi_mode(bool wifi_mode);
void modes_setup(void);