#include <AltSoftSerial.h>

AltSoftSerial mySerial;
int c = 0;
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
boolean POSLLH = 0;
boolean SVINFO = 0;
boolean SOL = 0;
boolean VELNED = 0;
char GPSdata[40];
long int longitude;
float longitudef;
long int latitude;
float latitudef;
long int height;
float heightf;
long int speed3;
float speed3f;
short int FixType;
short int NumSV;

void ubx_checksum(byte ubx_data) //рассчет контрольной суммы
{
  CK_A += ubx_data;
  CK_B += CK_A;
}

void getUBX(void) //Получаем пакет и делаем с ним штуки
{
  if (Serial.available())
  {
    uint8_t c = Serial.read();
    switch (gpsStep)
    {
      case 0:
        {
          if (c == 0xB5){
            gpsStep++;
   //         mySerial.print("0");
          }
          break;
        }
      case 1:
        {
          if (c == 0x62){
            gpsStep++;
  //          mySerial.print("1");
          }
          else{
            gpsStep = 0;
    //        mySerial.println("not62");
          }
          break;
        }
      case 2:
        {
          UBX_class = c;
          ubx_checksum(c);
          gpsStep++;
    //        mySerial.print("2");
          break;
        }
      case 3:
        {
          UBX_id = c;
          ubx_checksum(c);
          gpsStep++;
   //         mySerial.print("3");
          break;
        }
      case 4:
        {
          meslenL = c;
          ubx_checksum(c);
          gpsStep++;
 //           mySerial.print("ml");
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
  //          mySerial.print("mh");
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
            gotUBX = true;
          }
      //    clearVars();
          break;
        }
    }
  }
}

void decodeUBX(void)
{
  if (UBX_class == 0x01)
  {
    switch (UBX_id)
    {
      case 0x02:  //NAV-POSLLH
        {
          longitude = 0xFF & message[7];       //долгота
          longitude = longitude << 8;
          longitude |= message[6];
          longitude = longitude << 8;
          longitude |= message[5];
          longitude = longitude << 8;
          longitude |= message[4];
          longitudef = longitude / 10000000.0;

          latitude = 0xFF & message[11];       //широта
          latitude = latitude << 8;
          latitude |= message[10];
          latitude = latitude << 8;
          latitude |= message[9];
          latitude = latitude << 8;
          latitude |= message[8];
          latitudef = latitude / 10000000.0;

          height = 0xFF & message[19];         //высота
          height = height << 8;
          height |= message[18];
          height = height << 8;
          height |= message[17];
          height = height << 8;
          height |= message[16];
          heightf = height / 1000.0;
          
          POSLLH = true;
          break;
        }
      case 0x12:  //NAV-VELNED
        {
          speed3 = 0xFF & message[19];        //скорость
          speed3 = speed3 << 8;
          speed3 |= message[18];
          speed3 = speed3 << 8;
          speed3 |= message[17];
          speed3 = speed3 << 8;
          speed3 |= message[16];
          speed3f = speed3 / 100.0;

          VELNED = true;
          break;
        }
      case 0x06:  // NAV-SOL
        {
          NumSV = message[47];          //кол-во спутников в решении
          FixType |= message[10];        //тип фикса
   //     mySerial.print(message[47]);
          
          SOL = true;
          break;
        }
    }
  }
}

void sendGPS(void)
{
  mySerial.print("A");
  mySerial.print(longitudef, 5);
  mySerial.print("  B");
  mySerial.print(latitudef, 5);
  mySerial.print("  C");
  mySerial.print(heightf, 3);
  mySerial.print("  D");
  mySerial.print(speed3f, 3);
  mySerial.print("  E");
  mySerial.print(NumSV);
  mySerial.print("  F");
  mySerial.println(FixType);
  longitude = 0;
  latitude = 0;
  height = 0;
  speed3 = 0;
  NumSV = 0;
  FixType = 0;
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

void setup() {
  Serial.begin(57600);
  mySerial.begin(9600);
}

void loop()
{
  getUBX();
  if (gotUBX == true)
  {
    mySerial.print("");
    decodeUBX();
    if (POSLLH == 1 && VELNED == 1)
    {
      sendGPS();
      POSLLH = 0;
      VELNED = 0;
      SOL = 0;
    }
    clearVars();
    gotUBX = false;
  }
}
