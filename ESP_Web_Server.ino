/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

// Import required libraries

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <Wire.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "HX711.h"

int Weight;
int min_Weight;
int max_Weight;
int pre_Weight;
int pre_day_weight = 0;
int cousumpt_weight = 0;
int Sec_Count = 0;
int Status = 0, Status_Pre = 1;
int Flag_Up = 0, Flag_Down = 0;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// Variables to save date and time
String formattedDate;
String dayStamp;
String Nowday;
String timeStamp;

// Replace with your network credentials
const char* ssid = "Jason_2.4G";
const char* password = "0933660921";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

int readWeight() {
  // Read temperature as Celsius (the default)
  int t = Get_Weight();  //計算放在感測器上的重物重量
  // Convert temperature to Fahrenheit

  if (isnan(t)) {    
    Serial.println("Failed to read from BME280 sensor!");
    return 0;
  }
  else {
    return t;
  }
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  pinMode(4, OUTPUT);  
    // Initialize SPIFFS
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  bool status; 
  Init_Hx711();
  Serial.print("Welcome to use!\n");
  Get_Maopi();    //開始校正
  delay(3000);
  Get_Maopi();    //校正結束
  Serial.print("Start!\n");
  max_Weight = Weight;
  min_Weight = Weight;
  




  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());
  
    timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  timeClient.setTimeOffset(28800);
  getTimeStamp();
  Nowday = dayStamp;
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html");
  });
  server.on("/weight", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(Weight).c_str());
  });
  server.on("/cousumpt", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(cousumpt_weight).c_str());
  });
  server.on("/pre_cousumpt", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(pre_day_weight).c_str());
  });

  // Start server
  server.begin();
}
 
void loop(){
  Weight = readWeight();
  getTimeStamp();
  if (dayStamp != Nowday){
    Serial.print("Initialize consumption.....");
    max_Weight = Weight;
    min_Weight = Weight;
    pre_day_weight = cousumpt_weight;
    cousumpt_weight = 0;
    delay(3000);
    }
  Serial.print("Weight: ");
  Serial.println(Weight);
  Serial.print("max Weight: ");
  Serial.println(max_Weight);
  Serial.print("cousumpt_weight: ");
  Serial.println(cousumpt_weight);
  Serial.print("pre day cousumpt_weight: ");
  Serial.println(pre_day_weight);
  
  if (max_Weight<Weight){
    max_Weight = Weight;
    }
  
  if (Weight < 150){
    min_Weight = pre_Weight;
    cousumpt_weight = cousumpt_weight+max_Weight-min_Weight;
    Serial.print("cousumpt_weight: ");
    Serial.println(cousumpt_weight);
    while(Weight<250){
      Serial.print("fill water: ");
      Serial.println(Weight);
      Weight = readWeight();
      digitalWrite(4,HIGH);
      delay(100);
      digitalWrite(4,LOW);
      delay(3000);
      
      }
    max_Weight = readWeight();
    }
  pre_Weight = Weight;
  delay(3000);
}

// Function to get date and time from NTPClient
void getTimeStamp() {
  while(!timeClient.update()) {
    timeClient.forceUpdate();
  }
  // The formattedDate comes with the following format:
  // 2018-05-28T16:00:13Z
  // We need to extract date and time
  formattedDate = timeClient.getFormattedDate();
  Serial.println(formattedDate);

  // Extract date
  int splitT = formattedDate.indexOf("T");
  dayStamp = formattedDate.substring(0, splitT);
  Serial.println(dayStamp);
  // Extract time
  timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
  Serial.println(timeStamp);
}
