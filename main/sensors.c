#include "sensors.h"

#define TAG "sensors_stack"

struct Bmx280_data 
{
    double humidity;
    double temperature;
    double pressure;
};

bmx280_t *bmx280;

static esp_adc_cal_characteristics_t adc1_chars;

double pm_sense() 
{
    ESP_LOGI(TAG, "Starting sampling pm sensor.");

    bool s_led_state = false;
    int voltage;
    int delay = 280;
    int num_retry = 3;
    double pm_average = 0;
    double voltage_average = 0;

    for (int i = num_retry; i > 0; i--)
    {
        s_led_state = !s_led_state;
        gpio_set_level(GPIO_NUM_25, s_led_state);

        if (!s_led_state) {
            voltage = esp_adc_cal_raw_to_voltage(adc1_get_raw(ADC1_CHANNEL_7), &adc1_chars);
        }
        
        vTaskDelay(delay / portTICK_PERIOD_MS);

        if (!s_led_state) {
            voltage_average += voltage;
            // pm_average += 0.17 * voltage - 0.01;
            ESP_LOGI(TAG, "Led state: %i ADC1_CHANNEL_7: %d mV", s_led_state, voltage);
        }
    }


    ESP_LOGI(TAG, "voltage average: %f mV", voltage_average / num_retry);

    return (voltage_average / num_retry) / 1000;
}

int pm_init(void)
{
    gpio_set_direction(GPIO_NUM_25, GPIO_MODE_OUTPUT);
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_2_5, ADC_WIDTH_BIT_DEFAULT, 0, &adc1_chars);


    if (adc1_config_width(ADC_WIDTH_BIT_DEFAULT) != 0) {
        return 1;
    }

    if (adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_2_5) != 0) {
        return 1;
    }

    return 0;
}

int bm_init(void) 
{
    i2c_config_t i2c_cfg = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = GPIO_NUM_32,
        .scl_io_num = GPIO_NUM_33,
        .sda_pullup_en = false,
        .scl_pullup_en = false,

        .master = {
            .clk_speed = 100000
        }
    };

    ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_0, &i2c_cfg));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0));
    
    bmx280 = bmx280_create(I2C_NUM_0);

    if (!bmx280) { 
        ESP_LOGE(TAG, "Could not create bmx280 driver.");
        return 1;
    }


    if (bmx280_init(bmx280) != 0) {
        ESP_LOGE(TAG, "Could not initialise bmx280.");
        return 1; 
    }

    bmx280_config_t bmx_cfg = BMX280_DEFAULT_CONFIG;

    if (bmx280_configure(bmx280, &bmx_cfg) != 0) {
        ESP_LOGE(TAG, "Could not configure bmx280.");
        return 1; 
    }
    
    return ESP_OK;
}

void bm_deinit(void)
{
    bmx280_close(bmx280);
    i2c_driver_delete(I2C_NUM_0);
}

struct Bmx280_data bm_sense(void) 
{
    ESP_LOGI(TAG, "Starting sampling bmx280.");

    int num_retry = 4;

    struct Bmx280_data bmx_data;
    bmx_data.humidity = 0;
    bmx_data.pressure = 0;
    bmx_data.temperature = 0;

    for (int i = num_retry; i > 0; i--)
    {
        ESP_ERROR_CHECK(bmx280_setMode(bmx280, BMX280_MODE_FORCE));
        do {
            vTaskDelay(pdMS_TO_TICKS(1));
        } while(bmx280_isSampling(bmx280));

        float temp = 0, pres = 0, hum = 0;
        ESP_ERROR_CHECK(bmx280_readoutFloat(bmx280, &temp, &pres, &hum));

        pres = pres / 100;

        bmx_data.humidity += hum;
        bmx_data.pressure += pres;
        bmx_data.temperature += temp;

        ESP_LOGI(TAG, "Read Values: temp = %f, pres = %f, hum = %f", temp, pres, hum);
    }

    bmx_data.humidity = bmx_data.humidity / num_retry;
    bmx_data.pressure = bmx_data.pressure / num_retry;
    bmx_data.temperature = bmx_data.temperature / num_retry;

    return bmx_data;
}

double bm() 
{
    bm_sense();

    return 0.0;
}

bool gps_correct_update(gps_t *gps) 
{
    return gps->fix != GPS_FIX_INVALID
        ||
        (gps->latitude != 0 && gps->longitude != 0 && gps->altitude != 0);
}



void gps_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    gps_t *gps = NULL;
    switch (event_id) {
    case GPS_UPDATE:
        gps = (gps_t *)event_data;
        /* print information parsed from GPS statements */
        // ESP_LOGW(TAG, "Known statement:%s", (char *)event_data);
        ESP_LOGI(TAG, "%d/%d/%d %d:%d:%d => \r\n"
                 "\t\t\t\t\t\tlatitude   = %.05f°N\r\n"
                 "\t\t\t\t\t\tlongitude = %.05f°E\r\n"
                 "\t\t\t\t\t\taltitude   = %.02fm\r\n"
                 "\t\t\t\t\t\tspeed      = %fm/s",
                 gps->date.year + YEAR_BASE, gps->date.month, gps->date.day,
                 gps->tim.hour, gps->tim.minute, gps->tim.second,
                 gps->latitude, gps->longitude, gps->altitude, gps->speed);

        // char* str_json = create_json;
        // ESP_LOGI(TAG, "Json result %s", str_json);
        // parse_json(str_json);

        char date[100];
        char date_title[50];
        snprintf(date, sizeof(date), "%d-%d-%d %d:%d:%d", gps->date.year + YEAR_BASE, gps->date.month, gps->date.day, gps->tim.hour, gps->tim.minute, gps->tim.second);
        snprintf(date_title, sizeof(date_title), "%d-%d-%d.%d-%d", gps->date.year + YEAR_BASE, gps->date.month, gps->date.day, gps->tim.hour, gps->tim.minute);
        // snprintf(date_title, sizeof(date_title), "%d-%d-%d", gps->date.year + YEAR_BASE, gps->date.month, gps->date.day);

        struct Measurement_structure measurement;
        measurement.date = date;
        measurement.latitude = gps->latitude;
        measurement.longtitude = gps->longitude;
        measurement.altitude = gps->altitude;

        if (gps_correct_update(gps)) {
            ESP_LOGI(TAG, "Gps has fix, sensing");
            

            if (bm_init() != 0) { measurement_failure(); break; }
            struct Bmx280_data bmp_data = bm_sense();
            measurement.humidity = bmp_data.humidity;
            measurement.temperature = bmp_data.temperature;
            measurement.pressure = bmp_data.pressure;

            // bm_deinit();

            if (pm_init() != 0) { measurement_failure(); break; }
            vTaskDelay(1000/portTICK_PERIOD_MS);
            measurement.dust = pm_sense(); 

            save_json_measurement(measurement, date_title);
            ESP_LOGI(TAG, "Successfull");
            measurement_successfull();
        }
        else {
            ESP_LOGE(TAG, "Gps no fix");
            measurement_failure();
        }
        
        break;
    case GPS_UNKNOWN:
        /* print unknown statements */
        ESP_LOGW(TAG, "Unknown statement:%s", (char *)event_data);
        break;
    default:
        break;
    }
}