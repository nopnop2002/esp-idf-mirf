# Peer-to-peer
One-way communication for transmission or reception.   

# Configuration   

![config_main](https://user-images.githubusercontent.com/6020549/108617359-0cc3c500-7459-11eb-9a05-2dd5ce60113b.jpg)
![config-nrf24l01-1](https://user-images.githubusercontent.com/6020549/149711042-6e145028-716c-432d-a763-7734a76fcaab.jpg)
![config-nrf24l01-2](https://user-images.githubusercontent.com/6020549/149711045-138d90dd-812d-4c11-9430-edf2a3e7661f.jpg)



# Using as receiver   
I tested it with [this](https://github.com/nopnop2002/Arduino-STM32-nRF24L01/tree/master/example/Peer-to-peer%20Communication/TimeTest/Emitter).   

![receiver](https://user-images.githubusercontent.com/6020549/73982284-4b363100-4977-11ea-9ae1-af9da92b13fb.jpg)


# Using as transmitter   
I tested it with [this](https://github.com/nopnop2002/Arduino-STM32-nRF24L01/tree/master/example/Peer-to-peer%20Communication/TimeTest/Receive).   

![transmitter](https://user-images.githubusercontent.com/6020549/73982279-496c6d80-4977-11ea-82c6-f7c62764b18c.jpg)

# Using Advanced Settings   
When used at long distances, lowering the RF data rate stabilizes it.   
When changing the RF data rate, the sender and receiver must have the same value.   
When using 250KBps, it is necessary to increase the Auto Retransmit Delay.   
I tested it with [this](https://github.com/nopnop2002/Arduino-STM32-nRF24L01/tree/master/example/AdvancedSetting).   

![config-nrf24l01-3](https://user-images.githubusercontent.com/6020549/149711050-91b36d1f-457a-434d-a652-d0231c7de45d.jpg)


