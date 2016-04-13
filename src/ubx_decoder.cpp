#include <Arduino.h>
#include <SoftwareSerial.h>

/**
 *  \author volodink
 *  \brief define Arduino led on pin 13 as system led for status display (blink me twice, if you hear me)
 */
#define SYS_LED 13

/**
 *  \brief Software serial port object
 *  \author volodink
 */
SoftwareSerial serial(8,9);

/**
 *  \brief Starts only 1 time after reset
 *  \author volodink
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
 *  \brief Running in the infinite loop
 *  \author volodink
 */
void loop() {
    serial.println("hello");
    delay(1000);
}