#include "enrollPassenger.h"

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  Serial.println("Driver Place Your FingerPrint");
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID;
}
uint8_t authenticate() {
bool sensorStatus = checkSensor();
if(sensorStatus){
  Serial.print("sensor working");
  uint8_t driverStat =getFingerprintIDez();
  Serial.print(driverStat);
  while(driverStat==255){
    driverStat =getFingerprintIDez();
    delay(50);
  }
  if(driverStat==101){
    Serial.print("Driver Authenticated, We are about to enroll the passenger");
    delay(5000);
    //driver correctly identified, authenticate the passenger
    Serial.println("done enrolling passenger, returning to main loop");
    return enrollPassenger();
    
  }else{
    //driver not identified , try again
    
  }
}else{
  Serial.print("sensor not working");
}

}
