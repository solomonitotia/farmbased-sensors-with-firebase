#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>                                                    // esp8266 library
#include <FirebaseArduino.h>                                                // firebase library

#define FIREBASE_HOST "destin-demo-default-rtdb.firebaseio.com"                          // the project name address from firebase id
#define FIREBASE_AUTH "zK9YtUX7xvOwEatU2HnMWbIc2NnHXA49tFd5ld5Y"            // the secret key generated from firebase
#define WIFI_SSID "leonard"                                             // input your home or public wifi name 
#define WIFI_PASSWORD "Mabele2020*"                                    //password of wifi ssid
// Data wire is connected to digital pin 7 on the Arduino
#define ONE_WIRE_BUS 2
// Setup a oneWire instance to communicate with any OneWire device
OneWire oneWire(ONE_WIRE_BUS);

// Pass oneWire reference to DallasTemperature library
DallasTemperature sensors(&oneWire);
int sense_Pin = A0; // Soil Sensor input at Analog PIN A0
int sensorValue = 0;
// Variables to store temperature values
int percent = 0;
void setup()
{
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                     //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                                            //print local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                              // connect to firebase
  delay(2000);

}

void loop()
{
  int percentValue = 0;
  // Send the command to get temperatures
  sensors.requestTemperatures();
  sensorValue = analogRead(sense_Pin);
  percentValue = map(sensorValue, 1023, 465, 0, 100);



  float TempC = sensors.getTempCByIndex(0);
  float TempF = sensors.getTempCByIndex(0) * (9.0) / 5.0 + 32.0;
  float SoilV = percentValue;
  // value = value / 10;
  String fireTempC =String (TempC) + String("°C");
  String fireTempF =String (TempF) + String("F");
  String firesoilV =String (SoilV) + String("%");
  Serial.print("Temperature: ");
  Serial.print(TempC);
  Serial.print(" °C" ); // shows degrees character
  // print the temperature in Fahrenheit
  Serial.print(TempF);
  Serial.println(" F ");
  Serial.print("Soil Moisture Level is: ");
  Serial.println(SoilV);
  delay(5000);
  Firebase.pushString("/Crop_recommender/ TempC", fireTempC);
  Firebase.pushString("/Crop_recommender/ TempF", fireTempF);
  Firebase.pushString("/Crop_recommender/ SoilMoistureLevel", firesoilV);
      if (Firebase.failed()) 
    {
 
      Serial.print("pushing /logs failed:");
      Serial.println(Firebase.error()); 
      return;
  }
}

