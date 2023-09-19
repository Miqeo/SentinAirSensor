idf_component_register(SRCS "main.c" "server_stack.c" "mount.c" "wifi_stack.c" "bmx280.c" "sensors.c" "nmea_parser.c" "sleep.c" "nvs_data.c" "json_parser.c"
                       INCLUDE_DIRS "../../../esp-idf/components/json/cjson"
                       EMBED_FILES root.html upload_script.html favicon.ico)
