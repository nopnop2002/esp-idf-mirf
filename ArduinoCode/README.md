# Arduino code for nRF24L01

### Wirering

|nRF24L01||ATMega328|ATMega2560|ESP8266||
|:-:|:-:|:-:|:-:|:-:|:-:|
|VCC|--|3.3V|3.3V|3.3V|(*1)|
|GND|--|GND|GND|GND||
|MISO|--|D12|IO50|IO12||
|MOSI(*2)|--|D11|IO51|IO13||
|SCK(*2)|--|D13|IO52|IO14||
|CE(*2)|--|D10|IO10|IO15|(*3)|
|CSN(*2)|--|D9|IO9|IO16|(*3)|

(*1)   
Since the 3.3V output current capacity of UNO compatible devices is smaller than the official product, it may not work properly if 3.3V is supplied directly from the board.   
If you use an external regulator to convert from 5V to 3.3V and supply it to nRF24L01, it will operate stably.   


(*2)   
These pins are 5V Tolerant and can accept up to 5.25V.   


(*3)   
These pins can be changed in the sketch.   
