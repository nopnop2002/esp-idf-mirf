/*	Mirf Example

	This example code is in the Public Domain (or CC0 licensed, at your option.)

	Unless required by applicable law or agreed to in writing, this
	software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
	CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "driver/gpio.h"

#include "mirf.h"

static QueueHandle_t gpio_evt_queue = NULL;

#define GPIO_INPUT_PIN_SEL (1ULL<<CONFIG_IRQ_GPIO)
#define ESP_INTR_FLAG_DEFAULT 0

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

// GPIO interrupt handler
static void IRAM_ATTR gpio_isr_handler(void* arg)
{
	uint32_t gpio_num = (uint32_t) arg;
	xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}


#if CONFIG_RECEIVER
void receiver(void *pvParameters)
{
	ESP_LOGI(pcTaskGetName(0), "Start");
	NRF24_t dev;
	Nrf24_init(&dev);
	uint8_t payload = 32;
	uint8_t channel = CONFIG_RADIO_CHANNEL;
	Nrf24_config(&dev, channel, payload);

	// Set my own address using 5 characters
	esp_err_t ret = Nrf24_setRADDR(&dev, (uint8_t *)"FGHIJ");
	if (ret != ESP_OK) {
		ESP_LOGE(pcTaskGetName(0), "nrf24l01 not installed");
		while(1) { vTaskDelay(1); }
	}

#if CONFIG_ADVANCED
	AdvancedSettings(&dev);
#endif // CONFIG_ADVANCED

	// Print settings
	Nrf24_printDetails(&dev);
	ESP_LOGI(pcTaskGetName(0), "Listening...");

    uint8_t buf[32];

    // Clear RX FiFo
    while(1) {
        if (Nrf24_dataReady(&dev) == false) break;
        Nrf24_getData(&dev, buf);
    }

	uint32_t io_num;

	while(1) {
		// Wait for assertion of RX receive complete(RX_DR)
		if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
			ESP_LOGD(pcTaskGetName(0), "GPIO[%"PRIu32"] intr, val: %d", io_num, gpio_get_level(io_num));
			Nrf24_getData(&dev, buf);
			ESP_LOGI(pcTaskGetName(0), "Got data:%s", buf);
		}
	}
}
#endif // CONFIG_RECEIVER


#if CONFIG_SENDER
void sender(void *pvParameters)
{
	ESP_LOGI(pcTaskGetName(0), "Start");
	NRF24_t dev;
	Nrf24_init(&dev);
	uint8_t payload = 32;
	uint8_t channel = CONFIG_RADIO_CHANNEL;
	Nrf24_config(&dev, channel, payload);

	// Set destination address using 5 characters
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

	uint8_t buf[32];
	uint32_t io_num;
	while(1) {
		TickType_t nowTick = xTaskGetTickCount();
		sprintf((char *)buf, "Hello World %"PRIu32, nowTick);
		Nrf24_send(&dev, buf);
		ESP_LOGI(pcTaskGetName(0), "Wait for sending.....");
		// Wait for assertion of TX transmit retry over(MAX_RT)
		if(xQueueReceive(gpio_evt_queue, &io_num, 1000/portTICK_PERIOD_MS)) {
			ESP_LOGW(pcTaskGetName(0),"Send fail:");

		// Assert does not occur after successful transmission
		} else {
			ESP_LOGD(pcTaskGetName(0), "GPIO[%"PRIu32"] intr, val: %d", io_num, gpio_get_level(io_num));
			ESP_LOGI(pcTaskGetName(0),"Send success:%s", buf);
		}
		vTaskDelay(1000/portTICK_PERIOD_MS);
	}
}
#endif // CONFIG_SENDER


void app_main(void)
{
	// Initialize gpio
	//zero-initialize the config structure.
	gpio_config_t io_conf = {};
	//interrupt of falling edge
	io_conf.intr_type = GPIO_INTR_NEGEDGE;
	//bit mask of the pins, use GPIO4/5 here
	io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
	//set as input mode
	io_conf.mode = GPIO_MODE_INPUT;
	//enable pull-up mode
	io_conf.pull_up_en = 1;
	gpio_config(&io_conf);
	//install gpio isr service
	gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
	//hook isr handler for specific gpio pin
	gpio_isr_handler_add(CONFIG_IRQ_GPIO, gpio_isr_handler, (void*) CONFIG_IRQ_GPIO);
	//create a queue to handle gpio event from isr
	gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));


#if CONFIG_RECEIVER
	xTaskCreate(&receiver, "RECEIVER", 1024*3, NULL, 2, NULL);
#endif

#if CONFIG_SENDER
	xTaskCreate(&sender, "SENDER", 1024*3, NULL, 2, NULL);
#endif

}
