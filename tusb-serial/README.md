# tinyusb Example   
This is nRF24L01 and tinyusb gateway application.   
```
            +------------+          +------------+           +------------+
            |            |          |            |           |            |
            | USB Writer |--(USB)-->|    ESP32   |--(SPI)--->|  nRF24L01  |==(Radio)==>
            |            |          |            |           |            |
            +------------+          +------------+           +------------+

            +------------+           +------------+          +------------+
            |            |           |            |          |            |
==(Radio)==>|  nRF24L01  |--(SPI)--->|    ESP32   |--(USB)-->| USB Reader |
            |            |           |            |          |            |
            +------------+           +------------+          +------------+
```

# Hardware requirements
1. ESP32-S2/S3 Development board   
Because the ESP32-S2/S3 does support USB OTG.   

2. USB Connector   
I used this:   
![usb-connector](https://user-images.githubusercontent.com/6020549/124848149-3714ba00-dfd7-11eb-8344-8b120790c5c5.JPG)

```
ESP32-S2/S3 BOARD          USB CONNECTOR
                           +--+
                           | || VCC
    [GPIO 19]    --------> | || D-
    [GPIO 20]    --------> | || D+
    [  GND  ]    --------> | || GND
                           +--+
```


# Configuration
![config-top](https://github.com/user-attachments/assets/305dd670-682b-42e4-bd1e-8093ca9d9ff2)

### USB to Radio
Subscribe with USB and send to Radio.   
ESP32 acts as USB Reader.   
You can use this script as USB Writer.   
```python3 ./write.py```

```
            +------------+          +------------+           +------------+
            |            |          |            |           |            |
            | USB Writer |--(USB)-->|    ESP32   |--(SPI)--->|  nRF24L01  |==(Radio)==>
            |            |          |            |           |            |
            +------------+          +------------+           +------------+
```

![config-radio-1](https://github.com/user-attachments/assets/0cbdb9e2-a897-44ca-afe7-7007be0f41f2)

Communicate with Arduino Environment.   
Run this sketch.   
ArduinoCode\Ra01S_RX   


### Radio to USB
Receive from Radio and publish as USB.   
ESP32 acts as USB Writer.   
You can use this script as USB Reader.   
```python3 ./read.py```

```
            +------------+           +------------+          +------------+
            |            |           |            |          |            |
==(Radio)==>|  nRF24L01  |--(SPI)--->|    ESP32   |--(USB)-->| USB Reader |
            |            |           |            |          |            |
            +------------+           +------------+          +------------+
```

![config-radio-2](https://github.com/user-attachments/assets/af909869-b5e0-4038-acae-cfca06a44e9f)

Communicate with Arduino Environment.   
Run this sketch.   
ArduinoCode\Ra01S_TX   


# Using Windows Terminal Software
When you connect the USB cable to the USB port on your Windows machine and build the firmware, a new COM port will appear.   
Open a new COM port in the terminal software.   
I used TeraTerm.   
![tusb-TeraTerm](https://github.com/user-attachments/assets/b5eea94e-5228-45b4-bcad-81cce8c52479)

# Using Linux Terminal Software
When you connect the USB cable to the USB port on your Linux machine and build the firmware, a new /dev/tty device will appear.   
Open a new tty device in the terminal software.   
Most occasions, the device is /dev/ttyACM0.   
I used screen.   
![tusb-screen](https://github.com/user-attachments/assets/18a6e519-9250-4109-b05d-6bcd418bfb5b)

