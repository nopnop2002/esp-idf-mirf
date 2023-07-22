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
  Mirf.payload = sizeof(mydata.value);
  Mirf.channel = 90;             //Set the used channel
  Mirf.config();

  //Set your own address using 5 characters
  Mirf.setRADDR((byte *)"FGHIJ");

  //Set the receiver address using 5 characters
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
  if (Mirf.dataReady()) { //When the program is received, the received data is output from the serial port
    Mirf.getData(mydata.value);
    Serial.print("Got string: ");
    Serial.println(mydata.now_time);

    delay(10);
    Mirf.send(mydata.value);
    Serial.print("Wait for sending.....");
    //Test you send successfully
    if (Mirf.isSend()) {
      Serial.println("Send success:");
    } else {
      Serial.println("Send fail:");
    }
    
  }
}
