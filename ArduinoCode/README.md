# Arduino code for nRF24L01

### Wirering

|nRF24L01||ATMega328||
|:-:|:-:|:-:|:-:|
|VCC|--|3.3V|(*1)|
|GND|--|GND||
|MISO|--|D12||
|MOSI|--|D11|(*2)|
|SCK|--|D13|(*2)|
|CE|--|D10|(*2)|
|CSN|--|D9|(*2)|

(*1)   
Since the 3.3V output current capacity of UNO compatible devices is smaller than the official product, it may not work properly if 3.3V is supplied directly from the board.   
If you use an external regulator to convert from 5V to 3.3V and supply it to nRF24L01, it will operate stably.   


(*2)   
These pins are 5V Tolerant and can accept up to 5.25V.
