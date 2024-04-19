# Ping-Pong Example   
Send data from primary to secondary.   
In the secondary, sent back received data.   


# Configuration   

![config-top](https://user-images.githubusercontent.com/6020549/154790653-277f0f2f-0b51-4a2c-aead-6aeea0ac232f.jpg)

![config-app](https://github.com/nopnop2002/esp-idf-mirf/assets/6020549/29b81f36-dde7-4e36-bb99-0eb9efe6e1f2)

# nRF24L01 Address Register Setting
|Primary||||Secondary||||
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|TX_ADDR<br>"FGHIJ"|RX_ADDR_P0<br>"FGHIJ"|RX_ADDR_P1<br>"ABCDE"||TX_ADDR<br>"ABCDE"|RX_ADDR_P0<br>"ABCDE"|RX_ADDR_P1<br>"FGHIJ"||
|(Send Data)|->|->|->|->|->|(Get Data)|Data to Secondary|
||(Get Ack)|<-|<-|<-|<-|(Send Ack)|Ack to Primary|
|||||||||
|||(Get Data)|<-|(Send Data)|||Data to Primary|
|||(Send Ack)|->|->|(Get Ack)||Ack to Secondary|

# Setting Register
The underlined address match on the sending and receiving sides.

### Primary Register
![Register-Primary](https://github.com/nopnop2002/esp-idf-mirf/assets/6020549/fa2929cb-13cd-4aea-8022-db92d427bf18)

### Secondary Register
![Register-Secondary](https://github.com/nopnop2002/esp-idf-mirf/assets/6020549/b30bc789-0802-4c67-8586-67ca52a2051c)


# Communicat with Arduino Environment   
Run this sketch.   
ArduinoCode/Ping-Pong   

