#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "esp_err.h"
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"

#define PIN_MOSI GPIO_NUM_23
#define PIN_MISO GPIO_NUM_19
#define PIN_CLK GPIO_NUM_18
#define PIN_CS GPIO_NUM_5

#define MOUNT_POINT "/sdcard"

esp_err_t mount_storage(const char* base_path);
