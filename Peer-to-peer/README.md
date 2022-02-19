# Peer-to-peer
One-way communication for transmission or reception.   

# Configuration   

![config-top](https://user-images.githubusercontent.com/6020549/154790249-b1f28d18-7c60-4a55-b262-5d821adbbfc3.jpg)


# Using as receiver   
I tested it with [this](https://github.com/nopnop2002/Arduino-STM32-nRF24L01/tree/master/example/Peer-to-peer%20Communication/TimeTest/Emitter).   

![config-app-1](https://user-images.githubusercontent.com/6020549/154790362-0ac3875d-8130-4aed-9e33-199fa85564ab.jpg)
![receiver](https://user-images.githubusercontent.com/6020549/73982284-4b363100-4977-11ea-9ae1-af9da92b13fb.jpg)


# Using as transmitter   
I tested it with [this](https://github.com/nopnop2002/Arduino-STM32-nRF24L01/tree/master/example/Peer-to-peer%20Communication/TimeTest/Receive).   

![config-app-2](https://user-images.githubusercontent.com/6020549/154790370-1d6689c9-d77c-4d64-9cd9-b8622c6ba310.jpg)
![transmitter](https://user-images.githubusercontent.com/6020549/73982279-496c6d80-4977-11ea-82c6-f7c62764b18c.jpg)

# Using Advanced Settings   
When used at long distances, lowering the RF data rate stabilizes it.   
When changing the RF data rate, the sender and receiver must have the same value.   
When using 250KBps, it takes time to PAYLOAD sending and ACK PACKET receiving, so it is necessary to increase the automatic retransmission delay.   
I tested it with [this](https://github.com/nopnop2002/Arduino-STM32-nRF24L01/tree/master/example/AdvancedSetting).   

![config-app-3](https://user-images.githubusercontent.com/6020549/154790329-0ad796c4-ed61-46a1-99a3-f338cc810ea4.jpg)

__If you want to initialize the nRF24L01 settings after using the Advanced Settings, you need to power cycle the nRF24L01 before executing.__   
Because nRF24L01 remembers the previous setting.   
nRF24L01 does not have Software Reset function.   

