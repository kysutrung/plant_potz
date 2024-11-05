int sensorPin = A0;  // Chân analog A0 của ESP8266
int sensorValue = 0; // Biến lưu trữ giá trị đọc từ cảm biến

void setup() {
  Serial.begin(115200); // Khởi động Serial với tốc độ 115200
}

void loop() {
  sensorValue = analogRead(sensorPin); // Đọc giá trị từ chân analog A0
  Serial.print("Gia tri analog doc duoc: ");
  Serial.println(sensorValue);

  delay(1000); // Đọc dữ liệu mỗi giây
}
