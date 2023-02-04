# IRQ-Detection
Use IRQ to detect transmission completion and reception completion.   

# About the nrf24l01 assertion

nrf24l01 has three assertions.   
- MAX_RT   
- RX_DR   
- TX_DS   

![Enhanced-Shokburst](https://user-images.githubusercontent.com/6020549/216746999-586c5038-3150-4802-8e01-bf50e4cfb33a.jpg)

After the packet is transmitted by the PTX and received by the PRX the ACK packet with payload is transmitted from the PRX to the PTX.   
The RX_DR IRQ is asserted after the packet is received by the PRX, whereas on the PTX side the TX_DS IRQ is asserted when the ACK packet is received by the PTX.   
On the PRX side, the TX_DS IRQ for the ACK packet payload is asserted after a new packet from PTX is received.   
the ACK packet is lost and a retransmission is needed before the TX_DS IRQ is asserted, but the RX_DR IRQ is asserted immediately.   
MAX_RT IRQ is asserted if the auto retransmit counter(ARC_CNT) exceeds the programmed maximum limit(ARC).   
See the nrf24l01 datasheet for more details.   

This project treats RX_DR as received completion.   
This project treats MAX_RT as transmission failure.   
Therefore, if there is no assertion after the transmission, the transmission succeeds, and if there is an assertion, the transmission fails.   
This project does not deal with TX_DS.

# Configuration   

![config-app](https://user-images.githubusercontent.com/6020549/216746996-acef099e-3803-4a36-8eae-9a9d1b97bdb5.jpg)


# Wirering
For this project, we need one more connection.

|nRF24L01||ESP32|ESP32-S2/S3|ESP32-C2/C3||
|:-:|:-:|:-:|:-:|:-:|:-:|
|MISO|--|GPIO19|GPIO37|GPIO4|(*1)|
|MOSI|--|GPIO23|GPIO35|GPIO3|(*1)|
|SCK|--|GPIO18|GPIO36|GPIO2|(*1)|
|CE|--|GPIO16|GPIO34|GPIO1|(*1)|
|CSN|--|GPIO17|GPIO33|GPIO0|(*1)|
|IRQ|--|GPIO15|GPIO38|GPIO5|(*1)|
|GND|--|GND|GND|GND||
|VCC|--|3.3V|3.3V|3.3V||

