#include <Arduino.h>

#include <AltSoftSerial.h>

/**
 *  \author volodink
 *  \brief define Arduino led on pin 13 as system led for status display (blink me twice, if you hear me)
 */
#define SYS_LED 13

void getUBX();
void decodeUBX();
void ubx_checksum(byte ubx_data); //рассчет контрольной суммы
void clearVars(void);

/**
 *  \brief Software serial port object
 *  \author volodink
 */
AltSoftSerial mySerial;

int gpsStep = 0;

int count = 0;

uint8_t CK_A = 0, CK_B = 0;
uint8_t CK_AU = 0, CK_BU = 0;

uint8_t UBX_id = 0;
uint8_t UBX_class = 0;

uint8_t meslenL = 0;
uint8_t meslenH = 0;
int16_t meslen = 0;

bool gotUBX = false;
bool POSLLH = 0;
bool VELNED = 0;

uint8_t message[60];
uint8_t GPSdata[40];

/**
 *  \brief Starts only 1 time after reset
 *  \author volodink
 */
void setup() {
    // setup hardware serial for GPS side
    Serial.begin(57600);
    // setup software serial for PC side
    mySerial.begin(9600);

    // set pins direction
    pinMode(SYS_LED, OUTPUT);
}

/**
 *  \brief Running in the infinite loop
 *  \author volodink
 */
void loop() {
    getUBX();
    if (gotUBX) {
        decodeUBX();
        clearVars();
        gotUBX = false;
    }
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
                }
                break;
            }
            case 1:
            {
                if (c == 0x62){
                    gpsStep++;
                }
                else{
                    gpsStep = 0;
                }
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
                    gotUBX = true;
                }
                break;
            }
            default:
                break;

        }
    }
}

void ubx_checksum(byte ubx_data) //рассчет контрольной суммы
{
    CK_A += ubx_data;
    CK_B += CK_A;
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

            default:
                break;
        }
    }

    if (POSLLH == 1 && VELNED == 1)
    {
        mySerial.println("Ypa!");
        POSLLH = false;
        VELNED = false;
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