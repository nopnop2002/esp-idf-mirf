#define	PUBLISH		100
#define	SUBSCRIBE	200

typedef struct {
	int16_t topic_type;
	int16_t topic_len;
	char topic[64];
	int16_t payload_len;
	char payload[64];
} MQTT_t;

