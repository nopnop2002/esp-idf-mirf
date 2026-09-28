# Web Form Example   
Data entered through the web form is sent to Radio.   
Data received from Radio is displayed in a web form.   
```
            +-----------+              +-----------+              +-----------+
            |           |              |           |              |           |
            |  WebForm  |-(WebSocket)->|   ESP32   |----(SPI)---->|  nRF24L01 |==(Radio)==>
            |           |              |           |              |           |
            +-----------+              +-----------+              +-----------+

            +-----------+              +-----------+              +-----------+
            |           |              |           |              |           |
==(Radio)==>|  nRF24L01 |----(SPI)---->|   ESP32   |-(WebSocket)->|  WebForm  |
            |           |              |           |              |           |
            +-----------+              +-----------+              +-----------+
```

I used [this](https://github.com/Molorius/esp32-websocket) component.   
This component can communicate directly with the browser.   
It's a great job.   

# Configuration
<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/668247ac-24af-4adf-97a5-4a5f3bec969f" />
<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/237185a6-f9ea-446b-a155-48165de86bd2" />

## WiFi Setting
Set the information of your access point.   
<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/7f5a736b-1a17-47fc-975a-5ee4e7770e61" />

## Radio Setting
Set the wireless communication direction.   

### Web to Radio
Data entered through the web form is sent to Radio.   
```
            +-----------+              +-----------+              +-----------+
            |           |              |           |              |           |
            |  WebForm  |-(WebSocket)->|   ESP32   |----(SPI)---->|  nRF24L01 |==(Radio)==>
            |           |              |           |              |           |
            +-----------+              +-----------+              +-----------+
```

<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/9a10c4e4-d4f8-4dd5-8726-b9df3d240ac8" />


### Radio to Web
Data received from Radio is displayed in a web form.   

```
            +-----------+              +-----------+              +-----------+
            |           |              |           |              |           |
==(Radio)==>|  nRF24L01 |----(SPI)---->|   ESP32   |-(WebSocket)->|  WebForm  |
            |           |              |           |              |           |
            +-----------+              +-----------+              +-----------+
```

<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/f18a7da6-da8d-493f-8b0b-fe84deac7c66" />


# Launch a web browser
Enter the following in the address bar of your web browser.   
```
http:://{IP of ESP32}/
or
http://esp32-server.local/
```

<img width="947" height="886" alt="Image" src="https://github.com/user-attachments/assets/ec228525-e7f0-47da-94bf-1b12dce5244d" />
<img width="947" height="886" alt="Image" src="https://github.com/user-attachments/assets/e02ac119-b21b-4d13-9c80-a3dd1499d285" />

### Web to Radio
Enter the data to send in the TextBox and press the Send button.   
<img width="947" height="886" alt="Image" src="https://github.com/user-attachments/assets/a19a30d0-282a-4151-9b91-8dd7096606f0" />

### Radio to Web
The received data will be displayed in the TextBox.   
The Change button changes the number of lines displayed.   
The Copy button copies the received data to the clipboard.   
<img width="947" height="886" alt="Image" src="https://github.com/user-attachments/assets/4c3fbf8a-8ac6-4e53-a648-f31dfa7b1b09" />
<img width="947" height="886" alt="Image" src="https://github.com/user-attachments/assets/55bb97cd-8f6c-4925-b500-5806ef012ece" />

# WEB Pages
WEB Pages are stored in the html folder.   
I used [this](https://bulma.io/) open source css.   
You can change root.html as you like.   


