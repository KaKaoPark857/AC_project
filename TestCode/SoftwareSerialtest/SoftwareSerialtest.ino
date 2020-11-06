#include <SoftwareSerial.h>

// Swap RX/TX connections on bluetooth chip
//   Pin 2 --> Bluetooth TX
//   Pin 3 --> Bluetooth RX
SoftwareSerial mySerial(2, 3); // RX, TX

void setup()
{
  Serial.begin(9600);   //Original Serial
  mySerial.begin(9600); //Software Serial  
}

void loop() {
  //It is waiting while software serial buffer is available
  if(mySerial.available()) { 
    Serial.write(mySerial.read());
  }

  //It is waiting while original serial buffer is available
  if(Serial.available()) {
    mySerial.write(Serial.read());
  }

}
