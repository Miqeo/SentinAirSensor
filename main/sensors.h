#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

#include "sleep.h"

#include "nmea_parser.h"

#include "mount.h"

#include "bmx280.h"

#include "json_parser.h"

#define BUF_SIZE (1024)

#define YEAR_BASE (2000)


void gps_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

int bm_init(void);
void bm_deinit(void);
int pm_init(void);

double pm_sense();
double bm();
