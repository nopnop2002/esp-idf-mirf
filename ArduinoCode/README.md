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
UNO's 3.3V output can only supply 50mA.   
In addition, the output current capacity of UNO-compatible devices is smaller than that of official products.   
__So nRF24L01 may not work normally when supplied from the on-board 3v3__.   
nRF24L01+PA+LNA(nRF24L01+RFX24C01) needs 115mA.   
You will need to power it from the 5V pin using a regulator.   


(*2)   
These pins are 5V Tolerant and can accept up to 5.25V.   


(*3)   
These pins can be changed in the sketch.   
