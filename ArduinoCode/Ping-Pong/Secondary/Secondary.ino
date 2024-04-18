//secondary program

#include "Mirf.h"
#include "printf.h"

Nrf24l Mirf = Nrf24l(10, 9); // CE,CSN

union MYDATA_t {
  byte value[32];
  char now_time[32];
};

MYDATA_t mydata;

void setup()
{
  Serial.begin(115200);
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.payload = sizeof(mydata.value); // Set the payload size
  Mirf.channel = 90;                   // Set the used channel
  Mirf.config();

  // Set my own address to RX_ADDR_P1
  Mirf.setRADDR((byte *)"FGHIJ");

  // Set destination address to TX_ADDR
  // Set ACK waiting address to RX_ADDR_P0
  Mirf.setTADDR((byte *)"ABCDE");

  // Print current settings
  printf_begin();
  Mirf.printDetails();
  Serial.println("Listening...");

  // Clear RX FiFo
  while(1) {
    if (Mirf.dataReady() == false) break;
    Mirf.getData(mydata.value);
  }

}

void loop()
{
  // Wait for received data
  if (Mirf.dataReady()) {
    Mirf.getData(mydata.value);
    Serial.print("Got string: ");
    Serial.println(mydata.now_time);

    delay(10);
    Mirf.send(mydata.value);
    Serial.print("Wait for sending.....");
    // Verify send was successful
    if (Mirf.isSend()) {
      Serial.println("Send success:");
    } else {
      Serial.println("Send fail:");
    }
    
  }
}
