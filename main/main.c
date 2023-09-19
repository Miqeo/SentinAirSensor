
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_event.h"

#include "mount.h"
#include "sensors.h"
#include "sleep.h"

#include "wifi_stack.h"

#include "driver/uart.h"
#include "driver/gpio.h"

// #include <inttypes.h>


static const char *TAG = "main_stack";

#define LED_WH GPIO_NUM_14

void diode_task(void *arg) 
{   
    bool s_led_state = false;

    int delay = get_wifi_mode() ? 200 : 500;
    while (true) {
        s_led_state = !s_led_state;
        gpio_set_level(LED_WH, s_led_state);

        vTaskDelay(delay / portTICK_PERIOD_MS);
    }
}



void button_task(void *arg)
{
    bool button_state_off = false; //5000
    bool button_state_wifi = false; //2000

    int length_of_hold = 0;
    int time_since_press = 0;

    while (true) {
        if (gpio_get_level(GPIO_NUM_27)) {
            length_of_hold += 10;
        }
        else if (length_of_hold > 0) {

            if (length_of_hold <= 2000 && length_of_hold > 700) {
                button_state_wifi = true;
            }

            if (length_of_hold <= 5000 && length_of_hold > 2000) {
                button_state_off = true;
            }

            if (button_state_wifi && !button_state_off) {
                ESP_LOGW(TAG, "turn_wifi");
                turn_wifi();
            }
            else if (button_state_off) {
                ESP_LOGW(TAG, "turn_deep_sleep");
                turn_deep_sleep();
            }

            length_of_hold = 0;
        }

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    mount_storage();

    // gpio_reset_pin(GPIO_NUM_25);
    // gpio_set_direction(GPIO_NUM_25, GPIO_MODE_OUTPUT);
    // gpio_hold_dis(GPIO_NUM_25);

    // if (bm_init() != 0) { return; }
    // bm();
    
    // // bm_deinit();

    // if (pm_init() != 0) { return; }
    // pm_sense(); 

    

    gpio_reset_pin(GPIO_NUM_14);
    gpio_set_direction(GPIO_NUM_14, GPIO_MODE_OUTPUT);
    gpio_hold_dis(GPIO_NUM_14);

    modes_setup();
    setup_sleep();

    xTaskCreate(diode_task, "blink_task", 2048, NULL, 10, NULL);
    xTaskCreate(button_task, "button_task", 2048, NULL, 1, NULL);

    vTaskDelay(5000 / portTICK_PERIOD_MS);
    
    
    // if (bm_init() != 0 && pm_init() != 0) {
    //     return;
    // }

    if (get_wifi_mode()) {
        ESP_LOGI(TAG, "Currently in WIFI mode");

        esp_log_level_set("httpd_uri", ESP_LOG_ERROR);
        esp_log_level_set("httpd_txrx", ESP_LOG_ERROR);
        esp_log_level_set("httpd_parse", ESP_LOG_ERROR);

        set_wifi_mode(false);
        start_wifi_c_p();

        return;
    }
    
    nmea_parser_config_t config = NMEA_PARSER_CONFIG_DEFAULT();
    /* init NMEA parser library */
    nmea_parser_handle_t nmea_hdl = nmea_parser_init(&config);
    /* register event handler for NMEA parser library */
    nmea_parser_add_handler(nmea_hdl, gps_event_handler, NULL);
}
