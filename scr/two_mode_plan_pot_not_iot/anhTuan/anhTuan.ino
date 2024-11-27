//LIB
#include <TM1637Display.h>

//PINOUT
const int but01 = 8;
const int but02 = 9;
const int but03 = 10;
const int yellowLed = 11;
const int redLed = 12;
const int relayTrigger = 13;
const int soilSensor = A3;
#define CLK 6  // Chân CLK của TM1637 nối với D3
#define DIO 7  // Chân DIO của TM1637 nối với D2

//GLOBAL VAR
int SOIL_MOISTURE = 0;
int IS_AUTO_MODE_ON = 1;
int TIME_COUNTDOWN = 3;
int SETTED_TIME = 0;
long int START_TIME = 0;
bool PICKED_TIME = 0;
int WATER_TIME = 10;
bool BUT01_STILL_PRESSED = 0;
bool BUT02_STILL_PRESSED = 0;
bool BUT03_STILL_PRESSED = 0;
const uint8_t SEGMENT_A = 0b01110111;

//DECLARE
TM1637Display display(CLK, DIO);

void getSoilMoisture(){
  SOIL_MOISTURE = analogRead(soilSensor);
}

void setButton(){
  int statusBut01 = digitalRead(but01);
  int statusBut02 = digitalRead(but02);
  int statusBut03 = digitalRead(but03);

  if(statusBut01 == 1 && BUT01_STILL_PRESSED == 0){
    IS_AUTO_MODE_ON++;
    if(IS_AUTO_MODE_ON > 2){
      IS_AUTO_MODE_ON = 0;
    }
    BUT01_STILL_PRESSED = 1;
  }
  if(statusBut01 == 0){
    BUT01_STILL_PRESSED = 0;
  }

  if(statusBut02 == 1 && BUT02_STILL_PRESSED == 0 && IS_AUTO_MODE_ON == 2){
    TIME_COUNTDOWN++;
    SETTED_TIME = TIME_COUNTDOWN;
    BUT02_STILL_PRESSED = 1;
  }
  if(statusBut02 == 0){
    BUT02_STILL_PRESSED = 0;
  }

  if(statusBut03 == 1 && BUT03_STILL_PRESSED == 0 && IS_AUTO_MODE_ON == 2){
    TIME_COUNTDOWN--;
    SETTED_TIME = TIME_COUNTDOWN;
    if(TIME_COUNTDOWN < 0){
      TIME_COUNTDOWN = 0;
    }
    BUT03_STILL_PRESSED = 1;
  }
  if(statusBut03 == 0){
    BUT03_STILL_PRESSED = 0;
  }
}

void systemDisplay(){
  if(IS_AUTO_MODE_ON == 1){
    uint8_t segments[4] = {SEGMENT_A, 0, 0, 0};
    display.setSegments(segments);
  }
  if(IS_AUTO_MODE_ON == 0 || IS_AUTO_MODE_ON == 2){
    display.showNumberDec(TIME_COUNTDOWN);
  }
}

void autoPump(){
  getSoilMoisture();
  if(SOIL_MOISTURE > 500){
    digitalWrite(relayTrigger, LOW);
    digitalWrite(yellowLed, LOW);
    digitalWrite(redLed, HIGH);
  } 
  else{
    digitalWrite(relayTrigger, HIGH);
    digitalWrite(redLed, LOW);
    digitalWrite(yellowLed, HIGH);
  }
}

void timerPump(){
  if(PICKED_TIME == 0){
    START_TIME = millis();
    PICKED_TIME = 1;
  }
  if( millis() - START_TIME >= 1000){
      TIME_COUNTDOWN--;
      PICKED_TIME = 0;
      if(TIME_COUNTDOWN < - WATER_TIME){
        TIME_COUNTDOWN = SETTED_TIME;
      }
  }

}

void systemLogic(){
  if(IS_AUTO_MODE_ON == 0){
    timerPump();
  }
  if(IS_AUTO_MODE_ON == 1){
    autoPump();
  }
  else{
    digitalWrite(relayTrigger, HIGH);
  }

  if(TIME_COUNTDOWN < 0){
    digitalWrite(relayTrigger, LOW);
  }
}

void setup(){
  pinMode(but01, INPUT_PULLUP);
  pinMode(but02, INPUT_PULLUP);
  pinMode(but03, INPUT_PULLUP);
  pinMode(yellowLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(relayTrigger, OUTPUT);
  pinMode(soilSensor, INPUT);

  digitalWrite(yellowLed, HIGH);




  digitalWrite(relayTrigger, HIGH);

  display.setBrightness(1);
}

void loop(){
  setButton();
  getSoilMoisture();
  systemLogic();
  systemDisplay();
  //delay(100);
}