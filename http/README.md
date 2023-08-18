# HTTP Example   
This is nRF24L01 and HTTP gateway application.   
```
            +-----------+           +-----------+           +-----------+
            |           |           |           |           |           |
            |HTTP Client|--(HTTP)-->|   ESP32   |--(SPI)--->| nRF24L01  |==(Radio)==>
            |           |           |           |           |           |
            +-----------+           +-----------+           +-----------+

            +-----------+           +-----------+           +-----------+
            |           |           |           |           |           |
==(Radio)==>| nRF24L01  |--(SPI)--->|   ESP32   |--(HTTP)-->|HTTP Server|
            |           |           |           |           |           |
            +-----------+           +-----------+           +-----------+
```



# Configuration
![config-top](https://github.com/nopnop2002/esp-idf-mirf/assets/6020549/e8e22c35-da61-4d5b-82ab-751b05d54d98)
![config-app](https://github.com/nopnop2002/esp-idf-mirf/assets/6020549/aeb5992b-ca2e-4ec9-aa56-7b70e8a06491)

## WiFi Setting

![config-wifi](https://github.com/nopnop2002/esp-idf-mirf/assets/6020549/49cba564-dd2b-46b9-bcc4-c928a816697c)


## Radioi Setting

### HTTP to Radio
Subscribe with HTTP and send to Radio.   
ESP32 acts as HTTP Server.   
You can use curl as HTTP Client.   
```sh ./http-client.sh```

```
            +-----------+           +-----------+           +-----------+
            |           |           |           |           |           |
            |HTTP Client|--(HTTP)-->|   ESP32   |--(SPI)--->| nRF24L01  |==(Radio)==>
            |           |           |           |           |           |
            +-----------+           +-----------+           +-----------+
```

![config-radio-1](https://github.com/nopnop2002/esp-idf-mirf/assets/6020549/562285e0-3c3a-4315-b960-ab27186e9c95)


### Radio to HTTP
Receive from Radio and publish as HTTP.   
ESP32 acts as HTTP Client.   
You can use nc(netcat) as HTTP Server.   
```sh ./http-server.sh```

```
            +-----------+           +-----------+           +-----------+
            |           |           |           |           |           |
==(Radio)==>| nRF24L01  |--(SPI)--->|   ESP32   |--(HTTP)-->|HTTP Server|
            |           |           |           |           |           |
            +-----------+           +-----------+           +-----------+
```

![config-radio-2](https://github.com/nopnop2002/esp-idf-mirf/assets/6020549/118b4d07-7c43-48d3-84fd-670e0e678370)



### Specifying an HTTP Server   
You can specify your HTTP Server in one of the following ways:   
- IP address   
 ```192.168.10.20```   
- mDNS host name   
 ```http-server.local```   
- Fully Qualified Domain Name   
 ```http-server.public.io```


# Communicate with Arduino Environment   
Run this sketch.   
ArduinoCode\Peer-to-peer\StringTest   

