# esp-idf-mirf
A port of the Mirf library for ESP-IDF.   
The library provides basic control of the Nordic NRF24L01/NRF24L01+ RF modules.

I ported from [here](https://github.com/nopnop2002/Arduino-STM32-nRF24L01).   

![Standard](https://user-images.githubusercontent.com/6020549/154830046-77f034cf-ce30-4cbc-838c-66734656cd8e.JPG)
![SMD-3](https://user-images.githubusercontent.com/6020549/154830127-366ee996-751d-48c0-879f-b201b1bb31f7.JPG)

# Software requirements
esp-idf v4.4 or later.   
This is because this version supports ESP32-C3.   

# Installation

```Shell
git clone https://github.com/nopnop2002/esp-idf-mirf
cd esp-idf-mirf/Peer-to-peer
idf.py set-target {esp32/esp32s2/esp32s3/esp32c3}
idf.py menuconfig
idf.py flash
```

__Note for ESP32C3__   
For some reason, there are development boards that cannot use GPIO06, GPIO08, GPIO09, GPIO19 for SPI clock pins.   
According to the ESP32C3 specifications, these pins can also be used as SPI clocks.   
I used a raw ESP-C3-13 to verify that these pins could be used as SPI clocks.   

# Configuration for Transceiver
![config-mirf-1](https://user-images.githubusercontent.com/6020549/166088471-0653c792-4012-46e5-98b4-d775638bc965.jpg)
![config-mirf-2](https://user-images.githubusercontent.com/6020549/154790120-73f7d79d-f4e2-413a-a61d-f6b77e103ebc.jpg)

# Wirering

|nRF24L01||ESP32|ESP32-S2/S3|ESP32-C3|
|:-:|:-:|:-:|:-:|:-:|
|MISO|--|GPIO19|GPIO37|GPIO18|
|MOSI|--|GPIO23|GPIO35|GPIO19|
|SCK|--|GPIO18|GPIO36|GPIO10|
|CE|--|GPIO16|GPIO34|GPIO9|
|CSN|--|GPIO17|GPIO33|GPIO8|
|GND|--|GND|GND|GND|
|VCC|--|3.3V|3.3V|3.3V|

__You can change it to any pin using menuconfig.__   


# Communicat with Arduino Environment  
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

# Communicat with Raspberry Pi
I used [this](https://github.com/nopnop2002/Raspberry-Mirf) library on Raspberry Pi.   

# Important
When changing the settings of the nRF24L01, it is necessary to power cycle the nRF24L01 before executing.   
Because nRF24L01 remembers the previous setting.   
nRF24L01 does not have Software Reset function.   

# Enhanced ShockBurst features
nRF24L01 has "Enhanced ShockBurst" features.   
"Enhanced ShockBurst" automatically sets the PTX(=Transmitter) in receive mode to wait for the ACK packet from PRX(=Receiver).   

## Transmission Successful   
|ESP32||nRF24L01[PTX]||nRF24L01[PRX]||ESP32|
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|ESP32|-->|nRF24L01|||||
|||nRF24L01|--(Payload)-->|nRF24L01|||||
|||nRF24L01|<--(Ack Packet)--|nRF24L01|||||
|ESP32|<--|nRF24L01|||||||
|||||nRF24L01|-->|ESP32|

## Transmission Failure   
PTX waits for an ACK packet for 250uS and retransmits 3 times.   
|ESP32||nRF24L01[PTX]||nRF24L01[PRX]||ESP32|
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|ESP32|-->|nRF24L01|||||
|||nRF24L01|--(Payload)-->|nRF24L01|||||
||||250uS||||||
|||nRF24L01|--(Payload)-->|nRF24L01|||||
||||250uS||||||
|||nRF24L01|--(Payload)-->|nRF24L01|||||
||||250uS||||||
|ESP32|<--|nRF24L01|||||||


Using a data rate of 250KBps extends the range of radio waves.   
However, it takes time to send PAYLOAD and receive ACK PACKET.   
Therefore, the delay time for automatic retransmission should be longer than 250uS.   
If the delay of automatic retransmission is not increased, it is considered as a transmission failure.   
|ESP32||nRF24L01[PTX]||nRF24L01[PRX]||ESP32|
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|ESP32|-->|nRF24L01|||||
|||nRF24L01|--(Payload)-->|nRF24L01|||||
||||Over 250uS||||||
|||nRF24L01|<--(Ack Packet)--|nRF24L01|||||
|ESP32|<--|nRF24L01|||||||
|||||nRF24L01|-->|ESP32|

See the data sheet for details on Enhanced ShockBurst.   

# Throughput   
1 tick (10 MillSec) is required to send payload and receive ack packet.   
The maximum payload size of nRF24L01 is 32 bytes.   
This has nothing to do with SPI bus speed.   
The throughput of nRF24L01 is 3,200 bytes/sec.   
The data rate of nRF24L01 affects the radio range, but not the speed.   

# Reference

https://github.com/nopnop2002/esp-idf-mirf2mqtt

