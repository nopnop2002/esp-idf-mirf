# esp-idf-mirf
A port of the Mirf library for ESP-IDF.   
The library provides basic control of the Nordic NRF24L01 RF modules.

I ported from [here](https://github.com/nopnop2002/Arduino-STM32-nRF24L01).   

# Software requirements
esp-idf v4.4 or later.   
This is because this version supports ESP32-C3.   

# Installation for ESP32

```Shell
git clone https://github.com/nopnop2002/esp-idf-mirf
cd esp-idf-mirf
idf.py set-target esp32
idf.py menuconfig
idf.py flash
```

# Installation for ESP32-S2

```Shell
git clone https://github.com/nopnop2002/esp-idf-mirf
cd esp-idf-mirf
idf.py set-target esp32s2
idf.py menuconfig
idf.py flash
```

# Installation for ESP32-C3

```Shell
git clone https://github.com/nopnop2002/esp-idf-mirf
cd esp-idf-mirf
idf.py set-target esp32c3
idf.py menuconfig
idf.py flash
```

__Note__   
For some reason GPIO06, GPIO08, GPIO09, GPIO19 cannot be used for SPI clock pins on my board.   
I looked at the ESP32-C3 datasheet, but I don't understand why.   
This may be a problem only for my board.   
If you find a solution, please let me know.   

# Configuration   
You have to set this config value with menuconfig.   
- CONFIG_MISO_GPIO   
 GPIO number(IOxx) to MISO.
- CONFIG_MOSI_GPIO   
 GPIO number(IOxx) to MOSI.
- CONFIG_SCLK_GPIO   
 GPIO number(IOxx) to SCLK.
- CONFIG_CE_GPIO   
 GPIO number(IOxx) to CE.
- CONFIG_CSN_GPIO   
 GPIO number(IOxx) to CSN.
- CONFIG_DIRECTION   
 Communication direction.

![config_main](https://user-images.githubusercontent.com/6020549/108617359-0cc3c500-7459-11eb-9a05-2dd5ce60113b.jpg)

![config-esp32](https://user-images.githubusercontent.com/6020549/108617361-0fbeb580-7459-11eb-97de-4650e5225df0.jpg)


# Wirering

|nRF24L01||ESP32|ESP32-S2|ESP32-C3|
|:-:|:-:|:-:|:-:|:-:|
|MISO|--|GPIO19|GPIO33|GPIO18|
|MOSI|--|GPIO23|GPIO35|GPIO19|
|SCK|--|GPIO18|GPIO36|GPIO10|
|CE|--|GPIO16|GPIO37|GPIO9|
|CSN|--|GPIO17|GPIO38|GPIO8|
|GND|--|GND|GND|GND|
|VCC|--|3.3V|3.3V|3.3V|

__You can change it to any pin using menuconfig.__   
__But it may not work with other GPIOs.__

# Screen shot   

## Initial screen:   
![initial](https://user-images.githubusercontent.com/6020549/108617481-17cb2500-745a-11eb-88d7-85e6e2426a4e.jpg)

## For the transmitter:   
![transmitter](https://user-images.githubusercontent.com/6020549/73982279-496c6d80-4977-11ea-82c6-f7c62764b18c.jpg)

## For the receiver:   
![receiver](https://user-images.githubusercontent.com/6020549/73982284-4b363100-4977-11ea-9ae1-af9da92b13fb.jpg)

# Communicat with AtMega/STM32/ESP8266/ESP8285   
I used [this](https://github.com/nopnop2002/Arduino-STM32-nRF24L01) library on Arduino environment.   
This project communicates with [example/Peer-to-peer Communication/TimeTest](https://github.com/nopnop2002/Arduino-STM32-nRF24L01/tree/master/example/Peer-to-peer%20Communication/TimeTest).   

You need to match the payload size and channel with Arduino and esp-idf.   

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

# Ardunio NANO with nRF24L01
If you use [this](https://www.embeddedcomputing.com/technology/processing/for-the-professional-maker-getting-started-with-the-rf-nano-arduino-nrf24l01-combo), No wiring required.   

# Reference

https://github.com/nopnop2002/esp-idf-mirf2mqtt

