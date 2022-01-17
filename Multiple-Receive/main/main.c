/* Mirf Example

	 This example code is in the Public Domain (or CC0 licensed, at your option.)

	 Unless required by applicable law or agreed to in writing, this
	 software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
	 CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "mirf.h"

typedef union {
	uint8_t value[2];
	unsigned int ivalue;
} MYDATA_t;

MYDATA_t mydata;

void receiver(void *pvParameters)
{
	NRF24_t dev;

	ESP_LOGI(pcTaskGetTaskName(0), "Start");
	ESP_LOGI(pcTaskGetTaskName(0), "CONFIG_CE_GPIO=%d",CONFIG_CE_GPIO);
	ESP_LOGI(pcTaskGetTaskName(0), "CONFIG_CSN_GPIO=%d",CONFIG_CSN_GPIO);
	spi_master_init(&dev, CONFIG_CE_GPIO, CONFIG_CSN_GPIO, CONFIG_MISO_GPIO, CONFIG_MOSI_GPIO, CONFIG_SCLK_GPIO);

	uint8_t payload = sizeof(mydata.value);
	uint8_t channel = 90;
	Nrf24_config(&dev, channel, payload);

	//Set your own address using 5 characters
	Nrf24_setRADDR(&dev, (uint8_t *)"1RECV");

  //Add your own address using 1 characters
  Nrf24_addRADDR(&dev, 2, '2'); // 2RECV
  Nrf24_addRADDR(&dev, 3, '3'); // 3RECV
  Nrf24_addRADDR(&dev, 4, '4'); // 4RECV
  Nrf24_addRADDR(&dev, 5, '5'); // 5RECV

	//Print settings
	Nrf24_printDetails(&dev);
	ESP_LOGI(pcTaskGetTaskName(0), "Listening...");

	while(1) {
		//When the program is received, the received data is output from the serial port
		if (Nrf24_dataReady(&dev)) {
			uint8_t pipe = Nrf24_getDataPipe(&dev);
			Nrf24_getData(&dev, mydata.value);
			ESP_LOGI(pcTaskGetTaskName(0), "Got data pipe(%d):%d", pipe, mydata.ivalue);
		}
		vTaskDelay(1);
	}
}

void app_main(void)
{
	xTaskCreate(receiver, "RECEIVER", 1024*2, NULL, 2, NULL);
}
