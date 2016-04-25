#include <AltSoftSerial.h>
/**
 *  \brief Software serial port object
 *  \author volodink
 */
AltSoftSerial mySerial;

int c = 0; /**< The variable which is assigned received GPS data
 * for later comparison in getUBX
 */
uint8_t message[60]; /**< The maximum Byte Offset */
/**
 * The first argument to calculate the checksum
 */
uint8_t CK_A = 0, CK_B = 0;
/**< The second argument to calculate the checksum */
/**
 * The first argument to comparison the checksum
 */
uint8_t CK_AU = 0, CK_BU = 0; /**< The second argument to comparison the checksum */
int gpsStep = 0; /**< If the data suits us, this parameter makes the step */
uint8_t UBX_id = 0; /**< Message ID */
uint8_t UBX_class = 0; /**< Message class */
uint8_t meslenL = 0;
uint8_t meslenH = 0;
int16_t meslen = 0;
int count = 0;
boolean gotUBX = false; /**< The parameter which checks getting data */
boolean POSLLH = 0; /**< Geodetic Position Solution; This message outputs
 * the Geodetic position in the currently selected Ellipsoid.
 */
boolean SVINFO = 0;/**< Space Vehicle Information */
boolean SOL = 0;/**< Navigation Solution Information; This message combines Position,
 * velocity and time solution in ECEF, including accuracy figures
 */
boolean VELNED = 0;/**< Velocity Solution in NED */
char GPSdata[40];
long int longitude; /**< Longitude */
float longitudef; /**< Output longitude */
long int latitude; /**< Latitude */
float latitudef; /**< Output latitude */
long int height; /**< Height */
float heightf; /**< Output height */
long int speed3; /**< Longitude */
float speed3f; /**< Speed */
/**
 * GPSfix Type, range 0..5;
 * Is a measure of the communication
 * required for a GPS receiver to acquire
 * satellite signals and navigation data
 */
short int FixType;
short int NumSV; /**< Number of SVs used in Nav Solution */
/**
 * \brief uBlox checksum algorithm
 * \param ubx_data Contains the data over
 * which the checksum is to be calculated.
 */
void ubx_checksum(byte ubx_data) //рассчет контрольной суммы
{
    CK_A += ubx_data;
    CK_B += CK_A;
}
/**
 * \brief Getting and comparing data
 */
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

/**
 * \brief UBX-decoder
 */
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
/**
* \brief Function output data
*/
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
/**
* \brief Data cleansing
*/
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
/**
 *  \brief Starts only 1 time after reset
 *  \author volodink
 */
void setup() {
    Serial.begin(57600); /**< setup hardware serial for GPS side */
    mySerial.begin(9600);/**< setup hardware serial for GPS side */
}
/**
 *  \brief Running in the infinite loop
 *  \author volodink
 */
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
