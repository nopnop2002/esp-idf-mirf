# Multiple-Receive
The nRF24L01 has 6 receive data pipes(PTX1-PTX6).   
In this library, the first datapipe(PTX1) is used for automatic ACK reception on transmission.   
The second(PTX2) to sixth(PTX6) data pipes are used for data reception.   
Therefore, it is possible to receive from a maximum of five transmitting sides.   

In this example, it will receive from the following addresses:   
```1RECV/2RECV/3RECV/4RECV/5RECV```

I tested it with [this](https://github.com/nopnop2002/Arduino-STM32-nRF24L01/tree/master/example/Multiple-Receive%20Communication).   

![MultiCast-2](https://user-images.githubusercontent.com/6020549/149723671-e5a4fd63-199c-4716-8cc7-d3806b562243.jpg)

# Screen Shot   
![MultiCast-1](https://user-images.githubusercontent.com/6020549/201519900-2fc5d4cf-1ecc-4af1-a261-df53e73e1c59.jpg)

