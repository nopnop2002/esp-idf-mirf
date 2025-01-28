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

![config-radio-1](https://github.com/user-attachments/assets/7319564d-f00a-433b-adc3-7915cc2a756b)


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

![config-radio-2](https://github.com/user-attachments/assets/ba6cd672-270a-4d15-9304-44404fd67eb6)

### Select Transport   
This project supports TCP,SSL/TLS,WebSocket and WebSocket Secure Port.   
- Using TCP Port.   
 TCP Port uses the MQTT protocol.   
 ![config-mqtt-tcp](https://github.com/user-attachments/assets/a20aafa5-a814-49c3-8304-9aa5ba0af4e3)

- Using SSL/TLS Port.   
 SSL/TLS Port uses the MQTTS protocol instead of the MQTT protocol.   
 ![config-mqtt-ssl](https://github.com/user-attachments/assets/0b02c5ea-1147-4a3f-99cf-f142047fc477)

- Using WebSocket Port.   
 WebSocket Port uses the WS protocol instead of the MQTT protocol.   
 ![config-mqtt-ws](https://github.com/user-attachments/assets/ca202c91-2cf6-4093-9740-e34ed50c11df)

- Using WebSocket Secure Port.   
 WebSocket Secure Port uses the WSS protocol instead of the MQTT protocol.   
 ![config-mqtt-wss](https://github.com/user-attachments/assets/92961b07-2a91-4284-8e06-30badcbdc98c)

__Note for using secure port.__   
The default MQTT server is ```broker.emqx.io```.   
If you use a different server, you will need to modify ```getpem.sh``` to run.   
```
chmod 777 getpem.sh
./getpem.sh
```

WebSocket/WebSocket Secure Port may differ depending on the broker used.   
If you use a different server, you will need to change the port number from the default.   

__Note for using MQTTS/WS/WSS transport.__   
If you use MQTTS/WS/WSS transport, you can still publish and subscribe using MQTT transport.   
```
+----------+                   +----------+           +----------+
|          |                   |          |           |          |
|  ESP32   | ---MQTTS/WS/WSS-->|  Broker  | ---MQTT-->|Subsctiber|
|          |                   |          |           |          |
+----------+                   +----------+           +----------+

+----------+                   +----------+           +----------+
|          |                   |          |           |          |
|  ESP32   | <--MQTTS/WS/WSS---|  Broker  | <--MQTT---|Publisher |
|          |                   |          |           |          |
+----------+                   +----------+           +----------+
```


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

### Secure Option
Specifies the username and password if the server requires a password when connecting.   
[Here's](https://www.digitalocean.com/community/tutorials/how-to-install-and-secure-the-mosquitto-mqtt-messaging-broker-on-debian-10) how to install and secure the Mosquitto MQTT messaging broker on Debian 10.   
![config-radio-3](https://github.com/user-attachments/assets/7c04a8fc-96a0-46a6-ab59-17fa55b4ea59)

# Communicate with Arduino Environment   
Run this sketch.   
ArduinoCode\Peer-to-peer\StringTest   

