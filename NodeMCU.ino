#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
BlynkTimer timer;
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

static const int RXPin = 4, TXPin = 5;   // GPIO 4=D2(conneect Tx of GPS) and GPIO 5=D1(Connect Rx of GPS
static const uint32_t GPSBaud = 9600; //if Baud rate 9600 didn't work in your case then use 4800

TinyGPSPlus gps; // The TinyGPS++ object
WidgetMap myMap(V0);  // V0 for virtual pin of Map Widget

SoftwareSerial ss(RXPin, TXPin);  // The serial connection to the GPS device

unsigned int move_index = 1; 
char auth[] = "gET-1CGUa7aBrU1-uyc-A46AWLFbxVaP"; //Auth code sent via Email from blynk
char ssid[] = "Wifi Name";// Name of your network (HotSpot or Router name)
char pass[] = "Enter your wifi password"; 
int flag=0;
int data;
#define firepin D6
void notifyOnFire()
{
 int button = digitalRead(D6);
 if(button==1 && flag==0)
 {  
    Serial.println("Fire in the House");
    Blynk.notify("Alert : Fire in the House");
    flag=1;
    while (ss.available() > 0) 
    {
      
      if (gps.encode(ss.read()))
        displayInfo();
  }
 }
 else if(button==0){
  flag=0;
 }
 }

void setup()
{ 
Serial.begin(115200);
Serial.println();
ss.begin(GPSBaud);
pinMode(firepin,INPUT_PULLUP);
Blynk.begin(auth, ssid, pass);
timer.setInterval(1000L,notifyOnFire);
timer.setInterval(5000L, checkGPS);
}

void checkGPS(){
  if (gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
      Blynk.virtualWrite(V4, "GPS ERROR");  // Value Display widget  on V4 if GPS not detected
  }
  else {
    Serial.println("GPS detected");
  }
}
void loop()
{
  
  Blynk.run();
  timer.run();
}
void displayInfo()
{ 

  if (gps.location.isValid() ) 
  {
    
    float latitude = (gps.location.lat());     //Storing the Lat. and Lon. 
    float longitude = (gps.location.lng()); 
    
    Serial.print("LAT:  ");
    Serial.println(latitude, 6);  // float to x decimal places
    Serial.print("LONG: ");
    Serial.println(longitude, 6);
    Blynk.virtualWrite(V1, String(latitude, 6));   
    Blynk.virtualWrite(V2, String(longitude, 6));  
    myMap.location(move_index, latitude, longitude, "GPS_Location");
                  
  }
  

  Serial.println();
}
