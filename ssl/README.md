# SSL Example   
This is nRF24L01 and SSL gateway application.   
Receive from nRF24L01 and send to SSL Server.   
ESP32 acts as SSL Client.   

```
            +-----------+           +-----------+           +-----------+
            |           |           |           |           |           |
==(Radio)==>| nRF24L01  |--(SPI)--->|   ESP32   |--(SSL)--->| SSL Server|
            |           |           |           |           |           |
            +-----------+           +-----------+           +-----------+
```

# Installation
```
git clone https://github.com/nopnop2002/esp-idf-mirf
cd esp-idf-mirf/ssl/
chmod 777 mkkey.sh
./mkkey.sh
idf.py menuconfig
idf.py flash
```

mkkey.sh creates a server certificate file.   
The server certificate file is associated with the SSL server's IP address.   
mkkey.sh automatically retrieves the IP address of the server on which the script is executed and treats that address as an SSL server.   
To manually configure the SSL server's IP address, modify the script as follows:   
```
IP="192.168.0.123"
openssl req -x509 -new -nodes -key server.key -subj "/CN=${IP}" -days 10000 -out server.crt
```


# Configuration
<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/d35dcb7e-5466-4e7e-968b-69a8bc3b7bbb" />
<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/b7d8fcc1-e29b-4e95-b2bf-783d502d412d" />

## WiFi Setting
Set the information of your access point.   
<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/b1976279-f618-4d11-a2c4-1759e48a43bb" />

## Server Setting
Set the information of your SSL server.   
<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/1a3aaa84-2e0a-4c27-af74-785fd938ab8d" />

# Start the SSL server
- C language
	```
	cd clang-tls-communication
	make
	./server
	```
	<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/228834a5-0a90-426b-99fd-5de37cdffb04" />

- python script
	```
	cd python-tls-communication
	python3 server.py
	```
	<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/8eb08d95-a308-4ed8-a5c4-c67d15e45f8b" />
