#include "DHT.h"
#define DHTPIN 7
#define DHTTYPE DHT11  
DHT dht(DHTPIN, DHTTYPE);
bool bol;
#define flamePin 10 // Flame Sensor Connected To Arduino
#define buzzerPin 8 //Buzzer Connected To Arduino
#define firePin 9
#include <LiquidCrystal.h>
int Contrast=150;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);   

void setup() {
Serial.begin(115200);
dht.begin();
pinMode(buzzerPin,OUTPUT);
pinMode(flamePin,INPUT);
pinMode(firePin,OUTPUT);
analogWrite(6,Contrast);
lcd.begin(16, 2);
delay(300);


}

void loop() {

  int Flame = digitalRead(flamePin);
  float t = dht.readTemperature();
  Serial.println(t);
  bol = 0;
  
  if (Flame == LOW && t>25 )
  {  
     tone(buzzerPin,1000);
     bol = 1;
     Serial.write(bol);
     lcd.setCursor(0, 0);
     lcd.print("Alert");
    
     lcd.setCursor(0, 1);
      lcd.print("Fire");
      digitalWrite(firePin, HIGH);
     delay(2000);
     
     noTone(buzzerPin);
     
   }
  else if (Flame == HIGH )
  {  
     Serial.println(t);
     digitalWrite(firePin, LOW);
  }
  delay(300);

}
