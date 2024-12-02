/*==========================================================
Project name: Smart Plant Pot
Keyword: IOT
Platform: Blynk
Hardware: ESP8266
Project implementer: Nguyen Lam Thanh Tung, Luong Bao Tuan
Project developer: Nguyen Duc Trung
===========================================================*/

#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6I10mX5d5"
#define BLYNK_TEMPLATE_NAME "Plant Pot"
#define BLYNK_AUTH_TOKEN "zVGuxMgyvz3b5lNrbdO-xHDIjyIqBR9b"
// #define BLYNK_TEMPLATE_ID "TMPL6p4p2SHdQ"
// #define BLYNK_TEMPLATE_NAME "Chau Cay IOT"
// #define BLYNK_AUTH_TOKEN "dnb7zMOT856kDCAtoWHz71vOYjAVRhAn"


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>


char ssid[] = "Project 01";
char pass[] = "vnu.edu.vn";


// 1.28 Round TFT SCREEN
// CS -> D8 (GPIO15)
// DC -> D3 (GPIO0)
// RES -> D4 (GPIO2)
// SDA (MOSI) -> D7 (GPIO13)
// SCL (SCK) -> D5 (GPIO14)

#include <TFT_eSPI.h>
#include <SPI.h>
TFT_eSPI tft = TFT_eSPI();

#include <DHT.h>
#define DHTPIN 4   
#define DHTTYPE DHT21
DHT dht(DHTPIN, DHTTYPE);

#define SOIL_SENSOR_PIN 0

#define PUMP 5


bool IS_WATER_PUMP_RUN = 0;
bool IS_AUTO_MODE_ON = 1;
float airHumidity = 0;
float airTemperature = 0;
float soilMoisture = 0; //650 -> 1024
float num_a = 3.74;
bool feelSad = 0;
int value_2 = 0;
int value_1 = 0;

void smilez(){
  tft.fillScreen(11774);
  tft.drawArc(80, 100, 30, 25, 90, 270, TFT_BLACK, TFT_BLACK);
  tft.drawArc(160, 100, 30, 25, 90, 270, TFT_BLACK, TFT_BLACK);
  tft.drawArc(120, 120, 50, 45, 290, 70, TFT_BLACK, TFT_BLACK);
}

void sadz(){
  tft.fillScreen(63873);
  tft.fillCircle(80, 70, 5, TFT_BLACK);
  tft.fillCircle(160, 70, 5, TFT_BLACK);
  tft.drawArc(120, 170, 50, 45, 110, 250, TFT_BLACK, TFT_BLACK);
}

void setup() {
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  
  dht.begin();
  Serial.begin(9600);
  
  pinMode(SOIL_SENSOR_PIN, INPUT);

  pinMode(PUMP, OUTPUT);
  digitalWrite(PUMP, LOW);
  
  tft.init();
  tft.setRotation(2);
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.setTextSize(3);

}

void loop() {
  Blynk.run();

  soilMoisture = 100 - ((analogRead(SOIL_SENSOR_PIN) - 650)/num_a);
  airTemperature = dht.readTemperature();
  airHumidity = dht.readHumidity();

  if (WiFi.status() != WL_CONNECTED){
    tft.fillScreen(TFT_WHITE);
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    tft.setCursor(10, 100);
    tft.println("Disconnected");
  }
  else{
    if(feelSad){
      sadz();
    }
    else{
      smilez();
    }
  }

  if(value_1 == 0){
    IS_AUTO_MODE_ON = 0;
  }
  else if(value_1 == 1){
    IS_AUTO_MODE_ON = 1;
  }

  if(IS_AUTO_MODE_ON){
    if(soilMoisture < 30){
      feelSad = 1;
      digitalWrite(PUMP, HIGH);
      IS_WATER_PUMP_RUN = 1;
    }
    else if(soilMoisture < 40 && airTemperature > 27){
      feelSad = 1;
      digitalWrite(PUMP, HIGH);
      IS_WATER_PUMP_RUN = 1;
    }
    else if(soilMoisture < 50 && airTemperature > 30 && airHumidity < 40){
      feelSad = 1;
      digitalWrite(PUMP, HIGH);
      IS_WATER_PUMP_RUN = 1;
    }
    else{
      feelSad = 0;
      digitalWrite(PUMP, LOW);
      IS_WATER_PUMP_RUN = 0;
    }
  }
  else{
    if(soilMoisture < 30){
      Blynk.logEvent("your_plant_need_water", "Ur plant is thirsty!!!!");
      feelSad = 1;
    }
    else if(soilMoisture < 40 && airTemperature > 27){
      Blynk.logEvent("your_plant_need_water", "Ur plant is thirsty!!!!");
      feelSad = 1;
    }
    else if(soilMoisture < 50 && airTemperature > 30 && airHumidity < 40){
      Blynk.logEvent("your_plant_need_water", "Ur plant is thirsty!!!!");
      feelSad = 1;
    }
    else{
      feelSad = 0;
    }

    if(value_2 == 1){
      digitalWrite(PUMP, HIGH);
      IS_WATER_PUMP_RUN = 1;
    }
    else if(value_2 == 0){
      digitalWrite(PUMP, LOW);
      IS_WATER_PUMP_RUN = 0;
    }    
  }



  Blynk.virtualWrite(V2, airHumidity);
  Blynk.virtualWrite(V1, airTemperature);
  Blynk.virtualWrite(V3, soilMoisture);
  Blynk.virtualWrite(V0, IS_WATER_PUMP_RUN);

  delay(5000);
}

BLYNK_WRITE(V4)
{
    value_1 = param.asInt();
}

BLYNK_WRITE(V5)
{
    value_2 = param.asInt();
}