# esp-idf-mirf
A port of the Mirf library to ESP-IDF.   
The library provides basic control of the Nordic NRF24l01 RF Transceiver.

I ported from [here](https://github.com/nopnop2002/Arduino-STM32-nRF24L01).   

# Configure
You have to set this config value with menuconfig.
CONFIG_CE_GPIO
CONFIG_CSN_GPIO

```
git clone https://github.com/nopnop2002/esp-idf-mirf
cd esp-idf-mirf/
make menuconfig
make flash
```

![config-1](https://user-images.githubusercontent.com/6020549/73979588-c7c61100-4971-11ea-8540-fd1d4694eb52.jpg)
![config-2](https://user-images.githubusercontent.com/6020549/73979597-cac10180-4971-11ea-9caf-b1bf9e776e52.jpg)

# Wirering

|nRF24L01||ESP32|
|:-:|:-:|:-:|
|MISO|--|GPIO19|
|SCK|--|GPIO18|
|MOSI|--|GPIO23|
|CE|--|GPIO16(*)|
|CSN|--|GPIO17(*)|
|GND|--|GND|
|VCC|--|3.3V|

\*You can change any pin using menuconfig.   

![config-3](https://user-images.githubusercontent.com/6020549/73979603-cd235b80-4971-11ea-8e50-f70b17fea8e4.jpg)
![transmitter](https://user-images.githubusercontent.com/6020549/73979625-d3193c80-4971-11ea-9e8a-68b595eeb49a.jpg)
![receiver](https://user-images.githubusercontent.com/6020549/73979668-ec21ed80-4971-11ea-9c32-3eaa870219ca.jpg)

# Communicat with Arduino
I used [this](https://github.com/nopnop2002/Arduino-STM32-nRF24L01) library on Arduino.   


# Communicat with Raspberry Pi
I used [this](https://github.com/nopnop2002/Raspberry-Mirf) library on Raspberry Pi.   

