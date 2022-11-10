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
	uint8_t value[4];
	unsigned long now_time;
} MYDATA_t;

MYDATA_t mydata;

#if CONFIG_ADVANCED
void AdvancedSettings(NRF24_t * dev)
{
#if CONFIG_RF_RATIO_2M
	ESP_LOGW(pcTaskGetName(0), "Set RF Data Ratio to 2MBps");
	Nrf24_SetSpeedDataRates(dev, 1);
#endif // CONFIG_RF_RATIO_2M

#if CONFIG_RF_RATIO_1M
	ESP_LOGW(pcTaskGetName(0), "Set RF Data Ratio to 1MBps");
	Nrf24_SetSpeedDataRates(dev, 0);
#endif // CONFIG_RF_RATIO_2M

#if CONFIG_RF_RATIO_250K
	ESP_LOGW(pcTaskGetName(0), "Set RF Data Ratio to 250KBps");
	Nrf24_SetSpeedDataRates(dev, 2);
#endif // CONFIG_RF_RATIO_2M

	ESP_LOGW(pcTaskGetName(0), "CONFIG_RETRANSMIT_DELAY=%d", CONFIG_RETRANSMIT_DELAY);
	Nrf24_setRetransmitDelay(dev, CONFIG_RETRANSMIT_DELAY);
}
#endif // CONFIG_ADVANCED

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
	esp_err_t ret = Nrf24_setRADDR(&dev, (uint8_t *)"FGHIJ");
	if (ret != ESP_OK) {
		ESP_LOGE(pcTaskGetName(0), "nrf24l01 not installed");
		while(1) { vTaskDelay(1); }
	}

#if CONFIG_ADVANCED
	AdvancedSettings(&dev);
#endif // CONFIG_ADVANCED

	//Print settings
	Nrf24_printDetails(&dev);
	ESP_LOGI(pcTaskGetName(0), "Listening...");

	while(1) {
		//When the program is received, the received data is output from the serial port
		if (Nrf24_dataReady(&dev)) {
			Nrf24_getData(&dev, mydata.value);
			ESP_LOGI(pcTaskGetName(0), "Got data:%lu", mydata.now_time);
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
	esp_err_t ret = Nrf24_setTADDR(&dev, (uint8_t *)"FGHIJ");
	if (ret != ESP_OK) {
		ESP_LOGE(pcTaskGetName(0), "nrf24l01 not installed");
		while(1) { vTaskDelay(1); }
	}

#if CONFIG_ADVANCED
	AdvancedSettings(&dev);
#endif // CONFIG_ADVANCED

	//Print settings
	Nrf24_printDetails(&dev);

	while(1) {
		mydata.now_time = xTaskGetTickCount();
		Nrf24_send(&dev, mydata.value);
		vTaskDelay(1);
		ESP_LOGI(pcTaskGetName(0), "Wait for sending.....");
		if (Nrf24_isSend(&dev, 1000)) {
			ESP_LOGI(pcTaskGetName(0),"Send success:%lu", mydata.now_time);
		} else {
			ESP_LOGW(pcTaskGetName(0),"Send fail:");
		}
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
