
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <TinyGPS++.h>
#include "FirebaseESP8266.h"

Adafruit_MPU6050 mpu;
TinyGPSPlus gps;
FirebaseData firebaseData;
FirebaseJson json;

const int trigpin= D6;  
const int echopin= D7;  

long duration;  
int distance,disLim=40;  
 

#define FIREBASE_HOST "vachiclesafety-default-rtdb.firebaseio.com"  
#define FIREBASE_AUTH "UNE2g5wOAmysdIXQWXXXH1nlGeSCSPq8djVgg7af"

#define WIFI_SSID "BikeSafety_IOT"     
#define WIFI_PASSWORD "12345678" 



double Lat=10,Lon=11,x,y,tem;




void writeDB(String field,String value){
 Firebase.setString(firebaseData, "/Data/"+field,value );
  
}





void setup(void) {

 pinMode(trigpin,OUTPUT);  
  pinMode(echopin,INPUT); 
  
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  
  pinMode(D4,OUTPUT);
  while (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(D4,0);
    Serial.print(".");
    delay(200);
    digitalWrite(D4,1);
    Serial.print(".");
    delay(200);
    
  }
  
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  
  while (!Serial)
    delay(10); 

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");


  delay(100);
}

double xLimit=5.0,yLimit=5.0, tLimit=50.0;

void loop() {



  digitalWrite(trigpin,HIGH);  
  delayMicroseconds(10);  
  digitalWrite(trigpin,LOW);  
  duration=pulseIn(echopin,HIGH);  
  distance = duration*0.034/2;  
  Serial.print(distance);  

  

     while(Serial.available())             
  {
    gps.encode(Serial.read());       
   
  }
  if(gps.location.isUpdated())  
  {
   
    Lat=gps.location.lat();
    Lon=gps.location.lng();
    Serial.print("GPS: "+String(gps.location.lat(),6));
    Serial.print("  ");
    Serial.print(String(gps.location.lng(),6)+"   ");

    writeDB("/Location/Lat",String(Lat,6));
    writeDB("/Location/Lon",String(Lon,6));

  }

   delay(250);

  
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  x=a.acceleration.y;
  y=a.acceleration.z;
  tem=temp.temperature;

  Serial.print("Acceleration: ");
  Serial.print(x);
  Serial.print("  ");
  Serial.print(y);


  Serial.print("  Temp: ");
  Serial.println(tem);

   writeDB("/X",String(x));
   writeDB("/Y",String(y));
   writeDB("/Temp",String(tem));
   writeDB("/Danger",String(xLimit<=x || yLimit<=y || tLimit<=tem || distance<disLim)); 


  delay(250);


}
