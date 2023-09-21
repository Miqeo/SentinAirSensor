#include "/Users/mhecka/Developer/esp/esp-idf/components/json/cjson/cJSON.h"
#include "esp_log.h"
#include <string.h>
#include <math.h>

#define MAX_ARR_LEN 24 * 60 / 2

#ifndef JSON_Struct
#define JSON_Struct

struct Measurement_structure
{
    char *date;
    double humidity;
    double temperature;
    double pressure;
    double dust;
    double latitude;
    double longtitude;
    double altitude;
};

#endif

char *create_json(struct Measurement_structure measurement_struct);