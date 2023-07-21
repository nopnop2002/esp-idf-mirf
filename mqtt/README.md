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

### Recive from radio and send by MQTT
![config-radio-1](https://github.com/nopnop2002/esp-idf-mirf/assets/6020549/16dd7700-c2ca-434c-b02b-3d095cc653cc)

```
            +----------+           +----------+           +----------+           +----------+
            |          |           |          |           |          |           |          |
==(Radio)==>| nRF24L01 |--(SPI)--->|  ESP32   |--(MQTT)-->|  Broker  |--(MQTT)-->|Subscriber|
            |          |           |          |           |          |           |          |
            +----------+           +----------+           +----------+           +----------+
```

You can subscribe MQTT data using mosquitto_sub.   
```
mosquitto_sub -h broker.emqx.io -p 1883 -t '/mirf/#' -F %X -d
```

You can subscribe MQTT data using python.
```
python3 -m pip install -U paho-mqtt
python3 mqtt_sub.py
```


### Recive from MQTT and send by Radio
![config-radio-2](https://github.com/nopnop2002/esp-idf-mirf/assets/6020549/4f49c213-3dd4-44e3-8736-96b171cec616)

```
            +----------+           +----------+           +----------+           +----------+
            |          |           |          |           |          |           |          |
            |Publisher |--(MQTT)-->|  Broker  |--(MQTT)-->|  ESP32   |--(SPI)--->| nRF24L01 |==(Radio)==>
            |          |           |          |           |          |           |          |
            +----------+           +----------+           +----------+           +----------+
```

You can publish MQTT data using mosquitto_pub.   
```
echo -ne "\x01\x02\x03" | mosquitto_pub -h broker.emqx.io -p 1883 -t '/mirf' -s
echo -ne "Hello World" | mosquitto_pub -h broker.emqx.io -p 1883 -t '/mirf' -s
```

You can publish MQTT data using python.   
```
python3 -m pip install -U paho-mqtt
python3 mqtt_pub.py
```


# Communicate with Arduino Environment   
I used [this](https://github.com/nopnop2002/Arduino-STM32-nRF24L01) library on Arduino environment.   
__You need to match the payload size and channel with Arduino and esp-idf.__   

- Arduino environment   
```C++
Mirf.payload = sizeof(mydata.value);
Mirf.channel = 90;
Mirf.config();
```
- esp-idf   
```C
uint8_t payload = sizeof(value);
uint8_t channel = 90;
Nrf24_config(&dev, channel, payload);
```

# Limitation   
The maximum payload size of nRF24L01 is 32 bytes.   
When publishing data exceeding 32 bytes, only 32 bytes are used.   


# MQTT client Example
Example code in various languages.   
https://github.com/emqx/MQTT-Client-Examples


# Visualize data from nRF24L01   

## Using python
There is a lot of information on the internet about the Python + visualization library.   
- [matplotlib](https://matplotlib.org/)
- [seaborn](https://seaborn.pydata.org/index.html)
- [bokeh](https://bokeh.org/)
- [plotly](https://plotly.com/python/)

## Using node.js
There is a lot of information on the internet about the node.js + __real time__ visualization library.   
- [epoch](https://epochjs.github.io/epoch/real-time/)
- [plotly](https://plotly.com/javascript/streaming/)
- [chartjs-plugin-streaming](https://nagix.github.io/chartjs-plugin-streaming/1.9.0/)

# Important
When changing the settings of the nRF24L01, it is necessary to power cycle the nRF24L01 before executing.   
Because nRF24L01 remembers the previous setting.   
nRF24L01 does not have Software Reset function.   

