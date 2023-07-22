# IRQ Detection Example   
Use IRQ to detect transmission completion and reception completion.   

# About the nrf24l01 assertion

nrf24l01 has three assertions.   
- MAX_RT   
- RX_DR   
- TX_DS   

![Enhanced-Shokburst-1](https://user-images.githubusercontent.com/6020549/216748353-9028e1c9-0ec3-45fa-a13e-bfeabb42db86.jpg)

After the packet is transmitted by the PTX and received by the PRX the ACK packet with payload is transmitted from the PRX to the PTX.   
The RX_DR IRQ is asserted after the packet is received by the PRX, whereas on the PTX side the TX_DS IRQ is asserted when the ACK packet is received by the PTX.   
On the PRX side, the TX_DS IRQ for the ACK packet payload is asserted after a new packet from PTX is received.   

![Enhanced-Shokburst-2](https://user-images.githubusercontent.com/6020549/216748358-5264affc-6f7c-4f44-b8b8-23e0e30ddef1.jpg)

the ACK packet is lost and a retransmission is needed before the TX_DS IRQ is asserted, but the RX_DR IRQ is asserted immediately.   
MAX_RT IRQ is asserted if the auto retransmit counter(ARC_CNT) exceeds the programmed maximum limit(ARC).   
See the nrf24l01 datasheet for more details.   

This project treats RX_DR as received completion.   
This project treats MAX_RT as transmission failure.   
Therefore, if there is no assertion after the transmission, the transmission succeeds, and if there is an assertion, the transmission fails.   
This project does not deal with TX_DS.

# Configuration   

![config-top](https://github.com/nopnop2002/esp-idf-mirf/assets/6020549/3aabd6f8-7477-4b71-b6c4-950d18402a87)

![config-app](https://github.com/nopnop2002/esp-idf-mirf/assets/6020549/01c6e755-05b1-43e7-90d5-c0eb063f5b82)

# Wirering
__For this project we need one more connection for IRQ detection.__   
|nRF24L01||ESP32|ESP32-S2/S3|ESP32-C2/C3/C6||
|:-:|:-:|:-:|:-:|:-:|:-:|
|MISO|--|GPIO19|GPIO37|GPIO4|(*1)|
|MOSI|--|GPIO23|GPIO35|GPIO3|(*1)|
|SCK|--|GPIO18|GPIO36|GPIO2|(*1)|
|CE|--|GPIO16|GPIO34|GPIO1|(*1)|
|CSN|--|GPIO17|GPIO33|GPIO0|(*1)|
|IRQ|--|GPIO15|GPIO38|GPIO5|(*1)|
|GND|--|GND|GND|GND||
|VCC|--|3.3V|3.3V|3.3V||

(*1)You can change it to any pin using menuconfig.   
__IRQ needs to be able to use interrupts.__   
__Some GPIOs cannot use interrupts.__   

# Communicat with Arduino Environment   
Run this sketch.   
ArduinoCode\Peer-to-peer\StringTest   
