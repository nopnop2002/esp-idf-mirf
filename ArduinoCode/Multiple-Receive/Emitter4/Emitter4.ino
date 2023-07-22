//Transmitter program

#include "Mirf.h"

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
  Mirf.channel = 90;                   // Set the channel used
  Mirf.config();

  //Set the receiver address using 5 characters
  Mirf.setTADDR((byte *)"4RECV");
}

void loop()
{
  sprintf(mydata.now_time,"now is %lu",micros());
  Mirf.send(mydata.value);
  Serial.print("Wait for sending.....");
  //Test you send successfully
  if (Mirf.isSend()) {
    Serial.print("Send success:");
    Serial.println(mydata.now_time);
  } else {
    Serial.println("Send fail:");
  }
  delay(1000);
}
