#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); 
int c=0;
uint8_t message[60];
uint8_t CK_A = 0, CK_B = 0;
uint8_t CK_AU = 0, CK_BU = 0;
int gpsStep = 0;
uint8_t UBX_id = 0;
uint8_t UBX_class = 0;
uint8_t meslenL = 0;
uint8_t meslenH = 0;
int16_t meslen = 0;
int count = 0;
boolean gotUBX = false;
void setup() {
Serial.begin(57600);
mySerial.begin(9600);
}
void ubx_checksum(byte ubx_data) //рассчет контрольной суммы
{
  CK_A+=ubx_data;
  CK_B+=CK_A;
}
void getUBX(void) //Получаем покет и делаем с ним штуки
{  
  if (Serial.available())
  {
    uint8_t c = Serial.read();
    switch (gpsStep)
    {
      case 0:
      {
        if (c == 0xB5)
          gpsStep++;
        break;
      }
      case 1:
      {
        if (c == 0x62)
          gpsStep++;
        else
          gpsStep = 0;
        break;
      }
      case 2:
      {
        UBX_class = c;
        ubx_checksum(c);
        gpsStep++;
        break;
      }
      case 3:
      {
        UBX_id = c;
        ubx_checksum(c);
        gpsStep++;
        break;
      }
      case 4:
      {
        meslenL = c;
        ubx_checksum(c);
        gpsStep++;
        break;
      }
      case 5:
      {
        meslenH = c;
        ubx_checksum(c);
        gpsStep++;
        meslen = 0xFF & meslenL;
        meslen |= meslenH << 8;
        count = 0;
        break;
      }
      case 6:
      {
        message[count] = c;
        ubx_checksum(c);
        count++;
        if (count == meslen)
        {
          gpsStep++;
          count = 0;
        }
        break;        
      }
      case 7:
      {
        CK_AU = c;
        gpsStep++;
        break;
      }
   case 8:
      {
         CK_BU = c;
        if (CK_A == CK_AU && CK_B == CK_BU) //сравнение контрольной суммы
        {
           mySerial.print("Ypa!");
          gotUBX = true;
          }
        clearVars();
        break;
      }
      }
    }
  }  

void clearVars(void)
{
  gpsStep = 0;
  UBX_id = 0;
  UBX_class = 0;
  meslen = 0;
  count = 0;
  CK_AU = 0;
  CK_BU = 0;
  CK_A = 0;
  CK_B = 0;
  memset(message, 0, 60);
}

void loop()
{
   getUBX();
  if (gotUBX == true)
  {
 mySerial.print("Ypa!");
    gotUBX = false;
  }
}
