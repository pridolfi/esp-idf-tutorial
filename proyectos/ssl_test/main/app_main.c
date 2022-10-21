/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_err.h"
#include "nvs_flash.h"
#include "esp_tls.h"
#include "esp_log.h"

#include "wifi.h"

static const char *TAG = "app_main";

extern const uint8_t esp32_crt_start[] asm("_binary_esp32_crt_start");
extern const uint8_t esp32_crt_end[]   asm("_binary_esp32_crt_end");

extern const uint8_t esp32_key_start[] asm("_binary_esp32_key_start");
extern const uint8_t esp32_key_end[]   asm("_binary_esp32_key_end");

extern const uint8_t server_crt_start[] asm("_binary_server_crt_start");
extern const uint8_t server_crt_end[]   asm("_binary_server_crt_end");

void app_main(void)
{
    printf("ESP32 TLS/SSL connection example.\n");

    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU core(s), WiFi%s%s, ",
            CONFIG_IDF_TARGET,
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    printf("Minimum free heap size: %d bytes\n", esp_get_minimum_free_heap_size());

    //Initialize NVS, needed for WiFi
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    wifi_init_sta();

    esp_tls_cfg_t cfg = {
        .cacert_buf = (const unsigned char *) server_crt_start,
        .cacert_bytes = server_crt_end - server_crt_start,
        .common_name = "server",
        .clientcert_buf = (const unsigned char *) esp32_crt_start,
        .clientcert_bytes = esp32_crt_end - esp32_crt_start,
        .clientkey_buf = (const unsigned char *) esp32_key_start,
        .clientkey_bytes = esp32_key_end - esp32_key_start,
        .timeout_ms = 10000,
    };

    esp_tls_t * tls = esp_tls_conn_http_new("https://192.168.1.14:8443", &cfg);

    if (tls != NULL) {
        ESP_LOGI(TAG, "Connection established...");
        esp_tls_conn_write(tls, "Hello from ESP32!\n", 18);
        vTaskDelay(1000/portTICK_PERIOD_MS);
        uint8_t buffer[256];
        bzero(buffer, 256);
        esp_tls_conn_read(tls, buffer, 256);
        ESP_LOGI(TAG, "Server says: %s", buffer);
    } else {
        ESP_LOGE(TAG, "Connection failed...");
    }
}
