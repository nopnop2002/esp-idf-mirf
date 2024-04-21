# esp-idf-mirf
A port of the Mirf library for ESP-IDF.   
The library provides basic control of the Nordic nRF24L01/nRF24L01+ RF modules.

I ported from [here](https://github.com/nopnop2002/Arduino-STM32-nRF24L01).   

![Standard](https://user-images.githubusercontent.com/6020549/154830046-77f034cf-ce30-4cbc-838c-66734656cd8e.JPG)
![SMD-3](https://user-images.githubusercontent.com/6020549/154830127-366ee996-751d-48c0-879f-b201b1bb31f7.JPG)
![SMD-Pinout](https://github.com/nopnop2002/esp-idf-mirf/assets/6020549/4566928d-7fb8-4569-b780-4a502b77cc34)

# Software requirements
ESP-IDF V4.4/V5.x.   
ESP-IDF V5.0 is required when using ESP32-C2.   
ESP-IDF V5.1 is required when using ESP32-C6.   

# Installation

```Shell
git clone https://github.com/nopnop2002/esp-idf-mirf
cd esp-idf-mirf/Peer-to-peer
idf.py set-target {esp32/esp32s2/esp32s3/esp32c2/esp32c3/esp32c6}
idf.py menuconfig
idf.py flash
```

__Note for ESP32C3__   
For some reason, there are development boards that cannot use GPIO06, GPIO08, GPIO09, GPIO19 for SPI clock pins.   
According to the ESP32C3 specifications, these pins can also be used as SPI clocks.   
I used a raw ESP-C3-13 to verify that these pins could be used as SPI clocks.   

# Configuration for Transceiver
![config-nrf24l01-1](https://user-images.githubusercontent.com/6020549/168019514-93c377c9-2823-4840-bce4-168f0c2b7338.jpg)
![config-nrf24l01-2](https://github.com/nopnop2002/esp-idf-mirf/assets/6020549/4898836a-b614-4d30-8ef4-c653706ba7f3)

__Note for Channel__   
Channels range from 0 to 127.   
The channel number of the sender and receiver must be the same.   
Channel = 0 uses the frequency of 2.4000GHz, channel = 127 uses the frequency of 2.4127GHz.   
The width of one channel is only 0.01GHz.   
Using multiple radio node at the same time will almost always cause interference, even if you change channels.   

# SPI BUS selection   
![config-nrf24l01-3](https://github.com/nopnop2002/esp-idf-mirf/assets/6020549/377bbcc5-2ca9-4e74-a54d-3ca87c5ac241)

The ESP32 series has three SPI BUSs.   
SPI1_HOST is used for communication with Flash memory.   
You can use SPI2_HOST and SPI3_HOST freely.   
When you use SDSPI(SD Card via SPI), SDSPI uses SPI2_HOST BUS.   
When using this module at the same time as SDSPI or other SPI device using SPI2_HOST, it needs to be changed to SPI3_HOST.   
When you don't use SDSPI, both SPI2_HOST and SPI3_HOST will work.   
Previously it was called HSPI_HOST / VSPI_HOST, but now it is called SPI2_HOST / SPI3_HOST.   

# Using Advanced Settings   
When used at long distances, lowering the RF data rate stabilizes it.   
When changing the RF data rate, the sender and receiver must have the same value.   
When using 250KBps, it takes time to PAYLOAD sending and ACK PACKET receiving, so it is necessary to increase the automatic retransmission delay.   
I tested it with [this](https://github.com/nopnop2002/Arduino-STM32-nRF24L01/tree/master/example/AdvancedSetting).   

![config-nrf24l01-4](https://github.com/nopnop2002/esp-idf-mirf/assets/6020549/a18be916-7716-4e22-bdc0-6813883c64a5)

![config-nrf24l01-5](https://github.com/nopnop2002/esp-idf-mirf/assets/6020549/ba577705-c2b7-494c-bcd7-969d96cf859a)

__If you want to initialize the nRF24L01 settings after using the Advanced Settings, you need to power cycle the nRF24L01 before executing.__   
Because nRF24L01 remembers the previous setting.   
nRF24L01 does not have Software Reset function.   


# Wirering

|nRF24L01||ESP32|ESP32-S2/S3|ESP32-C2/C3/C6||
|:-:|:-:|:-:|:-:|:-:|:-:|
|MISO|--|GPIO19|GPIO37|GPIO4|(*1)|
|SCK|--|GPIO18|GPIO36|GPIO3|(*1)|
|MOSI|--|GPIO23|GPIO35|GPIO2|(*1)|
|CE|--|GPIO16|GPIO34|GPIO1|(*1)|
|CSN|--|GPIO17|GPIO33|GPIO0|(*1)|
|GND|--|GND|GND|GND||
|VCC|--|3.3V|3.3V|3.3V||

(*1)You can change it to any pin using menuconfig.   

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

# Using binary data
nRF24L01 is not interested in the contents of the payload.   
Therefore, nRF24L01 can send and receive binary data.   
But the internal format of the binary data depends on the MCU architecture, so communication between different architectures is not possible.   
If you want to send or receive binary data between different architectures, you need to encode it into a CSV format string like this.   
```123,-123,1234.5,-1234.5```   
nRF24L01 can send and receive up to 32 characters, but by adding an index to the sent data, it can send and receive 256 types of data.   


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
|||nRF24L01|--(Payload Packet)-->|nRF24L01|||||
||||wait 250uS||||||
|||nRF24L01|<--(Ack Packet)--|nRF24L01|||||
|ESP32|<--|nRF24L01|||||||
|||||nRF24L01|-->|ESP32|

## Transmission Failure   
PTX waits for an ACK packet for 250uS and retransmits 3 times.   
|ESP32||nRF24L01[PTX]||nRF24L01[PRX]||ESP32|
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|ESP32|-->|nRF24L01|||||
|||nRF24L01|--(Payload Packet)-->|nRF24L01|||||
||||wait 250uS||||||
|||nRF24L01|--(Payload Packet)-->|nRF24L01|||||
||||wait 250uS||||||
|||nRF24L01|--(Payload Packet)-->|nRF24L01|||||
||||wait 250uS||||||
|ESP32|<--|nRF24L01|||||||


Using a data rate of 250KBps extends the range of radio waves.   
However, it takes time to send PAYLOAD and receive ACK PACKET.   
Therefore, the delay time for automatic retransmission should be longer than 250uS.   
If the delay of automatic retransmission is not increased, it is considered as a transmission failure.   
|ESP32||nRF24L01[PTX]||nRF24L01[PRX]||ESP32|
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|ESP32|-->|nRF24L01|||||
|||nRF24L01|--(Payload Packet)-->|nRF24L01|||||
||||Over 250uS||||||
|||nRF24L01|<--(Ack Packet)--|nRF24L01|||||
|ESP32|<--|nRF24L01|||||||
|||||nRF24L01|-->|ESP32|

See the data sheet for details on Enhanced ShockBurst.   


# Throughput   
spi_device_transmit is executed by the SPI driver and consumes 1tick of CPU time.   
When DMA is enabled, SPI max_transfer_sz defaults to 4092(4K).   
This indicates that 4092 bytes of data can be sent in 1tick(10 millsec).   
Therefore, the SPI transmission potential of ESP-IDF is 400Kbytes/Sec.   
This has a significant impact on the nRF24L01, which has a small maximum payload size.   

Unfortunately, nRF24L01's maximum payload size is 32 bytes.   
1 tick (10 millsec) is required to send payload and receive ack packet.   
This has nothing to do with SPI bus speed.   
The throughput of nRF24L01 is 32 bytes/10 millsec(=3,200 bytes/sec).   
RF data rate of nRF24L01 affects the radio range, but not the throughput.   

# About Si24R1 clone   
Si24R1 is marketed as a nRF24L01 compatible.   
__Si24R1 clone cannot send and receive correctly.__   
After purchasing the nRF24L01, be sure to check the markings on the chip.   
Many suppliers sell Si24R1 as nRF24L01.   
