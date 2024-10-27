// Thực hiện dự án: Nguyen Lam Thanh Tung, Luong Bao Tuan
// Tên dự án: Smart Plant Pot

#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6I10mX5d5"
#define BLYNK_TEMPLATE_NAME "plant potz"
#define BLYNK_AUTH_TOKEN "zVGuxMgyvz3b5lNrbdO-xHDIjyIqBR9b"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include <TFT_eSPI.h>

#include <SPI.h>
#include <DHT.h>
#define DHTPIN 4         // Chân DATA của DHT21 nối với GPIO4 (D2 trên NodeMCU)
#define DHTTYPE DHT21    // Loại cảm biến là DHT21 (AM2301)
#define SOIL_SENSOR_PIN 5  // Chân DO nối với D1 (GPIO5) của ESP8266

#define PUMP 16 // Chân máy bơm nối với D0 tức GPIO16

DHT dht(DHTPIN, DHTTYPE);

TFT_eSPI tft = TFT_eSPI();
/*
Ghi chú đấu nối TFT SCREEN

CS -> D8 (GPIO15) trên ESP8266.
DC -> D3 (GPIO0) trên ESP8266.
RES -> D4 (GPIO2) trên ESP8266.
SDA (MOSI) -> D7 (GPIO13) trên ESP8266.
SCL (SCK) -> D5 (GPIO14) trên ESP8266.
*/

// Đặt mật khẩu và tên wifi của mình và đây
char ssid[] = "Le Xuan Hai Idol";
char pass[] = "hhhhhhhh";


void setup() {
    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
    
    dht.begin();
    
    pinMode(SOIL_SENSOR_PIN, INPUT);

    pinMode(PUMP, OUTPUT);
    digitalWrite(PUMP, LOW);

    // tft.pushImage(x, y, 240, 240, ah008);     
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);  // Chữ màu trắng, nền đen
    tft.setTextSize(3);  // Đặt kích thước chữ (có thể chỉnh để phóng to/thu nhỏ)
    
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
