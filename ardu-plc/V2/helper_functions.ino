
uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  // found a match!
  //Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  //Serial.print(" with confidence of "); Serial.println(finger.confidence); 
  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  //Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  //Serial.print(" with confidence of "); Serial.println(finger.confidence);
  Serial.println(String(finger.fingerID) + ",fp");
  return finger.fingerID; 
}


void fingerPrintSetup(){
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

  finger.getTemplateCount();
  Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  Serial.println("Waiting for valid finger...");
}

void stepperSetup(){
  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(EN, OUTPUT);
  resetEDPins();
}

void resetEDPins()
{
  digitalWrite(stp, LOW);
  digitalWrite(dir, LOW);
  digitalWrite(MS1, LOW);
  digitalWrite(MS2, LOW);
  digitalWrite(EN, HIGH);
}

//Default microstep mode function
void openCurtain()
{
  digitalWrite(EN, LOW);
  digitalWrite(dir, LOW); //Pull direction pin low to move "forward"
  for(x= 1; x<1000; x++)  //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(stp,HIGH); //Trigger one step forward
    delay(1);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(1);
  }
  resetEDPins();
}

//Reverse default microstep mode function
void closeCurtain()
{
  digitalWrite(EN, LOW);
  digitalWrite(dir, HIGH); //Pull direction pin high to move in "reverse"
  for(x= 1; x<1000; x++)  //Loop the stepping enough times for motion to be visible
  {
    digitalWrite(stp,HIGH); //Trigger one step
    delay(1);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(1);
  }
  resetEDPins();
}


float measurePower(){
  emon1.calcVI(20,2000);         // Calculate all. No.of half wavelengths (crossings), time-out
  emon1.serialprint();           // Print out all variables (realpower, apparent power, Vrms, Irms, power factor)
  
  float realPower = emon1.realPower;  
  return(realPower);
}

void switchOffLights(){
  for (int i=0; i<=15; i++)
  {
    leds.setPixelColor(i, 0);
  }                                                                                                                                                                                                                                            
  leds.show();
}

void switchOnLights(){
  for(int i=0; i<=15; i++){
    leds.setPixelColor(i, WHITE);
  }
  leds.show();
}

void switchOnIndoorLights(){
  for(int i=16; i<=23; i++){
    leds.setPixelColor(i, WHITE);
  }
  leds.show();
}

void switchOffIndoorLights(){
  for (int i=15; i<=23; i++)
  {
    leds.setPixelColor(i, 0);
  }                                                                                                                                                                                                                                            
  leds.show();
}

boolean checkLightIntensity(){
  boolean val = analogRead(LIGHT_INTENSITY_SENSOR) < LIGHT_INTENSITY_THERSHOLD;
  if(val){
    switchOnLights();
    if(!curtainClosed){
      closeCurtain();
      curtainClosed = true;
    }
    return(1);
  } else if((!val) && curtainClosed){
    switchOffLights();
    openCurtain();
    curtainClosed = false;
    return(0);
  }
}

void yelloww(int led){
  green = (color & 0x00FF00) >> 7;
  leds.setPixelColor(led, red, green, 0x000000); 
  leds.show();
}
void redd(int led){
  leds.setPixelColor(led, red, 0x000000, 0x000000); 
  leds.show();
}

void greenn(int led){
  leds.setPixelColor(led, 0x000000, green, 0x000000); 
  leds.show();

  
}

void whitee(int led){
  leds.setPixelColor(led, red, green, blue); 
  leds.show();
}
void bluee(int led){
  leds.setPixelColor(led, 0x000000, 0x000000, blue); 
  leds.show();
}
