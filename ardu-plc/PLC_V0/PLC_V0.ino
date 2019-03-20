#include <PLDuino.h>
using namespace PLDuino;

void setup()
{
  // Initializing Serial.
  Serial.begin(115200);
  
  // A necessary call.
  // It sets pin modes according to their function.
  PLDuino::init();
}

void checkSoilMoistureSensor(){
  int val = analogRead(A0);
  //Serial.println(val);
  if(val > 50){
    digitalWrite(PLDuino::RELAY1, HIGH);
  } else {
    digitalWrite(PLDuino::RELAY1, LOW);
  }
  delay(500);
}

void checkFingerPrintLock(){
  int val = analogRead(A1);
  if(val > 100){
    digitalWrite(PLDuino::RELAY2, HIGH);
  }
  else{
    digitalWrite(PLDuino::RELAY2, LOW);
  }
  
  //Serial.println("Finger Print Scanner: " + String(val));
  
}
void loop()
{
  checkSoilMoistureSensor();
  checkFingerPrintLock();
}
