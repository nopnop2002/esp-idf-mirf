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

#if CONFIG_RECEIVER
void receiver(void *pvParameters)
{
	ESP_LOGI(pcTaskGetName(0), "Start");
	NRF24_t dev;
	Nrf24_init(&dev);
	uint8_t payload = sizeof(mydata.value);
	uint8_t channel = 90;
	Nrf24_config(&dev, channel, payload);

	//Set own address using 5 characters
	esp_err_t ret = Nrf24_setRADDR(&dev, (uint8_t *)"1RECV");
	if (ret != ESP_OK) {
		ESP_LOGE(pcTaskGetName(0), "nrf24l01 not installed");
		while(1) { vTaskDelay(1); }
	}

	//Add own address using 1 characters
	Nrf24_addRADDR(&dev, 2, '2'); // 2RECV
	Nrf24_addRADDR(&dev, 3, '3'); // 3RECV
	Nrf24_addRADDR(&dev, 4, '4'); // 4RECV
	Nrf24_addRADDR(&dev, 5, '5'); // 5RECV

	//Print settings
	Nrf24_printDetails(&dev);
	ESP_LOGI(pcTaskGetName(0), "Listening...");

	while(1) {
		//When the program is received, the received data is output from the serial port
		if (Nrf24_dataReady(&dev)) {
			uint8_t pipe = Nrf24_getDataPipe(&dev);
			Nrf24_getData(&dev, mydata.value);
			ESP_LOGI(pcTaskGetName(0), "Got data pipe(%d):%d", pipe, mydata.ivalue);
		}
		vTaskDelay(1);
	}
}
#endif // CONFIG_RECEIVER

#if CONFIG_TRANSMITTER
void transmitter(void *pvParameters)
{
	ESP_LOGI(pcTaskGetName(0), "Start");
	NRF24_t dev;
	Nrf24_init(&dev);
	uint8_t payload = sizeof(mydata.value);
	uint8_t channel = 90;
	Nrf24_config(&dev, channel, payload);

	//Set the receiver address using 5 characters
#if CONFIG_TADDR1
	esp_err_t ret = Nrf24_setTADDR(&dev, (uint8_t *)"1RECV");
#elif CONFIG_TADDR2
	esp_err_t ret = Nrf24_setTADDR(&dev, (uint8_t *)"2RECV");
#elif CONFIG_TADDR3
	esp_err_t ret = Nrf24_setTADDR(&dev, (uint8_t *)"3RECV");
#elif CONFIG_TADDR4
	esp_err_t ret = Nrf24_setTADDR(&dev, (uint8_t *)"4RECV");
#elif CONFIG_TADDR5
	esp_err_t ret = Nrf24_setTADDR(&dev, (uint8_t *)"5RECV");
#endif
	if (ret != ESP_OK) {
		ESP_LOGE(pcTaskGetName(0), "nrf24l01 not installed");
		while(1) { vTaskDelay(1); }
	}

	//Print settings
	Nrf24_printDetails(&dev);

	mydata.ivalue = 0;
	while(1) {
		Nrf24_send(&dev, mydata.value);
		vTaskDelay(1);
		ESP_LOGI(pcTaskGetName(0), "Wait for sending.....");
		if (Nrf24_isSend(&dev, 1000)) {
			ESP_LOGI(pcTaskGetName(0),"Send success:%u", mydata.ivalue);
		} else {
			ESP_LOGW(pcTaskGetName(0),"Send fail:");
		}
		mydata.ivalue++;
		vTaskDelay(1000/portTICK_PERIOD_MS);
	}
}
#endif // CONFIG_TRANSMITTER


void app_main(void)
{
#if CONFIG_RECEIVER
	xTaskCreate(receiver, "RECEIVER", 1024*3, NULL, 2, NULL);
#endif

#if CONFIG_TRANSMITTER
    xTaskCreate(transmitter, "TRANSMITTER", 1024*3, NULL, 2, NULL);
#endif
}
