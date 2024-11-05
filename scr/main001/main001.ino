/*==========================================================
Project name: Smart Plant Pot
Keyword: IOT
Platform: Blynk
Hardware: ESP8266
Project implementer: Nguyen Lam Thanh Tung, Luong Bao Tuan
Project developer: Nguyen Duc Trung
===========================================================*/

//Thư viện
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6I10mX5d5"
#define BLYNK_TEMPLATE_NAME "plant potz"
#define BLYNK_AUTH_TOKEN "zVGuxMgyvz3b5lNrbdO-xHDIjyIqBR9b"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Đặt mật khẩu và tên wifi của mình và đây
char ssid[] = "Project 01";
char pass[] = "vnu.edu.vn";

//Khai báo chân cắm
/*
1.28 Round TFT SCREEN
CS -> D8 (GPIO15)
DC -> D3 (GPIO0)
RES -> D4 (GPIO2)
SDA (MOSI) -> D7 (GPIO13)
SCL (SCK) -> D5 (GPIO14)
*/
#include <TFT_eSPI.h>
#include <SPI.h>
TFT_eSPI tft = TFT_eSPI();

#include <DHT.h>
#define DHTPIN 4         // DHT21 Data PIN -> GPIO4 (D2)
#define DHTTYPE DHT21
DHT dht(DHTPIN, DHTTYPE);

#define SOIL_SENSOR_PIN 0  // Soil moisture sensor DO PIN -> GPIO5 (D1)

#define PUMP 16 // 3V water pump VCC PIN -> GPIO16 (D0)

//Biến toàn cục
bool IS_WATER_PUMP_RUN = 0;
bool IS_AUTO_MODE_ON = 1;
float airHumidity = 0;
float airTemperature = 0;
float soilMoisture = 0; //650 -> 1024
float num_a = 3.74; 

void setup() {
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  
  dht.begin();
  
  pinMode(SOIL_SENSOR_PIN, INPUT);

  pinMode(PUMP, OUTPUT);
  digitalWrite(PUMP, LOW);
  
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);  // Chữ màu trắng, nền đen
  tft.setTextSize(3);  // Đặt kích thước chữ (có thể chỉnh để phóng to/thu nhỏ)
  // tft.pushImage(x, y, 240, 240, ah008); 
}

void loop() {
  Blynk.run();

  tft.fillScreen(TFT_BLACK);

  soilMoisture = 100 - ((analogRead(SOIL_SENSOR_PIN) - 650)/num_a);
  airTemperature = dht.readTemperature();
  airHumidity = dht.readHumidity();

  tft.setCursor(60, 60);  
  tft.print(airHumidity);
  tft.println(" %");
  tft.setCursor(25, 100);  
  tft.print(airTemperature);
  tft.println(" C");
  tft.setCursor(25, 140);
  tft.print(soilMoisture);
  tft.println(" %");


  if(IS_AUTO_MODE_ON){
    if(soilMoisture < 30){
      digitalWrite(PUMP, HIGH);
      IS_WATER_PUMP_RUN = 1;
    }
    else if(soilMoisture < 40 && airTemperature > 27){
      digitalWrite(PUMP, HIGH);
      IS_WATER_PUMP_RUN = 1;
    }
    else if(soilMoisture < 50 && airTemperature > 30 && airHumidity < 40){
      digitalWrite(PUMP, HIGH);
      IS_WATER_PUMP_RUN = 1;
    }
    else{
      digitalWrite(PUMP, LOW);
      IS_WATER_PUMP_RUN = 0;
    }
  }
  else{
    if(soilMoisture < 30){
    Blynk.logEvent("your_plant_need_water", "Ur plant is thirsty!!!!");
    }
    else if(soilMoisture < 40 && airTemperature > 27){
    Blynk.logEvent("your_plant_need_water", "Ur plant is thirsty!!!!");
    }
    else if(soilMoisture < 50 && airTemperature > 30 && airHumidity < 40){
    Blynk.logEvent("your_plant_need_water", "Ur plant is thirsty!!!!");
    }
  }


  Blynk.virtualWrite(V2, airHumidity);
  Blynk.virtualWrite(V1, airTemperature);
  Blynk.virtualWrite(V3, soilMoisture);
  Blynk.virtualWrite(V0, IS_WATER_PUMP_RUN);

  delay(10000);
}

//Hàm hoạt động với chân ảo Blynk

BLYNK_WRITE(V4)  // Hàm này được gọi khi Virtual Pin V4 thay đổi giá trị
{
    int value_1 = param.asInt();  // Lấy giá trị của V4
    if(value_1 == 0){
      IS_AUTO_MODE_ON = 0;
    }
    else if(value_1 == 1){
      IS_AUTO_MODE_ON = 1;
      Blynk.virtualWrite(V5, 0);
    }
}

BLYNK_WRITE(V5)  // Hàm này được gọi khi Virtual Pin V4 thay đổi giá trị
{
    int value_2 = param.asInt();  // Lấy giá trị của V4
    if(value_2 == 1){
      Blynk.virtualWrite(V4, 0);
      digitalWrite(PUMP, HIGH);
      IS_WATER_PUMP_RUN = 1;
    }
    else{
      digitalWrite(PUMP, LOW);
      IS_WATER_PUMP_RUN = 1;
    }
}