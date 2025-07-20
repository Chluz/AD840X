#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"
#include "esp_log.h"

#include "AD840X.h"

#define TAG "MAIN"

#define CONFIG_MODEL_AD8402AR1 true
#define CONFIG_MOSI_GPIO 19
#define CONFIG_SCLK_GPIO 18
#define CONFIG_CS_GPIO 5
#define CONFIG_SPI2_HOST true
#define HOST_ID SPI2_HOST

 void app_main()
{
#if CONFIG_MODEL_AD8400AR10
	ESP_LOGI(TAG, "AD8400AR10");
	float total= KOHMS_AD8400AR10;
	int channels = CHANNELS_AD8400AR10;
#elif CONFIG_MODEL_AD8402AR1
	ESP_LOGI(TAG, "AD8402AR1");
	float total= KOHMS_AD8402AR1;
	int channels = CHANNELS_AD8402AR1;
#endif

	ESP_LOGI(TAG, "total=%f KOhms", total);
	ESP_LOGI(TAG, "CONFIG_MOSI_GPIO=%d", CONFIG_MOSI_GPIO);
	ESP_LOGI(TAG, "CONFIG_SCLK_GPIO=%d", CONFIG_SCLK_GPIO);
	ESP_LOGI(TAG, "CONFIG_CS_GPIO=%d", CONFIG_CS_GPIO);

	MCP_t dev;
	AD840X_begin(&dev, CONFIG_MOSI_GPIO, CONFIG_SCLK_GPIO, CONFIG_CS_GPIO, total);

	uint8_t cur_value = 0;
	uint8_t DIRECTION = 1;
	uint8_t potentiometer = METER_ALL;

	while(1) {
		AD840X_setWiper(&dev, cur_value, potentiometer, channels);
		float resistance = AD840X_getK(&dev, cur_value);
		ESP_LOGI(TAG, "cur_value=%d resistance=%f KOhms", cur_value, resistance);
		if(DIRECTION) {
			cur_value++;
			if(cur_value == 0) DIRECTION = 0;
		} else {
			cur_value--;
			if(cur_value == 0) DIRECTION = 1;
		}
		vTaskDelay(20);
	} // end while
}

