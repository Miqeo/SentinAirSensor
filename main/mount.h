#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "esp_err.h"
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "json_parser.h"

#define PIN_MOSI GPIO_NUM_23
#define PIN_MISO GPIO_NUM_19
#define PIN_CLK GPIO_NUM_18
#define PIN_CS GPIO_NUM_5

#define EXAMPLE_MAX_CHAR_SIZE    500

#define MOUNT_POINT "/sdcard"

esp_err_t mount_storage(void);
void write_file(char* value, char* title);
void save_json_measurement(struct Measurement_structure measurement, char* title);