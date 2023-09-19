#include "sleep.h"

#define TAG "sleep_stack"

#define LED_WH GPIO_NUM_14

EventGroupHandle_t event_group;
const int mes_success = BIT0;
const int mes_failure = BIT1;
const int turn_off = BIT2;
const int wifi_on = BIT3;

void measurement_successfull(void) 
{
    xEventGroupSetBits(event_group, mes_success);
}

void measurement_failure(void)
{
    xEventGroupSetBits(event_group, mes_failure);
}

void turn_deep_sleep(void)
{
    xEventGroupSetBits(event_group, turn_off);
}

void turn_wifi(void)
{
    xEventGroupSetBits(event_group, wifi_on);
}

void enter_off_sleep(void) 
{
    gpio_set_level(LED_WH, 1);
    gpio_hold_en(LED_WH);

    ESP_LOGW(TAG, "Entering off sleep");

    vTaskDelay(2000 / portTICK_PERIOD_MS);

    gpio_hold_dis(LED_WH);
    gpio_set_level(LED_WH, 0);
    gpio_hold_en(LED_WH);

    vTaskDelay(2000 / portTICK_PERIOD_MS);
    // enter deep sleep
    esp_deep_sleep_start();
}

void enter_deep_sleep(void) 
{
    gpio_set_level(LED_WH, 0);
    gpio_hold_en(LED_WH);

    ESP_LOGW(TAG, "Entering deep sleep");
    // enter deep sleep
    esp_deep_sleep_start();
}

static void deep_sleep_register_rtc_timer_wakeup(int wakeup_time_sec)
{
    // const int wakeup_time_sec = 30;
    
    ESP_LOGI(TAG, "Enabling timer wakeup, %ds", wakeup_time_sec);
    ESP_ERROR_CHECK(esp_sleep_enable_timer_wakeup(wakeup_time_sec * 1000000));
}

void sleep_off_task(void *arg)
{
    while (true)
    {
        xEventGroupWaitBits(event_group, turn_off, false, false, portMAX_DELAY);
        ESP_LOGI(TAG, "deep sleep without wake");
        
        set_wifi_mode(false);
        enter_off_sleep();
    }
}

void wifi_on_task(void *arg)
{
    while (true)
    {
        xEventGroupWaitBits(event_group, wifi_on, false, false, portMAX_DELAY);
        ESP_LOGI(TAG, "restart to wifi");

        set_wifi_mode(true);
        esp_restart();
    }
}

void mes_task(void *arg)
{
    while (true)
    {
        EventBits_t bits = xEventGroupWaitBits(event_group, mes_success | mes_failure, false, false, portMAX_DELAY);

        if (!(bits & (turn_off | wifi_on))) {
            ESP_LOGI(TAG, "deep sleep with wake");

            set_wifi_mode(false);
            if(bits & mes_success) {
                deep_sleep_register_rtc_timer_wakeup(120);
            }
            else if (bits & mes_failure) {
                deep_sleep_register_rtc_timer_wakeup(30);
            }
            
            enter_deep_sleep();
        }

    }
}

void setup_sleep(void) 
{
    event_group = xEventGroupCreate();

    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);
    gpio_pullup_dis(GPIO_NUM_27);
    gpio_pulldown_en(GPIO_NUM_27);
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_27, 1);

    // deep_sleep_register_rtc_timer_wakeup();
    xTaskCreate(sleep_off_task, "sleep_off_task", 2048, NULL, 1, NULL);
    xTaskCreate(wifi_on_task, "wifi_on_task", 2048, NULL, 1, NULL);
    xTaskCreate(mes_task, "measurement_task", 2048, NULL, 1, NULL);
}

