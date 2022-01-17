# MultiCast
Multiple send to one receive communication.   
The nRF24L01 has 6 receive data pipes(PTX1-PTX6).   
In this library, the first datapipe(PTX1) is used for automatic ACK reception on transmission.   
The second(PTX2) to sixth(PTX6) data pipes are used for data reception.   
Therefore, it is possible to receive from a maximum of five transmitting sides.   
This example receive from 1RECV/2RECV/3RECV/4RECV/5RECV.   

![MultiCast-2](https://user-images.githubusercontent.com/6020549/149723671-e5a4fd63-199c-4716-8cc7-d3806b562243.jpg)

# Configuration   

![config_main](https://user-images.githubusercontent.com/6020549/108617359-0cc3c500-7459-11eb-9a05-2dd5ce60113b.jpg)
![config-multicast-1](https://user-images.githubusercontent.com/6020549/149722464-2d7180a1-f98d-436a-bd10-5c978bd8ac23.jpg)

# MultiCast Receiver   
The ESP32 acts as a receiver from five pipes.   
I tested it with [this](https://github.com/nopnop2002/Arduino-STM32-nRF24L01/tree/master/example/MultiCast%20Communication).   

# Screen Shot   
![MultiCast-1](https://user-images.githubusercontent.com/6020549/149722501-df8ef00c-4a5a-44f2-b055-8d9f0dcad027.jpg)
