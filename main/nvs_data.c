#include "nvs_data.h"

#define TAG "nvs_stack"

bool get_wifi_mode(void)
{
    bool wifi_mode_status;

    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        wifi_mode_status = true;
    } else {
        ESP_LOGI(TAG, "Open NVS done\n");
        wifi_mode_status = true;
    }

    nvs_get_i32(nvs_handle, "wifi_mode", (bool *)&wifi_mode_status);

    nvs_close(nvs_handle);

    return wifi_mode_status;
}

void set_wifi_mode(bool wifi_mode)
{
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        ESP_LOGI(TAG, "Open NVS done\n");
    }

    ESP_ERROR_CHECK(nvs_set_i32(nvs_handle, "wifi_mode", wifi_mode));
    ESP_ERROR_CHECK(nvs_commit(nvs_handle));
    nvs_close(nvs_handle);
}

void modes_setup(void)
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}