//Receiver program

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
  Mirf.setRADDR((byte *)"1RECV");
  // Add my own address to RX_ADDR_P2
  Mirf.addRADDR(2, '2'); // 2RECV
  // Add my own address to RX_ADDR_P3
  Mirf.addRADDR(3, '3'); // 3RECV
  // Add my own address to RX_ADDR_P4
  Mirf.addRADDR(4, '4'); // 4RECV
  // Add my own address to RX_ADDR_P5
  Mirf.addRADDR(5, '5'); // 5RECV

  // Clear RX FiFo
  while(1) {
    if (Mirf.dataReady() == false) break;
    Mirf.getData(mydata.value);
  }

  // Print current settings
  printf_begin();
  Mirf.printDetails();
  Serial.println("Listening...");
}

void loop()
{
  // Wait for received data
  if (Mirf.dataReady()) {
    uint8_t pipe = Mirf.getDataPipe();
    Mirf.getData(mydata.value);
    Serial.print("Got data pipe(");
    Serial.print(pipe);
    Serial.print(") is: ");
    Serial.println(mydata.now_time);
  }
  delay(1);
}
