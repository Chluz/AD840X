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
 */

#ifndef AD8400_H
#define AD8400_H

#define KOHMS_AD8400AR10  10
#define KOHMS_AD8402AR1  1
#define CHANNELS_AD8400AR10  1
#define CHANNELS_AD8402AR1  2

enum MCP_POTENTIOMETER {
	METER_0,
	METER_1,
	METER_2,
	METER_3,
	METER_ALL
};

typedef struct {
	uint8_t _chipSelectPin;
	spi_device_handle_t _handle;
	float _totalKOhms;
} MCP_t;

void AD840X_begin(MCP_t * dev, int16_t GPIO_MOSI, int16_t GPIO_SCLK, int16_t GPIO_CS, float totalKOhms);
float AD840X_getK(MCP_t *dev,  uint8_t value);
esp_err_t AD840X_setWiper(MCP_t * dev,  uint8_t value, uint8_t potentiometer, uint8_t channels);
// esp_err_t MCP41_shutdownMode(MCP_t * dev);

#endif
