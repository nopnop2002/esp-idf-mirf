# Multiple Receive Example   
The nRF24L01 has 6 receive data pipes (RX_ADDR_P0-P6).   
In this library, the first data pipe(RX_ADDR_P0) is used for automatic ACK reception on transmission.   
The second(RX_ADDR_P1) to sixth(RX_ADDR_P5) data pipes are used for data reception.   
Therefore, it is possible to receive from a maximum of five transmitting sides.   
This example receive from ```1RECV/2RECV/3RECV/4RECV/5RECV```.   

# nRF24L01 Address Register Setting
|Sender|||||Receiver||||||
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|#1|TX_ADDR<br>"1RECV"|RX_ADDR_P0<br>"1RECV"|RX_ADDR_P1<br>NONE||TX_ADDR<br>NONE|RX_ADDR_P0<br>NONE|RX_ADDR_P1<br>"1RECV"|RX_ADDR_P2<br>"2RECV"|RX_ADDR_P5<br>"5RECV"||
||(Send Data)|->|->|->|->|->|(Get Data)|||Data to Receiver|
|||(Get Ack)|<-|<-|<-|<-|(Send Ack)|||Ack to Sender|
|#2|TX_ADDR<br>"2RECV"|RX_ADDR_P0<br>"2RECV"|RX_ADDR_P1<br>NONE||TX_ADDR<br>NONE|RX_ADDR_P0<br>NONE|RX_ADDR_P1<br>"1RECV"|RX_ADDR_P2<br>"2RECV"|RX_ADDR_P5<br>"5RECV"||
||(Send Data)|->|->|->|->|->|->|(Get Data)||Data to Receiver|
|||(Get Ack)|<-|<-|<-|<-|<-|(Send Ack)||Ack to Sender|
|#5|TX_ADDR<br>"5RECV"|RX_ADDR_P0<br>"5RECV"|RX_ADDR_P1<br>NONE||TX_ADDR<br>NONE|RX_ADDR_P0<br>NONE|RX_ADDR_P1<br>"1RECV"|RX_ADDR_P2<br>"2RECV"|RX_ADDR_P5<br>"5RECV"||
||(Send Data)|->|->|->|->|->|->|->|(Get Data)|Data to Receiver|
|||(Get Ack)|<-|<-|<-|<-|<-|<-|(Send Ack)|Ack to Sender|


# Configuration   

![config-top](https://github.com/nopnop2002/esp-idf-mirf/assets/6020549/cd5392c4-a6d5-4e55-bc8b-372050573a2b)

![config-app](https://github.com/nopnop2002/esp-idf-mirf/assets/6020549/e289c28c-72e5-4490-9db1-40163c9db5a0)

# Receiver Register
RX_ADDR_P1 is 0x3152454356.   
RX_ADDR_P2 will be 0x3252454356. The same value is used for the last 4 bytes.
![Register-Receiver](https://github.com/nopnop2002/esp-idf-mirf/assets/6020549/e8e0812a-8fa2-43ae-a2be-fd346c2e6da2)

# Communicat with Arduino Environment   
Run this sketch.   
ArduinoCode\Multiple-Receive/EmitterX   


# Receiver screenshot    

This is received from 5 units at the same time.   
![ScreenShot](https://github.com/nopnop2002/esp-idf-mirf/assets/6020549/d763f44f-9ca2-4ed8-a841-90272aa18032)

