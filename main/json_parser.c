#include "json_parser.h"

#define TAG "json_stack"

double round_number(double val, int place)
{
    return (double)((int)(val * pow(10, place)) / pow(10.0, place));
}


char *create_json(struct Measurement_structure measurement_struct)
{

    char *string = NULL;

    cJSON *geometry = NULL;
    cJSON *coordinates = NULL;
    cJSON *latitude = NULL;
    cJSON *longtitude = NULL;

    cJSON *properties = NULL;

    cJSON *measurement = cJSON_CreateObject();

    cJSON *features = NULL;

    cJSON *feature_collection = cJSON_CreateObject();


    if (measurement == NULL || feature_collection == NULL)
    {
        goto end;
    }

    if (cJSON_AddStringToObject(measurement, "type", "Feature") == NULL)
    {
        goto end;
    }

    if (cJSON_AddStringToObject(feature_collection, "type", "FeatureCollection") == NULL)
    {
        goto end;
    }

    geometry = cJSON_CreateObject();
    if (geometry == NULL)
    {
        goto end;
    }

    

    if (cJSON_AddStringToObject(geometry, "type", "Point") == NULL)
    {
        goto end;
    }

    coordinates = cJSON_CreateArray();
    if (coordinates == NULL)
    {
        goto end;
    }

    longtitude = cJSON_CreateNumber(round_number(measurement_struct.longtitude, 3));
    if (longtitude == NULL) {
        goto end;
    }
    cJSON_AddItemToArray(coordinates, longtitude);

    latitude = cJSON_CreateNumber(round_number(measurement_struct.latitude, 3));
    if (latitude == NULL) {
        goto end;
    }
    cJSON_AddItemToArray(coordinates, latitude);

    cJSON_AddItemToObject(geometry, "coordinates", coordinates);
    
    cJSON_AddItemToObject(measurement, "geometry", geometry);

    properties = cJSON_CreateObject();
    if (properties == NULL)
    {
        goto end;
    }

    cJSON_AddItemToObject(measurement, "properties", properties);

    if (cJSON_AddStringToObject(properties, "date", measurement_struct.date) == NULL)
    {
        goto end;
    }

    if (cJSON_AddNumberToObject(properties, "humidity", round_number(measurement_struct.humidity, 2)) == NULL)
    {
        goto end;
    }

    if (cJSON_AddNumberToObject(properties, "temperature", round_number(measurement_struct.temperature, 2)) == NULL)
    {
        goto end;
    }

    if (cJSON_AddNumberToObject(properties, "pressure", round_number(measurement_struct.pressure, 2)) == NULL)
    {
        goto end;
    }

    if (cJSON_AddNumberToObject(properties, "dust_voltage", round_number(measurement_struct.dust, 4)) == NULL)
    {
        goto end;
    }

    if (cJSON_AddNumberToObject(properties, "altitude", round_number(measurement_struct.altitude, 1)) == NULL)
    {
        goto end;
    }

    features = cJSON_CreateArray();
    if (features == NULL)
    {
        goto end;
    }

    cJSON_AddItemToArray(features, measurement);

    
    cJSON_AddItemToObject(feature_collection, "features", features);


    string = cJSON_Print(feature_collection);
    if (string == NULL)
    {
        ESP_LOGE(TAG, "Failed to print monitor.");
    }

end:
    cJSON_Delete(measurement);
    return string;
}
