# mqtt
This is nRF24L01 and MQTT gateway application.   
```
            +----------+           +----------+           +----------+           +----------+
            |          |           |          |           |          |           |          |
==(Radio)==>| nRF24L01 |--(SPI)--->|  ESP32   |--(MQTT)-->|  Broker  |--(MQTT)-->|Subscriber|
            |          |           |          |           |          |           |          |
            +----------+           +----------+           +----------+           +----------+

            +----------+           +----------+           +----------+           +----------+
            |          |           |          |           |          |           |          |
            |Publisher |--(MQTT)-->|  Broker  |--(MQTT)-->|  ESP32   |--(SPI)--->| nRF24L01 |==(Radio)==>
            |          |           |          |           |          |           |          |
            +----------+           +----------+           +----------+           +----------+
```



# Configuration
![config-top](https://github.com/nopnop2002/esp-idf-mirf/assets/6020549/bea8b4a8-cb37-4aed-b88c-06910018dfb2)
![config-app](https://github.com/nopnop2002/esp-idf-mirf/assets/6020549/e440b0c8-1b5f-48ec-880d-ee756da23d0d)


## WiFi Setting

![config-wifi](https://github.com/nopnop2002/esp-idf-mirf/assets/6020549/4930402d-5cbf-4880-98b9-75509eb76981)


## Radioi Setting

### Radio to MQTT
![config-radio-1](https://github.com/nopnop2002/esp-idf-mirf/assets/6020549/d8d0bb38-e9ad-4b86-b15f-7abd94bfb932)

```
            +----------+           +----------+           +----------+           +----------+
            |          |           |          |           |          |           |          |
==(Radio)==>| nRF24L01 |--(SPI)--->|  ESP32   |--(MQTT)-->|  Broker  |--(MQTT)-->|Subscriber|
            |          |           |          |           |          |           |          |
            +----------+           +----------+           +----------+           +----------+
```

You can subscribe MQTT data using mosquitto_sub.   
```
chmod 777 mqtt_sub.sh
mqtt_sub.sh
```



### MQTT to Radio
![config-radio-2](https://github.com/nopnop2002/esp-idf-mirf/assets/6020549/ad66e20f-ae63-4b33-b50a-1cc9faa9f7f2)

```
            +----------+           +----------+           +----------+           +----------+
            |          |           |          |           |          |           |          |
            |Publisher |--(MQTT)-->|  Broker  |--(MQTT)-->|  ESP32   |--(SPI)--->| nRF24L01 |==(Radio)==>
            |          |           |          |           |          |           |          |
            +----------+           +----------+           +----------+           +----------+
```

You can publish MQTT data using mosquitto_pub.   
```
chmod 777 mqtt_pub.sh
sh ./mqtt_pub.sh
```

# Communicate with Arduino Environment   
Run this sketch.   
ArduinoCode\Peer-to-peer\StringTest   

