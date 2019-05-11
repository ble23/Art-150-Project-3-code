#include <Wire.h>
#include "Adafruit_MCP9808.h"
#include <LiquidCrystal_I2C.h>
#include <Time.h>
#include <TimeLib.h>

//----------------------------------------------------------------------------------------------------------------------------------------------------
Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();
LiquidCrystal_I2C lcd(0x27,20,4); 
int fsrAnalogPin = 0;
int fsrReading;
int state = 1;
const int trigPin = 10;
const int echoPin = 11;
long duration, inches, cm;
//----------------------------------------------------------------------------------------------------------------------------------------------------
void setup() {
  lcd.init(); 
  lcd.backlight();
  Serial.begin(9600);
  while (!Serial); 
  Serial.println("MCP9808 demo");
  if (!tempsensor.begin(0x18)) {
    Serial.println("Couldn't find MCP9808! Check your connections and verify the address is correct.");
    while (1);
  }
   Serial.println("Found MCP9808!");
  tempsensor.setResolution(3);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void loop() {

  tempsensor.wake();  
  float c = tempsensor.readTempC();
  float f = tempsensor.readTempF();
  tempsensor.shutdown_wake(1);
      
  lcd.clear();
  
  switch(state){
      
    case 0:  
      lcd.setCursor(0,0);
      lcd.print(c);
      lcd.setCursor(6,0);
      lcd.print("Celcius");
      lcd.setCursor(0,1);
      lcd.print(f);
      lcd.setCursor(6,1);
      lcd.print("Farenheit");
      break;
       
    case 1:
    
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);

      duration = pulseIn(echoPin, HIGH);

      inches = microsecondsToInches(duration);
      cm = microsecondsToCentimeters(duration);

      lcd.setCursor(0,0);
      lcd.print(cm);
      lcd.print(" cm");
      lcd.setCursor(0,1);
      lcd.print(inches);
      lcd.print(" inches");
      break;
  }
  delay(1000);
}


long microsecondsToInches(long microseconds)
{
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds)
{
  return microseconds / 29 / 2;
}
