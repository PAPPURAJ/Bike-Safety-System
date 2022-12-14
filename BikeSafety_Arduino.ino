
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <TinyGPS++.h>
#include "FirebaseESP8266.h"

Adafruit_MPU6050 mpu;
TinyGPSPlus gps;
FirebaseData firebaseData;
FirebaseJson json;
 

#define FIREBASE_HOST "bikeaccidentdetectionsystem-default-rtdb.firebaseio.com"  
#define FIREBASE_AUTH "AVeFlnxwgxHGyLRPD3ujAhhDYVQE48GaM41LejP4"

#define WIFI_SSID "BikeSafety"     
#define WIFI_PASSWORD "12345678" 



double Lat=10,Lon=11,x,y,tem;




void writeDB(String field,String value){
 Firebase.setString(firebaseData, "/Data/"+field,value );
  
}





void setup(void) {

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
   String d=String(xLimit<x || -xLimit>x || yLimit<=y  || -yLimit>y|| tLimit<=tem);
   Serial.println(d);
   writeDB("/Danger",d); 


  delay(250);


}
