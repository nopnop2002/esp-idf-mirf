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
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_vfs.h"
#include "nvs_flash.h"
#include "esp_err.h"
#include "esp_log.h"
#include "mdns.h"
#include "netdb.h" // gethostbyname
//#include "lwip/dns.h"

#include "mirf.h"
#include "mqtt.h"

static const char *TAG = "MAIN";

/* FreeRTOS event group to signal when we are connected*/
static EventGroupHandle_t s_wifi_event_group;

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

static int s_retry_num = 0;

QueueHandle_t xQueue_mqtt_tx;
QueueHandle_t xQueue_mqtt_rx;


static void event_handler(void* arg, esp_event_base_t event_base,
								int32_t event_id, void* event_data)
{
	if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
		esp_wifi_connect();
	} else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
		if (s_retry_num < CONFIG_ESP_MAXIMUM_RETRY) {
			esp_wifi_connect();
			s_retry_num++;
			ESP_LOGI(TAG, "retry to connect to the AP");
		} else {
			xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
		}
		ESP_LOGI(TAG,"connect to the AP fail");
	} else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
		ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
		ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
		s_retry_num = 0;
		xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
	}
}


bool wifi_init_sta(void)
{
	bool ret = false;
	s_wifi_event_group = xEventGroupCreate();

	ESP_ERROR_CHECK(esp_netif_init());

	ESP_ERROR_CHECK(esp_event_loop_create_default());
	esp_netif_create_default_wifi_sta();

	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK(esp_wifi_init(&cfg));

	esp_event_handler_instance_t instance_any_id;
	esp_event_handler_instance_t instance_got_ip;
	ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
		ESP_EVENT_ANY_ID,
		&event_handler,
		NULL,
		&instance_any_id));
	ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
		IP_EVENT_STA_GOT_IP,
		&event_handler,
		NULL,
		&instance_got_ip));

	wifi_config_t wifi_config = {
		.sta = {
			.ssid = CONFIG_ESP_WIFI_SSID,
			.password = CONFIG_ESP_WIFI_PASSWORD,
			/* Setting a password implies station will connect to all security modes including WEP/WPA.
			 * However these modes are deprecated and not advisable to be used. Incase your Access point
			 * doesn't support WPA2, these mode can be enabled by commenting below line */
			.threshold.authmode = WIFI_AUTH_WPA2_PSK,

			.pmf_cfg = {
				.capable = true,
				.required = false
			},
		},
	};
	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
	ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
	ESP_ERROR_CHECK(esp_wifi_start() );

	ESP_LOGI(TAG, "wifi_init_sta finished.");

	/* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
	 * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) */
	EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
		WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
		pdFALSE,
		pdFALSE,
		portMAX_DELAY);

	/* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
	 * happened. */
	if (bits & WIFI_CONNECTED_BIT) {
		ESP_LOGI(TAG, "connected to ap SSID:%s password:%s", CONFIG_ESP_WIFI_SSID, CONFIG_ESP_WIFI_PASSWORD);
		ret = true;
	} else if (bits & WIFI_FAIL_BIT) {
		ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s", CONFIG_ESP_WIFI_SSID, CONFIG_ESP_WIFI_PASSWORD);
	} else {
		ESP_LOGE(TAG, "UNEXPECTED EVENT");
	}

	/* The event will not be processed after unregister */
	ESP_ERROR_CHECK(esp_event_handler_instance_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, instance_got_ip));
	ESP_ERROR_CHECK(esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, instance_any_id));
	vEventGroupDelete(s_wifi_event_group);
	return ret;
}


#if CONFIG_RADIO_TO_MQTT
void mirf_receiver(void *pvParameters)
{
	ESP_LOGI(pcTaskGetName(0), "Start");
	NRF24_t dev;
	Nrf24_init(&dev);

	ESP_LOGI(pcTaskGetName(0), "CONFIG_PAYLOAD_SIZE=%d", CONFIG_PAYLOAD_SIZE);
	ESP_LOGI(pcTaskGetName(0), "CONFIG_RECEIVER_ADDRESS=[%s]", CONFIG_RECEIVER_ADDRESS);
	ESP_LOGI(pcTaskGetName(0), "CONFIG_RADIO_CHANNEL=%d", CONFIG_RADIO_CHANNEL);
	uint8_t mydata[CONFIG_PAYLOAD_SIZE];
	uint8_t payload = sizeof(mydata);
	uint8_t channel = CONFIG_RADIO_CHANNEL;
	Nrf24_config(&dev, channel, payload);

	//Set own address
	esp_err_t ret = Nrf24_setRADDR(&dev, (uint8_t *)CONFIG_RECEIVER_ADDRESS);
	if (ret != ESP_OK) {
		ESP_LOGE(pcTaskGetName(0), "nrf24l01 not installed");
		while(1) { vTaskDelay(1); }
	}

	//Print settings
	Nrf24_printDetails(&dev);

	ESP_LOGI(pcTaskGetName(0), "Wait for radio...");
	MQTT_t mqttBuf;
	while(1) {
		if (Nrf24_dataReady(&dev)) {
			Nrf24_getData(&dev, mydata);
			// When the program is received, the received data is output from the serial port
			ESP_LOGI(pcTaskGetName(0), "Got data from nRF24L01");
			ESP_LOG_BUFFER_HEXDUMP(pcTaskGetName(0), mydata, sizeof(mydata), ESP_LOG_INFO);
			mqttBuf.topic_type = PUBLISH;
			strcpy(mqttBuf.topic, CONFIG_MQTT_PUB_TOPIC);
			mqttBuf.topic_len = strlen(mqttBuf.topic);
			mqttBuf.payload_len = sizeof(mydata);
			for(int i=0;i<sizeof(mydata);i++) {
				mqttBuf.payload[i] = mydata[i];
			}
			if (xQueueSend(xQueue_mqtt_tx, &mqttBuf, portMAX_DELAY) != pdPASS) {
				ESP_LOGE(pcTaskGetName(0), "xQueueSend Fail");
			}

		} // end if
		vTaskDelay(1);
	} // end while
}
#endif // CONFIG_RADIO_TO_MQTT


#if CONFIG_MQTT_TO_RADIO
void mirf_transmitter(void *pvParameters)
{
	ESP_LOGI(pcTaskGetName(0), "Start");
	NRF24_t dev;
	Nrf24_init(&dev);

	ESP_LOGI(pcTaskGetName(0), "CONFIG_PAYLOAD_SIZE=%d", CONFIG_PAYLOAD_SIZE);
	ESP_LOGI(pcTaskGetName(0), "CONFIG_DESTINATION_ADDRESS=[%s]", CONFIG_DESTINATION_ADDRESS);
	ESP_LOGI(pcTaskGetName(0), "CONFIG_RADIO_CHANNEL=%d", CONFIG_RADIO_CHANNEL);
	uint8_t mydata[CONFIG_PAYLOAD_SIZE];
	uint8_t payload = sizeof(mydata);
	uint8_t channel = CONFIG_RADIO_CHANNEL;
	Nrf24_config(&dev, channel, payload);

	// Set the receiver address
	//Nrf24_setTADDR(&dev, (uint8_t *)"FGHIJ");
	esp_err_t ret = Nrf24_setTADDR(&dev, (uint8_t *)CONFIG_DESTINATION_ADDRESS);
	if (ret != ESP_OK) {
		ESP_LOGE(pcTaskGetName(0), "nrf24l01 not installed");
		while(1) { vTaskDelay(1); }
	}

	//Print settings
	Nrf24_printDetails(&dev);

	ESP_LOGI(pcTaskGetName(0), "Wait for mqtt...");
	MQTT_t mqttBuf;
	while(1) {
		xQueueReceive(xQueue_mqtt_rx, &mqttBuf, portMAX_DELAY);
		if (mqttBuf.topic_type == SUBSCRIBE) {
			ESP_LOGI(pcTaskGetName(0), "topic=[%s] topic_len=%d", mqttBuf.topic, mqttBuf.topic_len);
			ESP_LOG_BUFFER_HEXDUMP(pcTaskGetName(0), mqttBuf.payload, mqttBuf.payload_len, ESP_LOG_INFO);
			//mydata.payload.now_millis = xTaskGetTickCount() * portTICK_RATE_MS;
			//ESP_LOGI(pcTaskGetName(0), "now_millis=%ld", mydata.payload.now_millis);
			int payload_len = mqttBuf.payload_len;
			//if (mqttBuf.payload_len > 32) payload_len = 32;
			if (mqttBuf.payload_len > CONFIG_PAYLOAD_SIZE) payload_len = CONFIG_PAYLOAD_SIZE;
			memset(mydata, 0, sizeof(mydata));
			for(int i=0;i<payload_len;i++) {
				mydata[i] = mqttBuf.payload[i];
			}
			Nrf24_send(&dev, mydata);

			ESP_LOGI(pcTaskGetName(0), "Wait for sending.....");
			if (Nrf24_isSend(&dev, 1000)) {
				ESP_LOGI(pcTaskGetName(0),"Send success");
			} else {
				ESP_LOGW(pcTaskGetName(0),"Send fail");
			}

		} // end if
	} // end while
}
#endif // CONFIG_MQTT_TO_RADIO

esp_err_t query_mdns_host(const char * host_name, char *ip)
{
	ESP_LOGD(__FUNCTION__, "Query A: [%s]", host_name);

	struct esp_ip4_addr addr;
	addr.addr = 0;

	esp_err_t err = mdns_query_a(host_name, 10000,	&addr);
	if(err){
		if(err == ESP_ERR_NOT_FOUND){
			ESP_LOGW(__FUNCTION__, "%s: Host was not found!", esp_err_to_name(err));
			return ESP_FAIL;
		}
		ESP_LOGE(__FUNCTION__, "Query Failed: %s", esp_err_to_name(err));
		return ESP_FAIL;
	}

	ESP_LOGD(__FUNCTION__, "Query A: %s.local resolved to: " IPSTR, host_name, IP2STR(&addr));
	sprintf(ip, IPSTR, IP2STR(&addr));
	return ESP_OK;
}

void convert_mdns_host(char * from, char * to)
{
	ESP_LOGI(__FUNCTION__, "from=[%s]",from);
	strcpy(to, from);
	char *sp;
	sp = strstr(from, ".local");
	if (sp == NULL) return;

	int _len = sp - from;
	ESP_LOGD(__FUNCTION__, "_len=%d", _len);
	char _from[128];
	strcpy(_from, from);
	_from[_len] = 0;
	ESP_LOGI(__FUNCTION__, "_from=[%s]", _from);

	char _ip[128];
	esp_err_t ret = query_mdns_host(_from, _ip);
	ESP_LOGI(__FUNCTION__, "query_mdns_host=%d _ip=[%s]", ret, _ip);
	if (ret != ESP_OK) return;

	strcpy(to, _ip);
	ESP_LOGI(__FUNCTION__, "to=[%s]", to);
}

void mqtt_pub_task(void *pvParameters);
void mqtt_sub_task(void *pvParameters);
	
void app_main(void)
{
	// Initialize NVS
	esp_err_t ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}
	ESP_ERROR_CHECK(ret);

	// Initialize WiFi
	if (wifi_init_sta() == false) {
		while(1) vTaskDelay(10);
	}

	// Initialize mDNS
	ESP_ERROR_CHECK( mdns_init() );

	// Create Queue
	xQueue_mqtt_tx = xQueueCreate( 10, sizeof(MQTT_t) );
	configASSERT( xQueue_mqtt_tx );
	xQueue_mqtt_rx = xQueueCreate( 10, sizeof(MQTT_t) );
	configASSERT( xQueue_mqtt_rx );

#if CONFIG_RADIO_TO_MQTT
	// Create Task
	xTaskCreate(mirf_receiver, "RECV", 1024*3, NULL, 2, NULL);
	xTaskCreate(mqtt_pub_task, "PUB", 1024*4, NULL, 2, NULL);
#endif

#if CONFIG_MQTT_TO_RADIO
	// Create Task
	xTaskCreate(mirf_transmitter, "TRANS", 1024*4, NULL, 2, NULL);
	xTaskCreate(mqtt_sub_task, "SUB", 1024*4, NULL, 2, NULL);
#endif

}
