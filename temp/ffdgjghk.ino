#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); 
int c=0;
uint8_t CK_A = 0, CK_B = 0;
void setup() {
Serial.begin(57600);
mySerial.begin(9600);
Serial.read();
}
void ubx_checksum(byte ubx_data)
{
  CK_A+=ubx_data;
  CK_B+=CK_A;
}
void loop()
{
  mySerial.write(Serial);
}


