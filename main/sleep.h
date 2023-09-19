#include "esp_sleep.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "nvs_data.h"
#include "esp_system.h"


void setup_sleep(void);

void measurement_successfull(void);
void measurement_failure(void);
void turn_deep_sleep(void);
void turn_wifi(void);
