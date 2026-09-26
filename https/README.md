# HTTPS Example   
This is nRF24L01 and HTTPS gateway application.   
Receive from nRF24L01 and send to HTTPS Server.   
ESP32 acts as HTTPS Client.   

```
            +-----------+           +-----------+            +------------+
            |           |           |           |            |            |
==(Radio)==>| nRF24L01  |--(SPI)--->|   ESP32   |--(HTTPS)-->|HTTPS Server|
            |           |           |           |            |            |
            +-----------+           +-----------+            +------------+
```

# Installation
```
git clone https://github.com/nopnop2002/esp-idf-mirf
cd esp-idf-mirf/https/
chmod 777 mkkey.sh
./mkkey.sh
idf.py menuconfig
idf.py flash
```

mkkey.sh creates a server certificate file.   
The server certificate file is associated with the HTTPS server's IP address.   
mkkey.sh automatically retrieves the IP address of the server on which the script is executed and treats that address as an HTTPS server.   
To manually configure the HTTPS server's IP address, modify the script as follows:   
```
IP="192.168.0.123"
openssl req -x509 -new -nodes -key server.key -subj "/CN=${IP}" -days 10000 -out server.crt
```


# Configuration
<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/7ea9245b-2bb6-4f48-8d30-c572bb367d6e" />
<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/d4fcb658-1e17-424f-83e8-a69696135342" />

## WiFi Setting
Set the information of your access point.   
<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/3d68c221-af93-4c7f-b59f-e2907aa985a2" />

## Server Setting
Set the information of your HTTPS server.   
<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/0d24a06b-1c7f-47db-9535-babb0eabbe24" />

# Start the HTTPS server
```
python3 https-server.py
```
<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/1cf491d3-d574-4a37-ba91-22fcfb69a96a" />
