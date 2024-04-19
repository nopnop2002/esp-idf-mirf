# Peer-to-peer Example   
One-way communication for transmission or reception.   

# Configuration   

![config-top](https://user-images.githubusercontent.com/6020549/154790249-b1f28d18-7c60-4a55-b262-5d821adbbfc3.jpg)

![config-app](https://github.com/nopnop2002/esp-idf-mirf/assets/6020549/73e39ee3-9f25-44de-93d4-b46f70ce4c14)

# Parameter Setting
The following parameters must match on the sender and receiver.   

## Payload size & channel
```
	uint8_t payload = 32;
	uint8_t channel = CONFIG_RADIO_CHANNEL;
```

## Sender side
```
    // Set destination address using 5 characters
    esp_err_t ret = Nrf24_setTADDR(&dev, (uint8_t *)"FGHIJ");
```

## Receiver side
```
    // Set my own address using 5 characters
    esp_err_t ret = Nrf24_setRADDR(&dev, (uint8_t *)"FGHIJ");
```

# nRF24L01 Address Register Setting
|Sender||||Receiver||||
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|TX_ADDR<br>"FGHIJ"|RX_ADDR_P0<br>"FGHIJ"|RX_ADDR_P1<br>NONE||TX_ADDR<br>NONE|RX_ADDR_P0<br>NONE|RX_ADDR_P1<br>"FGHIJ"||
|(Send Data)|->|->|->|->|->|(Get Data)|Data to Receiver|
||(Get Ack)|<-|<-|<-|<-|(Send Ack)|Ack to Sender|

# Setting Register
The underlined address match on the sending and receiving sides.   
No other addresses are used.   

### Sender Register
![Register-Sender](https://github.com/nopnop2002/esp-idf-mirf/assets/6020549/b534f7b9-f3b5-4bb3-a065-716dc81f5888)

### Receiver Register
![Register-Receiver](https://github.com/nopnop2002/esp-idf-mirf/assets/6020549/4025b2c5-24fe-4fc2-8e9c-8ab38d301149)

# Screen Shot
![Peer-to-peer](https://github.com/nopnop2002/esp-idf-mirf/assets/6020549/f0a7a3f8-692b-4890-a0eb-97244c4e64db)


# Communicat with Arduino Environment   
Run this sketch.   
ArduinoCode\Peer-to-peer\StringTest   

