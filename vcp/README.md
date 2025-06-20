# VCP Example   
This is nRF24L01 and VCP(Virtual Com Port) gateway application.   

ESP-IDF supports VCP hosts.   
VCP hosts can communicate with VCP devices using the USB port.   
Representative VCP devices include Arduino Uno and Arduino Mega, which have a UART-USB conversion chip.   
I based it on [this](https://github.com/espressif/esp-idf/tree/master/examples/peripherals/usb/host/cdc/cdc_acm_vcp).   

This project uses the following components.   
Other UART-USB converter chips are not supported.   
- https://components.espressif.com/components/espressif/usb_host_ch34x_vcp   
- https://components.espressif.com/components/espressif/usb_host_cp210x_vcp   
- https://components.espressif.com/components/espressif/usb_host_ftdi_vcp   


```
            +------------+          +------------+          +------------+
            | VCP device |          |            |          |            |           |
            |Arduino Uno |--(USB)-->|    ESP32   |--(SPI)-->| nRF24L01   |==(Radio)==>
            |Arduino Mega|          |            |          |            |
            +------------+          +------------+          +------------+

            +------------+          +------------+          +------------+
            |            |          |            |          | VCP device |
==(Radio)==>| nRF24L01   |--(SPI)-->|    ESP32   |--(USB)-->|Arduino Uno |
            |            |          |            |          |Arduino Mega|
            +------------+          +------------+          +------------+
```


# Hardtware requirements

## ESP32-S2/S3
This project only works with ESP32S2/S3.   
The ESP32S2/S3 has USB capabilities.   

## USB Type-A Femail connector
USB connectors are available from AliExpress or eBay.   
I used it by incorporating it into a Universal PCB.   
![USBConnector](https://github.com/user-attachments/assets/8d7d8f0a-d289-44b8-ae90-c693a1099ca0)

We can buy this breakout on Ebay or AliExpress.   
![usb-conector-11](https://github.com/user-attachments/assets/848998d4-fb0c-4b4f-97ae-0b3ae8b8996a)
![usb-conector-12](https://github.com/user-attachments/assets/6fc34dcf-0b13-4233-8c71-07234e8c6d06)


# Configuration
![Image](https://github.com/user-attachments/assets/5116949f-5f68-4203-825d-692016c2edab)
![Image](https://github.com/user-attachments/assets/ad00fe0b-10ed-4cfc-80a9-063d5a649456)

## Radio Setting

### VCP to Radio
Receive from VCP device and send to Radio.   
ESP32 acts as the VCP host.   
```
            +------------+          +------------+          +------------+
            | VCP device |          |            |          |            |           |
            |Arduino Uno |--(USB)-->|    ESP32   |--(SPI)-->| nRF24L01   |==(Radio)==>
            |Arduino Mega|          |            |          |            |
            +------------+          +------------+          +------------+
```

![Image](https://github.com/user-attachments/assets/75a8d620-fdf5-439f-a99b-e77e8d549324)


### Radio to VCP
Receive from Radio and send to VCP device.   
ESP32 acts as the VCP host.   
```
            +------------+          +------------+          +------------+
            |            |          |            |          | VCP device |
==(Radio)==>| nRF24L01   |--(SPI)-->|    ESP32   |--(USB)-->|Arduino Uno |
            |            |          |            |          |Arduino Mega|
            +------------+          +------------+          +------------+
```

![Image](https://github.com/user-attachments/assets/f59f37b5-c77d-4f2a-b910-02a489f0a42b)


## VCP Setting

![Image](https://github.com/user-attachments/assets/1adb68c5-e115-4180-a2b7-cdf8e32f4301)


# Write this sketch on Arduino Uno.   
You can use any AtMega microcontroller that has a USB port.   

```
const int LED_PIN = 13;
unsigned long lastMillis = 0;

void setup() {
  Serial.begin(115200);
  pinMode( LED_PIN, OUTPUT );
}

void loop() {
  while (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    digitalWrite( LED_PIN, !digitalRead(LED_PIN) );
  }

  if(lastMillis + 1000 <= millis()){
    Serial.print("Hello World ");
    Serial.println(millis());
    lastMillis += 1000;
  }

  delay(1);
}
```

Strings from Arduino to ESP32 are terminated with CR(0x0d)+LF(0x0a).   
This project will remove the termination character and send to Radio.   
```
I (24869) VCP: Receiving data through CdcAcmDevice
I (24869) VCP: 0x3fca1804   48 65 6c 6c 6f 20 57 6f  72 6c 64 20 34 30 31 30  |Hello World 4010|
I (24869) VCP: 0x3fca1814   30 30 0d 0a                                       |00..|
I (25429) SEND: xMessageBufferReceive received=18
I (25429) SEND: 0x3fc9d970   48 65 6c 6c 6f 20 57 6f  72 6c 64 20 34 30 31 30  |Hello World 4010|
I (25429) SEND: 0x3fc9d980   30 30 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |00..............|
```

The Arduino sketch inputs data with LF as the terminator.   
So strings from the ESP32 to the Arduino must be terminated with LF (0x0a).   
If the string output from the ESP32 to the Arduino is not terminated with LF (0x0a), the Arduino sketch will complete the input with a timeout.   
The default input timeout for Arduino sketches is 1000 milliseconds.   
This project will add a LF to the end and send to Arduino.   
The Arduino sketch will blink the on-board LED.   
```
I (2269) RECEIVE: Got data:now is 11028188
I (2269) RECEIVE: 0x3fc9d860   6e 6f 77 20 69 73 20 31  31 30 32 38 31 38 38 00  |now is 11028188.|
I (2269) RECEIVE: 0x3fc9d870   00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
I (2279) VCP: Sending data through CdcAcmDevice
I (2289) VCP: 0x3fc9ef40   6e 6f 77 20 69 73 20 31  31 30 32 38 31 38 38     |now is 11028188|
I (2289) VCP: 0x3fc9ef40   6e 6f 77 20 69 73 20 31  31 30 32 38 31 38 38 0a  |now is 11028188.|
```

# Wireing   
Arduino Uno connects via USB connector.   
The USB port on the ESP32S2/S3 development board does not function as a USB-HOST.   

```
+---------+  +-------------+  +-----------+
|ESP BOARD|==|USB CONNECTOR|==|Arduino Uno|
+---------+  +-------------+  +-----------+
```

```
ESP BOARD          USB CONNECTOR (type A)
                         +--+
5V        -------------> | || VCC
[GPIO19]  -------------> | || D-
[GPIO20]  -------------> | || D+
GND       -------------> | || GND
                         +--+
```

![Image](https://github.com/user-attachments/assets/7bf405af-b1ec-4c7c-87d1-8bbe176e807b)

