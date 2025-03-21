# Multiple Receive Example   
nRF24L01 has 6 receive data pipes (RX_ADDR_P0-P6).   
In this library, the first data pipe(RX_ADDR_P0) is used for automatic ACK reception on transmission.   
The second(RX_ADDR_P1) to sixth(RX_ADDR_P5) data pipes are used for data reception.   
Therefore, it is possible to receive from a maximum of five transmitting sides.   
This example receive from ```1RECV/2RECV/3RECV/4RECV/5RECV```.   
![Image](https://github.com/user-attachments/assets/bf6902a2-f7cc-4fe0-bb1c-ee5d78950765)

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

## As Sender
You can configure up to five senders.   
![config-sender](https://github.com/user-attachments/assets/bec1f1a6-525a-4271-b3fa-5c1a0cca61a5)

## As Receiver
![config-receiver](https://github.com/user-attachments/assets/b249e6ce-d903-4ddb-871d-67686dc87f67)

# Receiver Register
RX_ADDR_P1 is a 10-byte register and the listening address is 0x3152454356.   
RX_ADDR_P2 through RX_ADDR_P6 are 2-byte registers.   
RX_ADDR_P2 will be 0x3252454356.   
The last 8 bytes use the same value as RX_ADDR_P1.   
When performing multiple receives, the sender must follow these rules.   
In other words, you can communicate with 1RECV/2RECV/3RECV/4RECV/5RECV, but you cannot communicate with RECV1/RECV2/RECV3/RECV4/RECV5.   

|RX_ADDR|Byte0|Byte1|Byte2|Byte3|Byte4|
|:-:|:-:|:-:|:-:|:-:|:-:|
|P1|0x31|0x52|0x45|0x43|0x56|
|P2|0x32|(0x52)|(0x45)|(0x43)|(0x56)|
|P3|0x33|(0x52)|(0x45)|(0x43)|(0x56)|
|P4|0x34|(0x52)|(0x45)|(0x43)|(0x56)|
|P5|0x35|(0x52)|(0x45)|(0x43)|(0x56)|

![Register-Receiver](https://github.com/nopnop2002/esp-idf-mirf/assets/6020549/e8e0812a-8fa2-43ae-a2be-fd346c2e6da2)

# Communicat with Arduino Environment   
Run this sketch.   
ArduinoCode\Multiple-Receive/EmitterX   


# Receiver screenshot    

This is received from 5 units at the same time.   
![ScreenShot](https://github.com/nopnop2002/esp-idf-mirf/assets/6020549/d763f44f-9ca2-4ed8-a841-90272aa18032)

