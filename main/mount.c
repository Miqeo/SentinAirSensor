#include "mount.h"

static const char *TAG = "mount_stack";

sdmmc_host_t host = SDSPI_HOST_DEFAULT();
sdmmc_card_t *card;

char data[EXAMPLE_MAX_CHAR_SIZE];
char file_json[100];
char *array_to_save;

esp_err_t mount_storage(void)
{
    ESP_LOGI(TAG, "Initializing SD card");
    esp_err_t ret;

    // Options for mounting the filesystem.
    // If format_if_mount_failed is set to true, SD card will be partitioned and
    // formatted in case when mounting fails.
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
#ifdef CONFIG_EXAMPLE_FORMAT_IF_MOUNT_FAILED
        .format_if_mount_failed = true,
#else
        .format_if_mount_failed = false,
#endif // EXAMPLE_FORMAT_IF_MOUNT_FAILED
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };
    
    const char mount_point[] = MOUNT_POINT;
    ESP_LOGI(TAG, "Initializing SD card");

    // Use settings defined above to initialize SD card and mount FAT filesystem.
    // Note: esp_vfs_fat_sdmmc/sdspi_mount is all-in-one convenience functions.
    // Please check its source code and implement error recovery when developing
    // production applications.
    ESP_LOGI(TAG, "Using SPI peripheral");

    // By default, SD card frequency is initialized to SDMMC_FREQ_DEFAULT (20MHz)
    // For setting a specific frequency, use host.max_freq_khz (range 400kHz - 20MHz for SDSPI)
    // Example: for fixed frequency of 10MHz, use host.max_freq_khz = 10000;
    

    spi_bus_config_t bus_cfg = {
        .mosi_io_num = PIN_MOSI,
        .miso_io_num = PIN_MISO,
        .sclk_io_num = PIN_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4000,
    };
    ret = spi_bus_initialize(host.slot, &bus_cfg, SDSPI_DEFAULT_DMA);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize bus.");
        return ESP_FAIL;
    }

    // This initializes the slot without card detect (CD) and write protect (WP) signals.
    // Modify slot_config.gpio_cd and slot_config.gpio_wp if your board has these signals.
    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = PIN_CS;
    slot_config.host_id = host.slot;

    ESP_LOGI(TAG, "Mounting filesystem");
    ret = esp_vfs_fat_sdspi_mount(mount_point, &host, &slot_config, &mount_config, &card);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount filesystem. "
                     "If you want the card to be formatted, set the CONFIG_EXAMPLE_FORMAT_IF_MOUNT_FAILED menuconfig option.");
        } else {
            ESP_LOGE(TAG, "Failed to initialize the card (%s). "
                     "Make sure SD card lines have pull-up resistors in place.", esp_err_to_name(ret));
        }
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "Filesystem mounted");

    // Card has been initialized, print its properties
    sdmmc_card_print_info(stdout, card);

    return ESP_OK;
}

void deinitialiseCard(void) {
    // All done, unmount partition and disable SPI peripheral
    esp_vfs_fat_sdcard_unmount(MOUNT_POINT, card);
    ESP_LOGI(TAG, "Card unmounted");

    //deinitialize the bus after all devices are removed
    spi_bus_free(host.slot);
}

static esp_err_t s_example_read_file(const char *path)
{
    ESP_LOGI(TAG, "example reading file %s", path);
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for reading");
        return ESP_FAIL;
    }
    // char data[EXAMPLE_MAX_CHAR_SIZE];


    // char buf[260];
    // fgets(buf, sizeof(buf), f);

    size_t n = 0;

    while ( fgets( data + n,20,stdin) != NULL)
    {
        n += strlen( data + n );
    }
    
    
    fclose(f);

    // // strip newline
    // char *pos = strchr(data, '\n');
    // if (pos) {
    //     *pos = '\0';
    // }
    ESP_LOGI(TAG, "Read from file: '%s'", data);

    return ESP_OK;
}

static esp_err_t s_example_write_file(const char *path, char *data)
{
    ESP_LOGI(TAG, "example opening file %s", path);
    FILE *f = fopen(path, "w");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for writing");
        return ESP_FAIL;
    }
    fprintf(f, data);
    fclose(f);
    ESP_LOGI(TAG, "File written");

    return ESP_OK;
}



void write_file(char* value, char* title) 
{
    esp_err_t ret;
    // char data[EXAMPLE_MAX_CHAR_SIZE];
    
    // const char *file_json = MOUNT_POINT;

    
    snprintf(file_json, sizeof(file_json), "%s/%s.json", MOUNT_POINT, title);

    memset(data, 0, EXAMPLE_MAX_CHAR_SIZE);
    snprintf(data, EXAMPLE_MAX_CHAR_SIZE, "%s", value);

    ESP_LOGI(TAG, "saving file %s with %s", file_json, data);
    ret = s_example_write_file(file_json, data);
    if (ret != ESP_OK) {
        return;
    }

    //Open file for reading
    ret = s_example_read_file(file_json);
    if (ret != ESP_OK) {
        return;
    }
}

void save_json_measurement(struct Measurement_structure measurement, char* title)
{
    char path[100];

    snprintf(path, sizeof(path), "%s/%s.json", MOUNT_POINT, title);

    ESP_LOGI(TAG, "Reading file as json %s", path);
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        ESP_LOGW(TAG, "Failed to open file for reading");

        array_to_save = create_json(measurement);

        // parse_json(array_to_save, measurement);

        ESP_LOGI(TAG, "add to json: %s", array_to_save);
        
        write_file(array_to_save, title);
        return;
    }
    else {
        size_t n = 0;

        while ( fgets( data + n, 50, f) != NULL)
        {
            n += strlen( data + n );
        }
        
        
        fclose(f);

        // fgets(data, sizeof(data), f);
        // fclose(f);
        
        // char *pos = strchr(data, '\n');
        // if (pos) {
        //     *pos = '\0';
        // }
        ESP_LOGI(TAG, "Read from file: '%s'", data);

        array_to_save = create_json(measurement);

        ESP_LOGI(TAG, "add to json: %s", array_to_save);
        
        write_file(array_to_save, title);
    }
    
}

