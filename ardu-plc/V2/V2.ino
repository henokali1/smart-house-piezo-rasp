#include <Wire.h>
#include "SparkFun_SCD30_Arduino_Library.h" 
#include "Wire.h"
#include "Adafruit_LiquidCrystal.h"
#include <Adafruit_NeoPixel.h>
#include "WS2812_Definitions.h"
#include <Adafruit_Fingerprint.h>
#include "EmonLib.h"             // Include Emon Library
EnergyMonitor emon1;

#define LIGHT_INTENSITY_SENSOR A0
#define LIGHT_INTENSITY_THERSHOLD 300
#define PIN 5
#define LED_COUNT 24
#define GLASS_BREAK_SENSOR A1

#define stp 8
#define dir 9
#define MS1 10
#define MS2 11
#define EN  12

int x;
int y;
int state;

const int CURRENT_SENSOR = A3;
const int avgSamples = 10;
int sensorValue = 0;
float sensitivity = 100.0 / 500.0; //100mA per 500mV = 0.2
float Vref = 2500; // Output voltage with no current: ~ 2500mV or 2.5V

unsigned long color = 10000000;
byte red = (color & 0xFF0000) >> 16;
byte green = (color & 0x00FF00) >> 8;
byte blue = (color & 0x0000FF);
const int pirPin = 6;
const int liquidLevel = 7;


String coTwo = "";
String temp = "";
String humidity = "";

unsigned long pMil = 0;
unsigned long pMilCli = 0;
unsigned long pMilGls = 0;
unsigned long pMilInLig = 0;
const long pwrInt = 5000;  
const int cliInt = 5000;
const int indoorLightInterval = 10000;
boolean curtainClosed = false;

// Connect via i2c, default address #0 (A0-A2 not jumpered)
Adafruit_LiquidCrystal lcd(0);
SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

SCD30 airSensor;

Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

String toSend = "";
boolean glsPriv = false;
boolean liqPriv = false;
boolean pirPriv = false;
boolean lastState = false;

void setup()
{
  leds.begin();
  switchOffLights();
  Serial.begin(115200);
  finger.begin(57600);
  stepperSetup();
  Wire.begin();
  lcd.clear();
  lcd.begin(16, 4);
  lcd.setBacklight(HIGH);
  airSensor.begin(); //This will cause readings to occur every two seconds
  pinMode(pirPin, INPUT);
  pinMode(liquidLevel, INPUT);
  emon1.voltage(2, 234.26, 1.7);  // Voltage: input pin, calibration, phase_shift
  emon1.current(3, 111.1);    
}

void sendToRasp(String priv, String cur){
  int a=0;
}

void loop(){
  //Serial.println(measurePower());
  //delay(500);
  getFingerprintIDez();
  checkLightIntensity();
  unsigned long cMilCli = millis();
  if(cMilCli - pMilCli >= cliInt){
    pMilCli = cMilCli;
    if (airSensor.dataAvailable())
    {    
      lcd.setCursor(0, 0);
      //Serial.print("co2(ppm):");
      coTwo = String(airSensor.getCO2());
      lcd.print("co2(ppm):" + coTwo);
      //Serial.print(coTwo);
      lcd.setCursor(0, 1);
      //Serial.print(" temp(C):");
      temp = String(airSensor.getTemperature());
      //Serial.print(temp);
      lcd.print("Temp(C):" + temp);
      lcd.setCursor(0, 2);
      humidity = String(airSensor.getHumidity());
      //Serial.print(" humidity(%):");
      //Serial.print(humidity);
      lcd.print("Humidity(%):" + humidity);
      //Serial.println();
  
      lcd.setCursor(0, 3);
      lcd.print("LQ.L:" + String(digitalRead(liquidLevel)) + "        PIR:" + String(digitalRead(pirPin)));
  
      toSend = temp + ',' + humidity + ',' + coTwo + ",cli";
      Serial.println(toSend);
    }
  }

  boolean glsCrt = analogRead(GLASS_BREAK_SENSOR) > 800;
  if(glsCrt != glsPriv){
    glsPriv = glsCrt;
    if(glsCrt){
      Serial.println("True,gls");
    } else{
      delay(5000);
      Serial.println("False,gls");
    }
  } 

  boolean liqCrt = digitalRead(liquidLevel) == 0;
  if(liqCrt != liqPriv){
    liqPriv = liqCrt;
    if(liqCrt){
      Serial.println("True,lek");
    } else{
      Serial.println("False,lek");
    }
  }

  boolean pirCrt = digitalRead(pirPin);
  if(pirCrt != pirPriv){
    pirPriv = pirCrt;
    if(pirCrt){
      Serial.println("True,mot");
    } else{
      Serial.println("False,mot");
    }
  }
  
  unsigned long cMil = millis();
  if(cMil - pMil >= pwrInt){
    pMil = cMil;
    Serial.println(String(measurePower())+",pwr");
  } 

  unsigned long cMilInLgt = millis();
  if(cMilInLgt - pMilInLig >= indoorLightInterval){
    pMilInLig = cMil;
    if(lastState){
      switchOnIndoorLights();
      lastState = false;
    } else{
      switchOffIndoorLights();
      lastState = true;
    }
  } 
}
