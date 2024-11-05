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
#define BLYNK_TEMPLATE_NAME "plant potz"
#define BLYNK_AUTH_TOKEN "zVGuxMgyvz3b5lNrbdO-xHDIjyIqBR9b"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Đặt mật khẩu và tên wifi của mình và đây
char ssid[] = "Project 01";
char pass[] = "vnu.edu.vn";

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

#define SOIL_SENSOR_PIN 5  // Soil moisture sensor DO PIN -> GPIO5 (D1)

#define PUMP 16 // 3V water pump VCC PIN -> GPIO16 (D0)


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
    int soilMoistureStatus = digitalRead(SOIL_SENSOR_PIN);  // Đọc trạng thái từ chân DO
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    tft.setCursor(60, 60);  
    tft.print(humidity);
    tft.println(" %");
    tft.setCursor(25, 100);  
    tft.print(temperature);
    tft.println(" C");

    if (soilMoistureStatus == HIGH) {
      tft.setCursor(25, 140);  
      tft.print("Kho VCL");
      Blynk.virtualWrite(V0, 1);
    } else {
      tft.setCursor(25, 140);  
      tft.print("OK roi");
      Blynk.virtualWrite(V0, 0);
    }

    Blynk.virtualWrite(V2, humidity);
    Blynk.virtualWrite(V1, temperature);
    Blynk.virtualWrite(V3, soilMoistureStatus);

    delay(10000);
}

BLYNK_WRITE(V0)  // Hàm này được gọi khi Virtual Pin V0 thay đổi giá trị
{
    int value = param.asInt();  // Lấy giá trị của V0
    if(value == 0){
      digitalWrite(PUMP, LOW);
    }
    else if(value == 1){
      digitalWrite(PUMP, HIGH);
    }
}
