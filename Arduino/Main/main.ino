#include <AltSoftSerial.h>
#include <LiquidCrystal.h>
#include<stdio.h>
#include <string.h>
AltSoftSerial BTserial; 
// Function Declarations
void wateringSystem();
void recvDataFromLightBoard();
void splitLux();
void recvDataFromTHBoard();
void splitTemperatureAndHumidity();

const byte maxDataLength = 20;
//TH Board         
char receivedCharsFromTHBoard[maxDataLength+1] ;
boolean newDataFromTHBoard = false; 
float tempreture, humidity;
//Light Board
char receivedCharsFromLightBoard[maxDataLength+1] ;
boolean newDataFromLightBoard = false;  
float lux;
//LED
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//
const int tempVar = 25, luxVar = 600, upperHumi = 80, lowerHumi = 50; 
int drop;

void setup() {
  Serial.begin(9600);
  BTserial.begin(9600);
  newDataFromTHBoard = false;
  tempreture = 0.00;
  humidity = 0.00;
  newDataFromLightBoard = false;
  drop = 0;
  lux = 0.00;
  lcd.begin(20, 4);
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("Watering Rate is ");
  lcd.setCursor(0, 1);
  lcd.print(drop); lcd.print(" drops per minute.");

  recvDataFromTHBoard();                                           
  if (newDataFromTHBoard) {splitTemperatureAndHumidity(); wateringSystem();}      

  recvDataFromLightBoard(); 
  if (newDataFromLightBoard) {splitLux(); wateringSystem();}

  delay(100);
}

void wateringSystem() {
  if (humidity > upperHumi) {drop = 0;}
  else if(humidity < lowerHumi) {drop = 15;}
  else {
    if (tempreture < tempVar) {
      if (lux < luxVar) {drop = 10;}
      else {drop = 5;}
    } 
    else {drop = 10;} 
  }
}
void recvDataFromLightBoard() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  if (Serial.available() > 0) {
    char rc = Serial.read();
    Serial.print("rc_L: "); Serial.println(rc);
    if (recvInProgress == true) {
      if (rc != endMarker){
        if (ndx < maxDataLength) { receivedCharsFromLightBoard[ndx] = rc; ndx++;  }
      }
      else {
        receivedCharsFromLightBoard[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
        newDataFromLightBoard = true;
      }
    }
    else if (rc == startMarker) { recvInProgress = true; }
  }
}

void splitLux() {
  newDataFromLightBoard = false;
  lux = atof(receivedCharsFromLightBoard);
  Serial.print("Recieved Data From Light-Borad: ");  Serial.println(lux);
}

void recvDataFromTHBoard() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  if (BTserial.available() > 0) {
    char rc = BTserial.read();
    Serial.print("rc_T: "); Serial.println(rc);
    if (recvInProgress == true) {
      if (rc != endMarker){
        if (ndx < maxDataLength) { receivedCharsFromTHBoard[ndx] = rc; ndx++;  }
      }
      else {
        receivedCharsFromTHBoard[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
        newDataFromTHBoard = true;
      }
    }
    else if (rc == startMarker) { recvInProgress = true; }
  }
}

void splitTemperatureAndHumidity() {
  newDataFromTHBoard = false;
  Serial.print("Recieved Data From TH-Borad: ");  Serial.println(receivedCharsFromTHBoard);
  char* tempretureStr = strtok(receivedCharsFromTHBoard, ":");
  char* humidityStr = strtok(NULL, ":");
  tempreture = atof( tempretureStr);
  humidity = atof( humidityStr);
  Serial.print("tempreture: "); Serial.println(tempreture);
  Serial.print("humidity: "); Serial.println(humidity);
}
