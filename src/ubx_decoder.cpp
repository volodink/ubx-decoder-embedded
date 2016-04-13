#include <Arduino.h>
#include <SoftwareSerial.h>

/**
 *  \author volodink
 *  \brief define Arduino led on pin 13 as system led for status display (blink me twice, if you hear me)
 */
#define SYS_LED 13

/**
 *  \author volodink
 */
SoftwareSerial serial(8,9);

/**
 * \author volodink
 */
void setup() {
    // setup hardware serial
    Serial.begin(57600);
    // setup software serial
    serial.begin(9600);

    // set pins direction
    pinMode(SYS_LED, OUTPUT);
}

/**
 *  \author volodink
 */
void loop() {
    serial.println("hello");
    delay(1000);
}