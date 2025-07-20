/** 
 * Copyright (C) 2017 James Sleeman
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a 
 * copy of this software and associated documentation files (the "Software"), 
 * to deal in the Software without restriction, including without limitation 
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN 
 * THE SOFTWARE.
 * 
 * @author James Sleeman, http://sparks.gogo.co.nz/
 * @license MIT License
 * @see http://ww1.microchip.com/downloads/en/DeviceDoc/11195c.pdf
 */


#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <stdbool.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_log.h"

#include "AD840X.h"

#define TAG "AD840X"

// SPI stuff
#if CONFIG_SPI1_HOST
#define HOST_ID SPI1_HOST
#elif CONFIG_SPI2_HOST
#define HOST_ID SPI2_HOST
#elif CONFIG_SPI3_HOST
#define HOST_ID SPI3_HOST
#else 
#define HOST_ID SPI2_HOST
#endif

static const int SPI_Frequency = SPI_MASTER_FREQ_10M;

/** Begin communication to the potentiometer using the supplied pin as it's 
 *	chipSelect.
 */

void AD840X_begin(MCP_t * dev, int16_t GPIO_MOSI, int16_t GPIO_SCLK, int16_t GPIO_CS, float totalKOhms)
{
	esp_err_t ret;

	ESP_LOGI(TAG, "GPIO_CS=%d",GPIO_CS);
	ESP_LOGI(TAG, "GPIO_MOSI=%d",GPIO_MOSI);
	ESP_LOGI(TAG, "GPIO_SCLK=%d",GPIO_SCLK);

	//gpio_pad_select_gpio( GPIO_CS );
	gpio_reset_pin( GPIO_CS );
	gpio_set_direction( GPIO_CS, GPIO_MODE_OUTPUT );
	gpio_set_level( GPIO_CS, 1 );

	spi_bus_config_t buscfg = {
		.mosi_io_num = GPIO_MOSI,
		.miso_io_num = -1,
		.sclk_io_num = GPIO_SCLK,
		.quadwp_io_num = -1,
		.quadhd_io_num = -1,
		.max_transfer_sz = 0,
		.flags = 0
	};

	ret = spi_bus_initialize( HOST_ID, &buscfg, SPI_DMA_CH_AUTO );
	ESP_LOGD(TAG, "spi_bus_initialize=%d",ret);
	assert(ret==ESP_OK);

	spi_device_interface_config_t devcfg;
	memset(&devcfg, 0, sizeof(devcfg));
	devcfg.clock_speed_hz = SPI_Frequency;
	devcfg.queue_size = 7;
	//devcfg.mode = 2;
	devcfg.mode = 0;
	devcfg.flags = SPI_DEVICE_NO_DUMMY;
	devcfg.spics_io_num = GPIO_CS;
	
	spi_device_handle_t handle;
	ret = spi_bus_add_device( HOST_ID, &devcfg, &handle);
	ESP_LOGD(TAG, "spi_bus_add_device=%d",ret);
	assert(ret==ESP_OK);
	dev->_chipSelectPin = GPIO_CS;
	dev->_handle = handle;
	dev->_totalKOhms = totalKOhms;
}

/** Set the wiper position of the potentiometer, to a position between 0 and 255.
 */

esp_err_t AD840X_setWiper(MCP_t * dev,  uint8_t value, uint8_t potentiometer, uint8_t channels)
{
	uint8_t data[2];
	if (potentiometer == METER_ALL) {
		for (uint8_t pm = 0; pm < channels; pm++ ){
			data[0] = pm;
			data[1] = value;
			//digitalWrite(_chipSelectPin, LOW);
			spi_transaction_t SPITransaction;
			esp_err_t ret;
			memset( &SPITransaction, 0, sizeof( spi_transaction_t ) );
			SPITransaction.length = 2 * 8;
			SPITransaction.tx_buffer = data;
			#if 1
				ret = spi_device_transmit( dev->_handle, &SPITransaction );
			#else
				ret = spi_device_polling_transmit( dev->_handle, &SPITransaction );
			#endif
				assert(ret==ESP_OK); 
		}
	}
	else{
		if (potentiometer == METER_0) {
			data[0] = 0; // Command executed on Potentiometer 0.
		} else if (potentiometer == METER_1) {
			data[0] = 1; // Command executed on Potentiometer 1.
		} else if (potentiometer == METER_2) {
			data[0] = 2; // Command executed on Potentiometer 2.
		} else if (potentiometer == METER_3) {
			data[0] = 3; // Command executed on Potentiometer 3.
		} 
		data[1] = value;
		//digitalWrite(_chipSelectPin, LOW);
		spi_transaction_t SPITransaction;
		esp_err_t ret;
		memset( &SPITransaction, 0, sizeof( spi_transaction_t ) );
		SPITransaction.length = 2 * 8;
		SPITransaction.tx_buffer = data;
		#if 1
			ret = spi_device_transmit( dev->_handle, &SPITransaction );
		#else
			ret = spi_device_polling_transmit( dev->_handle, &SPITransaction );
		#endif
			assert(ret==ESP_OK); 
	}
	//digitalWrite(_chipSelectPin, HIGH);
	return true;
}

/** Get the current resistance value.
 */

float AD840X_getK(MCP_t *dev,  uint8_t value)
{
	return (dev->_totalKOhms/0xFF) * value;
}

