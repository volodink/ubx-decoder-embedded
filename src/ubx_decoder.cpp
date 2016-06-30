#include <AltSoftSerial.h>

/**
 *  \brief Software serial port object
 *  \author volodink
 */
AltSoftSerial mySerial;

volatile uint8_t message[512]; /**<  Maximum message size */

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
uint8_t meslenL = 0; /**< variable, with which compare a message */
uint8_t meslenH = 0; /**< variable, which shifts to the left */
int16_t meslen = 0; /**< variable, which assigned a message to its further comparisons */
int count = 0; /**< Message size */
boolean gotUBX = false; /**< The parameter which checks getting data */
boolean POSLLH = 0; /**< Geodetic Position Solution; This message outputs
 * the Geodetic position in the currently selected Ellipsoid.
 */
boolean SVINFO = 0;/**< Space Vehicle Information */
boolean SOL = 0;/**< Navigation Solution Information; This message combines Position,
 * velocity and time solution in ECEF, including accuracy figures
 */
boolean VELNED = 0;/**< Velocity Solution in NED */
long int longitude; /**< Longitude */
float longitudef; /**< Output longitude */
long int latitude; /**< Latitude */
float latitudef; /**< Output latitude */
long int height; /**< Height */
float heightf; /**< Output height */
long int speed3; /**< Speed */
float speed3f; /**< Output speed */
long int heading; /**< Heading */
float headingf; /**< Output heading */
long int hAcc; /**< Horizontal accuracy */
float hAccf; /**< Output horizontal accuracy */
long int vAcc; /**< Vertical accuracy */
float vAccf; /**< Output vertical accuracy */
long int pAcc; /**< 3D accuracy */
float pAccf; /**< Output 3D accuracy */
char bufer[20];

/**
 * GPSfix Type, range 0..5;
 * Is a measure of the communication
 * required for a GPS receiver to acquire
 * satellite signals and navigation data
 */
unsigned char FixType;
unsigned char NumSVs; /**< Number of SVs used in Nav Solution */

int ledPin = 13; /**< working pin */
int fixStep = 0; /**< If parameter FixType is 0x02 or 0x03, this parameter makes the step */
int nofixStep = 0; /**< If parameter FixType is not 0x02 or 0x03, this parameter makes the step */

void clearVars(void);

/**
 * \brief uBlox checksum algorithm
 * \param ubx_data Contains the data over
 * which the checksum is to be calculated.
 */
void ubx_checksum(byte ubx_data) { //рассчет контрольной суммы
    CK_A += ubx_data;
    CK_B += CK_A;
}

/**
 * \brief Getting and comparing data
 */
void getUBX(void) { //Получаем пакет и делаем с ним штуки
    if (Serial.available() > 0) {
        uint8_t c = Serial.read();
        switch (gpsStep) {
        case 0:

            if (c == 0xB5) {
                gpsStep++;
                //                mySerial.print("Start 0xB5 ");
            }
            break;

        case 1:

            if (c == 0x62) {
                gpsStep++;
                //                          mySerial.print("Start 0x62 ");
            } else {
                gpsStep = 0;
                //                        mySerial.println(" Error not62 ");
            }
            break;

        case 2:

            UBX_class = c;
            ubx_checksum(c);
            gpsStep++;
            //                   mySerial.println(""); mySerial.print(" UBX_CLASS = "); mySerial.println(UBX_class);
            break;

        case 3:

            UBX_id = c;
            ubx_checksum(c);
            gpsStep++;
            //                     mySerial.println(""); mySerial.print(" UBX_id = "); mySerial.println(UBX_id);
            break;

        case 4:

            meslenL = c;
            ubx_checksum(c);
            gpsStep++;
            //                 mySerial.print(" ml ");
            break;

        case 5:

            meslenH = c;
            ubx_checksum(c);
            gpsStep++;
            meslen = 0xFF & meslenL;
            meslen |= meslenH << 8;
            count = 0;
            //               mySerial.print(" mh ");
            //                mySerial.print("mlen=");
            //                mySerial.print(meslen);
            break;

        case 6:

            message[count] = c;
            ubx_checksum(c);
            count++;
            if (count == meslen) {
                gpsStep++;
                count = 0;
            }
            break;

        case 7:

            CK_AU = c;
            gpsStep++;


            break;

        case 8:

            CK_BU = c;


            if (CK_A == CK_AU && CK_B == CK_BU) { //сравнение контрольной суммы
                gotUBX = true;
                gpsStep++;
            }

            break;

        }
    }
}

/**
 * \brief UBX-decoder
 */
void decodeUBX(void) {
    if (UBX_class == 0x01) {
        switch (UBX_id) {
        case 0x02:  //NAV-POSLLH
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

            hAcc = 0xFF & message[23];          //гор. точность
            hAcc = hAcc << 8;
            hAcc |= message[22];
            hAcc = hAcc << 8;
            hAcc |= message[21];
            hAcc = hAcc << 8;
            hAcc |= message[20];
            hAccf = hAcc / 1000.0;

            vAcc = 0xFF & message[27];          //верт. точность
            vAcc = vAcc << 8;
            vAcc |= message[26];
            vAcc = vAcc << 8;
            vAcc |= message[25];
            vAcc = vAcc << 8;
            vAcc |= message[24];
            vAccf = vAcc / 1000.0;

            POSLLH = true;
            //            mySerial.println("POSLLH got.");
            break;
        case 0x12:  //NAV-VELNED
            speed3 = 0xFF & message[19];        //скорость
            speed3 = speed3 << 8;
            speed3 |= message[18];
            speed3 = speed3 << 8;
            speed3 |= message[17];
            speed3 = speed3 << 8;
            speed3 |= message[16];
            speed3f = speed3 / 100.0;
            
            heading = 0xFF & message[27];
            heading = heading << 8;
            heading |= message[26];
            heading = heading << 8;
            heading |= message[25];
            heading = heading << 8;
            heading |= message[24];
            headingf = heading / 100000.0;
            

            VELNED = true;
            //         mySerial.println("VELNED got.");
            break;
        case 0x06:  // NAV-SOL
            NumSVs = message[47];          //кол-во спутников в решении
            FixType = message[10];        //тип фикса

            pAcc = 0xFF & message[27];        //3D точность
            pAcc = pAcc << 8;
            pAcc |= message[26];
            pAcc = pAcc << 8;
            pAcc |= message[25];
            pAcc = pAcc << 8;
            pAcc |= message[24];
            pAccf = pAcc / 100.0;

            if (FixType == 0x02 || FixType == 0x03) {
                fixStep++;
                nofixStep = 0;
            } else {
                nofixStep++;
                fixStep = 0;
            }

            if (fixStep == 12) digitalWrite(ledPin, HIGH);
            if (nofixStep == 12) digitalWrite(ledPin, LOW);

            //       mySerial.println(NumSVs);
            //       mySerial.println(FixType);

            SOL = true;

            //        mySerial.println("SOL got.");

            break;
            //      default:
//            mySerial.print("Unknown ubx id = ");
//           mySerial.println(UBX_id);

        }
    }
}

/**
* \brief Function output data
*/
void sendGPS(void) {
    mySerial.print("A");
    mySerial.print(longitudef, 6);

    mySerial.print("B");
    mySerial.print(latitudef, 6);

    mySerial.print("C");
    mySerial.print(heightf, 3);

    mySerial.print("D");
    mySerial.print(speed3f, 3);

    mySerial.print("E");
    mySerial.print(vAccf, 3);

    mySerial.print("F");
    mySerial.print(hAccf, 3);

    mySerial.print("G");
    mySerial.print(pAccf);
    
    mySerial.print("H");
    mySerial.print(headingf, 2)

    mySerial.print("I");
    mySerial.print(NumSVs);

    mySerial.print("J");
    mySerial.println(FixType);

    longitude = 0;
    latitude = 0;
    height = 0;
    speed3 = 0;
    NumSVs = 0;
    FixType = 0;
}
/**
* \brief Data cleansing
*/
void clearVars(void) {
    gpsStep = 0;
    UBX_id = 0;
    UBX_class = 0;
    meslen = 0;
    count = 0;
    CK_AU = 0;
    CK_BU = 0;
    CK_A = 0;
    CK_B = 0;
    for(int i = 0; i < 60; i++) {
        message[i] = 0;
    }
}
/**
 *  \brief Starts only 1 time after reset
 *  \author volodink
 */
void setup() {
    Serial.begin(57600); /**< setup hardware serial for GPS side */
    mySerial.begin(9600);/**< setup hardware serial for GPS side */
    pinMode(13, OUTPUT);
    mySerial.println("UBX ready.");
    clearVars();
}
/**
 *  \brief Running in the infinite loop
 *  \author volodink
 */
void loop() {
    //mySerial.println("enter loop");

    getUBX();
    if (gotUBX == true) {
        //    mySerial.println("got ubx packet");
        decodeUBX();
        //    mySerial.println("ubx packet decoded");
        if (POSLLH == 1 && VELNED == 1 && SOL == 1) {

            //    mySerial.println("if posllh == 1");
            sendGPS();
            POSLLH = 0;
            VELNED = 0;
            SOL = 0;
        }
        clearVars();
        gotUBX = false;
    }
}
