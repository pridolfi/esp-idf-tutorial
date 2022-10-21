idf_component_register(SRCS "app_main.c" "wifi.c"
                    INCLUDE_DIRS "."
                    EMBED_TXTFILES esp32.crt esp32.key server.crt)
                    
