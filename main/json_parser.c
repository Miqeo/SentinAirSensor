#include "json_parser.h"

#define TAG "json_stack"


char *create_json(struct Measurement_structure measurement_struct)
{

    char *string = NULL;

    cJSON *measurement = cJSON_CreateObject();
    if (measurement == NULL)
    {
        goto end;
    }

    if (cJSON_AddStringToObject(measurement, "date", measurement_struct.date) == NULL)
    {
        goto end;
    }

    if (cJSON_AddNumberToObject(measurement, "humidity", measurement_struct.humidity) == NULL)
    {
        goto end;
    }

    if (cJSON_AddNumberToObject(measurement, "temperature", measurement_struct.temperature) == NULL)
    {
        goto end;
    }

    if (cJSON_AddNumberToObject(measurement, "pressure", measurement_struct.pressure) == NULL)
    {
        goto end;
    }

    if (cJSON_AddNumberToObject(measurement, "dust", measurement_struct.dust) == NULL)
    {
        goto end;
    }

    if (cJSON_AddNumberToObject(measurement, "lat", measurement_struct.latitude) == NULL)
    {
        goto end;
    }

    if (cJSON_AddNumberToObject(measurement, "lon", measurement_struct.longtitude) == NULL)
    {
        goto end;
    }

    if (cJSON_AddNumberToObject(measurement, "alt", measurement_struct.altitude) == NULL)
    {
        goto end;
    }

    // measurements = cJSON_CreateArray();
    // if (measurements == NULL)
    // {
    //     goto end;
    // }
    // cJSON_AddItemToObject(monitor, "measurements", measurements);

    // for (index = 0; index < num_of_current; index++)  //sizeof(measurements_list) / sizeof(struct Measurement_structure); index++)
    // {
    //     cJSON *measurement = cJSON_CreateObject();

    //     if (cJSON_AddStringToObject(measurement, "date", measurements_list[index].date) == NULL)
    //     {
    //         goto end;
    //     }

    //     if (cJSON_AddNumberToObject(measurement, "humidity", measurements_list[index].humidity) == NULL)
    //     {
    //         goto end;
    //     }

    //     if (cJSON_AddNumberToObject(measurement, "temperature", measurements_list[index].temperature) == NULL)
    //     {
    //         goto end;
    //     }

    //     if (cJSON_AddNumberToObject(measurement, "pressure", measurements_list[index].pressure) == NULL)
    //     {
    //         goto end;
    //     }

    //     if (cJSON_AddNumberToObject(measurement, "dust", measurements_list[index].dust) == NULL)
    //     {
    //         goto end;
    //     }

    //     if (cJSON_AddNumberToObject(measurement, "lat", measurements_list[index].latitude) == NULL)
    //     {
    //         goto end;
    //     }

    //     if (cJSON_AddNumberToObject(measurement, "lon", measurements_list[index].longtitude) == NULL)
    //     {
    //         goto end;
    //     }

    //     if (cJSON_AddNumberToObject(measurement, "alt", measurements_list[index].altitude) == NULL)
    //     {
    //         goto end;
    //     }

    //     cJSON_AddItemToArray(measurements, measurement);
    // }

    string = cJSON_Print(measurement);
    if (string == NULL)
    {
        ESP_LOGE(TAG, "Failed to print monitor.");
    }

end:
    cJSON_Delete(measurement);
    return string;
}

// void parse_json(const char* value, struct Measurement_structure* measurement_to_parse)
// {
//     // const cJSON *measurements = NULL;
//     const cJSON *measurement = NULL;

//     cJSON *monitor_json = cJSON_Parse(value);

//     if (monitor_json == NULL)
//     {
//         const char *error_ptr = cJSON_GetErrorPtr();
//         if (error_ptr != NULL)
//         {
//             ESP_LOGW(TAG, "Empty json before: %s, for value: %s", error_ptr, value);
//         }
//         return;
//         // return NULL;
//     }
    
//     // measurements = cJSON_GetObjectItemCaseSensitive(monitor_json, "measurements");
//     measurement = cJSON_GetObjectItemCaseSensitive(monitor_json, "measurements");


//     cJSON *date = cJSON_GetObjectItemCaseSensitive(measurement, "date");
//     cJSON *humidity = cJSON_GetObjectItemCaseSensitive(measurement, "humidity");
//     cJSON *temperature = cJSON_GetObjectItemCaseSensitive(measurement, "temperature");
//     cJSON *pressure = cJSON_GetObjectItemCaseSensitive(measurement, "pressure");
//     cJSON *dust = cJSON_GetObjectItemCaseSensitive(measurement, "dust");
//     cJSON *latitude = cJSON_GetObjectItemCaseSensitive(measurement, "lat");
//     cJSON *longtitude = cJSON_GetObjectItemCaseSensitive(measurement, "lon");
//     cJSON *altitude = cJSON_GetObjectItemCaseSensitive(measurement, "alt");

//     if (!cJSON_IsNumber(humidity) || !cJSON_IsNumber(temperature) || !cJSON_IsNumber(pressure) || !cJSON_IsNumber(latitude) || !cJSON_IsNumber(longtitude) || !cJSON_IsNumber(altitude) || !cJSON_IsString(date))
//     {
//         ESP_LOGW(TAG, "parse_json json object with wrong data type");
//         return;
//     }
//     struct Measurement_structure measurement_struct;

//     measurement_struct.date = date->valuestring;
//     measurement_struct.humidity = humidity->valuedouble;
//     measurement_struct.temperature = temperature->valuedouble;
//     measurement_struct.pressure = pressure->valuedouble;
//     measurement_struct.dust = dust->valuedouble;
//     measurement_struct.latitude = latitude->valuedouble;
//     measurement_struct.longtitude = longtitude->valuedouble;
//     measurement_struct.altitude = altitude->valuedouble;

//     measurement_to_parse = measurement_struct;

//     ESP_LOGI(TAG, "date: %s, humidity: %f, temperature: %f, pressure: %f, dust: %f, lat: %f, lon: %f, alt: %f",
//             measurement_struct.date, measurement_struct.humidity, measurement_struct.temperature, measurement_struct.pressure, measurement_struct.dust, measurement_struct.latitude, measurement_struct.longtitude, measurement_struct.altitude);
//     // int actual_arr_size = cJSON_GetArraySize(measurements);

//     // ESP_LOGI(TAG, "parse_json json array size: %d, max size: %d", actual_arr_size, arr_size);
//     // struct Measurement_structure measurements_list[arr_size];

//     // int iterate = 0;

//     // cJSON_ArrayForEach(measurement, measurements)
//     // {   
//     //     // if (iterate >= arr_size) { break; }
        
//     //     cJSON *date = cJSON_GetObjectItemCaseSensitive(measurement, "date");
//     //     cJSON *humidity = cJSON_GetObjectItemCaseSensitive(measurement, "humidity");
//     //     cJSON *temperature = cJSON_GetObjectItemCaseSensitive(measurement, "temperature");
//     //     cJSON *pressure = cJSON_GetObjectItemCaseSensitive(measurement, "pressure");
//     //     cJSON *dust = cJSON_GetObjectItemCaseSensitive(measurement, "dust");
//     //     cJSON *latitude = cJSON_GetObjectItemCaseSensitive(measurement, "lat");
//     //     cJSON *longtitude = cJSON_GetObjectItemCaseSensitive(measurement, "lon");
//     //     cJSON *altitude = cJSON_GetObjectItemCaseSensitive(measurement, "alt");

//     //     if (!cJSON_IsNumber(humidity) || !cJSON_IsNumber(temperature) || !cJSON_IsNumber(pressure) || !cJSON_IsNumber(latitude) || !cJSON_IsNumber(longtitude) || !cJSON_IsNumber(altitude) || !cJSON_IsString(date))
//     //     {
//     //         ESP_LOGW(TAG, "parse_json json object with wrong data type");
//     //         continue;
//     //     }
//     //     struct Measurement_structure measurement;

//     //     measurement.date = date->valuestring;
//     //     measurement.humidity = humidity->valuedouble;
//     //     measurement.temperature = temperature->valuedouble;
//     //     measurement.pressure = pressure->valuedouble;
//     //     measurement.dust = dust->valuedouble;
//     //     measurement.latitude = latitude->valuedouble;
//     //     measurement.longtitude = longtitude->valuedouble;
//     //     measurement.altitude = altitude->valuedouble;

//     //     measurements_list[iterate] = measurement;

//     //     ESP_LOGI(TAG, "parse_json adding measurement %d %s", iterate, measurements_list[iterate].date);
//     //     iterate += 1;
//     // }

//     // for(int y = 0; y < sizeof(measurements_list) / sizeof(struct Measurement_structure); y++ )
//     // {
//     //     ESP_LOGI(TAG, "date: %s, humidity: %f, temperature: %f, pressure: %f, dust: %f, lat: %f, lon: %f, alt: %f",
//     //         measurements_list[y].date, measurements_list[y].humidity, measurements_list[y].temperature, measurements_list[y].pressure, measurements_list[y].dust, measurements_list[y].latitude, measurements_list[y].longtitude, measurements_list[y].altitude);
//     // }
// }