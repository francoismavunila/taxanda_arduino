
#include <Adafruit_Fingerprint.h>


#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
// For UNO and others without hardware serial, we must use software serial...
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
// Set up the serial port to use softwareserial..
SoftwareSerial mySerial(2, 3);

#else
// On Leonardo/M0/etc, others with hardware serial, use hardware serial!
// #0 is green wire, #1 is white
#define mySerial Serial2

#endif


Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

//Adafruit_Fingerprint finger2 = Adafruit_Fingerprint(&mySerial);

bool checkSensor(){
   if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
    return true;
   } else {
    Serial.println("Did not find fingerprint sensor :(");
    return false;
   }
}
