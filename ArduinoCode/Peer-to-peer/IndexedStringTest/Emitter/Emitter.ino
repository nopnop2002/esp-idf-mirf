//Transmitter program

#include "Mirf.h"

Nrf24l Mirf = Nrf24l(10, 9); // CE,CSN

struct Pack {
  byte index;
  char payload[31];
};

#define PackSize sizeof(Pack)

union MYDATA_t {
  byte value[PackSize];
  struct Pack pack;
};

MYDATA_t mydata;

void setup()
{
  Serial.begin(115200);
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.payload = sizeof(mydata.value); // Set the payload size
  Mirf.channel = 90;                   // Set the channel used
  Mirf.config();
  
  // Set destination address to TX_ADDR
  // Set ACK waiting address to RX_ADDR_P0
  Mirf.setTADDR((byte *)"FGHIJ");
}

void loop()
{
  static int payload = 0x41; // 'A'
  mydata.pack.index = payload - 0x41; // 0
  for (int i=0;i<30;i++) {
    mydata.pack.payload[i] = payload;
  }
  mydata.pack.payload[30] = 0;
  Mirf.send(mydata.value);
  Serial.print("Wait for sending.....");
  // Verify send was successfuly
  if (Mirf.isSend()) {
    Serial.print("Send success:");
    Serial.println(mydata.pack.index);
    payload++;
    if(payload == 0x7e) {
      payload = 0x41;
    }
  } else {
    Serial.println("Send fail:");
  }
  delay(1000);
}
