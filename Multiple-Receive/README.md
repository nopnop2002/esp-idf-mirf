# Multiple Receive Example   
The nRF24L01 has 6 receive data pipes(PTX1-PTX6).   
In this library, the first datapipe(PTX1) is used for automatic ACK reception on transmission.   
The second (PTX2) to sixth (PTX6) data pipes are used to transmit the payload.   
Therefore, it is possible to receive from up to 5 senders simultaneously.

![MultiCast](https://github.com/nopnop2002/esp-idf-mirf/assets/6020549/14ac392c-fc93-43d9-9575-39696bbab59e)

In this example, it will receive from the following addresses:   
```1RECV/2RECV/3RECV/4RECV/5RECV```

# Configuration   

![config-top](https://github.com/nopnop2002/esp-idf-mirf/assets/6020549/cd5392c4-a6d5-4e55-bc8b-372050573a2b)

![config-app](https://github.com/nopnop2002/esp-idf-mirf/assets/6020549/e289c28c-72e5-4490-9db1-40163c9db5a0)

# Communicat with Arduino Environment   
Run this sketch.   
ArduinoCode\Multiple-Receive/EmitterX   


# Receiver screenshot    

This is received from 5 units at the same time.   
![ScreenShot](https://github.com/nopnop2002/esp-idf-mirf/assets/6020549/d763f44f-9ca2-4ed8-a841-90272aa18032)

