# MQTT Example   
This is nRF24L01 and MQTT gateway application.   
```
            +----------+           +----------+           +----------+           +----------+
            |          |           |          |           |          |           |          |
            |Publisher |--(MQTT)-->|  Broker  |--(MQTT)-->|  ESP32   |--(SPI)--->| nRF24L01 |==(Radio)==>
            |          |           |          |           |          |           |          |
            +----------+           +----------+           +----------+           +----------+

            +----------+           +----------+           +----------+           +----------+
            |          |           |          |           |          |           |          |
==(Radio)==>| nRF24L01 |--(SPI)--->|  ESP32   |--(MQTT)-->|  Broker  |--(MQTT)-->|Subscriber|
            |          |           |          |           |          |           |          |
            +----------+           +----------+           +----------+           +----------+
```



# Configuration
![config-top](https://github.com/nopnop2002/esp-idf-mirf/assets/6020549/bea8b4a8-cb37-4aed-b88c-06910018dfb2)
![config-app](https://github.com/nopnop2002/esp-idf-mirf/assets/6020549/e440b0c8-1b5f-48ec-880d-ee756da23d0d)


## WiFi Setting

![config-wifi](https://github.com/nopnop2002/esp-idf-mirf/assets/6020549/4930402d-5cbf-4880-98b9-75509eb76981)


## Radioi Setting

### MQTT to Radio
Subscribe with MQTT and send to Radio.   
You can use mosquitto_pub as Publisher.   
```sh ./mqtt_pub.sh```

```
            +----------+           +----------+           +----------+           +----------+
            |          |           |          |           |          |           |          |
            |Publisher |--(MQTT)-->|  Broker  |--(MQTT)-->|  ESP32   |--(SPI)--->| nRF24L01 |==(Radio)==>
            |          |           |          |           |          |           |          |
            +----------+           +----------+           +----------+           +----------+
```

![config-radio-2](https://github.com/nopnop2002/esp-idf-mirf/assets/6020549/ad66e20f-ae63-4b33-b50a-1cc9faa9f7f2)



### Radio to MQTT
Receive from Radio and publish as MQTT.   
You can use mosquitto_sub as Subscriber.   
```sh ./mqtt_sub.sh```

```
            +----------+           +----------+           +----------+           +----------+
            |          |           |          |           |          |           |          |
==(Radio)==>| nRF24L01 |--(SPI)--->|  ESP32   |--(MQTT)-->|  Broker  |--(MQTT)-->|Subscriber|
            |          |           |          |           |          |           |          |
            +----------+           +----------+           +----------+           +----------+
```

![config-radio-1](https://github.com/nopnop2002/esp-idf-mirf/assets/6020549/d8d0bb38-e9ad-4b86-b15f-7abd94bfb932)


### Specifying an MQTT Broker   
You can specify your MQTT broker in one of the following ways:   
- IP address   
 ```192.168.10.20```   
- mDNS host name   
 ```mqtt-broker.local```   
- Fully Qualified Domain Name   
 ```broker.emqx.io```

You can use this as broker.   
https://github.com/nopnop2002/esp-idf-mqtt-broker


# Communicate with Arduino Environment   
Run this sketch.   
ArduinoCode\Peer-to-peer\StringTest   

