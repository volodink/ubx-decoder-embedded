#include <Arduino.h>

#include <AltSoftSerial.h>

/**
 *  \author volodink
 *  \brief define Arduino led on pin 13 as system led for status display (blink me twice, if you hear me)
 */
#define SYS_LED 13


void getUBX();

/**
 *  \brief Software serial port object
 *  \author volodink
 */
AltSoftSerial mySerial;

bool gotUBX = false;



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
    if (gotUBX == true) {
        mySerial.println("hello");
    }
}

void getUBX() {

}
