# WebSocket Example   
This is nRF24L01 and WebSocket gateway application.   
```
            +-----------+              +-----------+              +-----------+
            |           |              |           |              |           |
            | Web Client|-(WebSocket)->|   ESP32   |----(SPI)---->| nRF24L01  |==(Radio)==>
            |           |              |           |              |           |
            +-----------+              +-----------+              +-----------+

            +-----------+              +-----------+              +-----------+
            |           |              |           |              |           |
==(Radio)==>| nRF24L01  |----(SPI)---->|   ESP32   |-(WebSocket)->| Web Server|
            |           |              |           |              |           |
            +-----------+              +-----------+              +-----------+
```



# Configuration
![config-top](https://github.com/user-attachments/assets/3953103c-466b-4167-a298-dec72ff7bdad)
![config-app](https://github.com/user-attachments/assets/11ee35bd-3f59-4b95-9ab3-1c07241751ae)

## WiFi Setting
Set the information of your access point.   
![config-wifi](https://github.com/user-attachments/assets/8654b6a8-9dbe-46f7-825c-0928b1a0e9e1)

## Radio Setting
Set the wireless communication direction.   

### WebSocket to Radio
Receive from WebSocket and send to Radio.   
ESP32 acts as Web Server.   
You can use ws-client.py as Web Client.   
You need to install websockets from here.   
https://github.com/python-websockets/websockets   

```
            +-----------+              +-----------+              +-----------+
            |           |              |           |              |           |
            | Web Client|-(WebSocket)->|   ESP32   |----(SPI)---->| nRF24L01  |==(Radio)==>
            |           |              |           |              |           |
            +-----------+              +-----------+              +-----------+
```

![config-radio-1](https://github.com/user-attachments/assets/ce32a476-e13a-4c83-85cd-cdd8719fe08b)



### Radio to WebSocket
Receive from Radio and send to WebSocket.   
ESP32 acts as Web Client.   
You can use ws-server.py as Web Server.   
You need to install websocket-server from here.   
https://github.com/Pithikos/python-websocket-server   

```
            +-----------+              +-----------+              +-----------+
            |           |              |           |              |           |
==(Radio)==>| nRF24L01  |----(SPI)---->|   ESP32   |-(WebSocket)->| Web Server|
            |           |              |           |              |           |
            +-----------+              +-----------+              +-----------+
```

![config-radio-2](https://github.com/user-attachments/assets/5a8a44e3-7982-4795-bdc0-1da57fd7cb01)



### Specifying an WebSocket Server   
You can specify your WebSocket Server in one of the following ways:   
- IP address   
 ```192.168.10.20```   
- mDNS host name   
 ```http-server.local```   


# Communicate with Arduino Environment
Run this sketch.   
ArduinoCode\Peer-to-peer\StringTest   
