//#include <AltSoftwareSerial.h>
//SoftwareSerial mySerial(10, 11);
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
boolean VELNED = 0;
char GPSdata[40];

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
          GPSdata[0] = message[4];    //longitude-0
          GPSdata[1] = message[5];    //longitude-1
          GPSdata[2] = message[6];    //longitude-2
          GPSdata[3] = message[7];    //longitude-3

          GPSdata[4] = message[8];    //latitude-0
          GPSdata[5] = message[9];    //latitude-1
          GPSdata[6] = message[10];   //latitude-2
          GPSdata[7] = message[11];   //latitude-3

          GPSdata[8] = message[16];   //height-0
          GPSdata[9] = message[17];   //height-1
          GPSdata[10] = message[18];  //height-2
          GPSdata[11] = message[19];  //height-3

          GPSdata[12] = message[20];  //horAcc-0
          GPSdata[13] = message[21];  //horAcc-1
          GPSdata[14] = message[22];  //horAcc-2
          GPSdata[15] = message[23];  //horAcc-3

          GPSdata[16] = message[24];  //vertAcc-0
          GPSdata[17] = message[25];  //vertAcc-1
          GPSdata[18] = message[26];  //vertAcc-2
          GPSdata[19] = message[27];  //vertAcc-3

          POSLLH = true;
          break;
        }
      case 0x12:  //NAV-VELNED
        {
          GPSdata[20] = message[16];  //speed-0
          GPSdata[21] = message[17];  //speed-1
          GPSdata[22] = message[18];  //speed-2
          GPSdata[23] = message[19];  //speed-3

          GPSdata[24] = message[20];  //gSpeed-0
          GPSdata[25] = message[21];  //gSpeed-1
          GPSdata[26] = message[22];  //gSpeed-2
          GPSdata[27] = message[23];  //gSpeed-3

          GPSdata[28] = message[24];  //heading-0
          GPSdata[29] = message[25];  //heading-1
          GPSdata[30] = message[26];  //heading-2
          GPSdata[31] = message[27];  //heading-3

          GPSdata[32] = message[28];  //speedAcc-0
          GPSdata[33] = message[29];  //speedAcc-1
          GPSdata[34] = message[30];  //speedAcc-2
          GPSdata[35] = message[31];  //speedAcc-3

          GPSdata[36] = message[32];  //headingAcc-0
          GPSdata[37] = message[33];  //headingAcc-1
          GPSdata[38] = message[34];  //headingAcc-2
          GPSdata[39] = message[35];  //headingAcc-3

          VELNED = true;
          break;
        }
    }
  }
  if (POSLLH == 1 && VELNED == 1)
  {
    mySerial.println("Ypa!");
    POSLLH = 0;
    VELNED = 0;
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

void setup() {
  Serial.begin(57600);
  mySerial.begin(9600);
}

void loop()
{
  getUBX();
  if (gotUBX == true)
  {
    mySerial.println("Got it!");
    decodeUBX();
    clearVars();
    gotUBX = false;
  }
}
