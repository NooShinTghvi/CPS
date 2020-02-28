#include <AltSoftSerial.h>
#include <LiquidCrystal.h>
#include<stdio.h>
#include <string.h>
AltSoftSerial BTserial; 

const byte maxDataLength = 20;
//TH Board         
char receivedCharsFromTHBoard[maxDataLength+1] ;
boolean newDataFromTHBoard = false; 
float tempreture, humidity;
//Light Board
char receivedCharsFromLightBoard[maxDataLength+1] ;
boolean newDataFromLightBoard = false;  
int lux;
//LED
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
     Serial.begin(9600);
     BTserial.begin(9600);
     newDataFromTHBoard = false;
     tempreture = 0.00;
     humidity = 0.00;
     newDataFromLightBoard = false;
     lux = 0;
     lcd.begin(20, 4);
}

void loop() {
     lcd.setCursor(0, 0);
     lcd.println("Hello");
     lcd.setCursor(0, 1);
     lcd.println("12");

     recvDataFromTHBoard();                                           
     if (newDataFromTHBoard) {splitTemperatureAndHumidity();}      

     recvDataFromLightBoard(); 
     if (newDataFromLightBoard) {splitLux();}
     
     // Serial.print("LDR = " );Serial.println(Serial.read());
     // Serial.print("Bluetooth = " );Serial.println(BTserial.read());
     delay(1100);
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
     lux = atoi(receivedCharsFromLightBoard);
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
     // Serial.print("Recieved Data From TH-Borad: ");  Serial.println(receivedCharsFromTHBoard);
     char* tempretureStr = strtok(receivedCharsFromTHBoard, ":");
     char* humidityStr = strtok(NULL, ":");
     memmove(tempretureStr, tempretureStr + 1, strlen(tempretureStr));
     // Serial.print("tempretureStr: "); Serial.println(tempretureStr);
     humidityStr[strlen(humidityStr)-1] = '\0';
     // Serial.print("humidityStr: "); Serial.println(humidityStr );
     tempreture = atof( tempretureStr);
     humidity = atof( humidityStr);
}
