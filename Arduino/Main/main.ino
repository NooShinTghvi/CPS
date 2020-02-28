#include <AltSoftSerial.h>
#include <LiquidCrystal.h>
#include<stdio.h>
#include <string.h>
AltSoftSerial BTserial; 

// Variables used for incoming data
const byte maxDataLength = 20;          // maxDataLength is the maximum length allowed for received data.
char receivedCharsFromTHBoard[maxDataLength+1] ;
char receivedCharsFromLightBoard[maxDataLength+1] ;
boolean newDataFromTHBoard = false;     // newData is used to determine if there is a new command from TH board
boolean newDataFromLightBoard = false;  // newData is used to determine if there is a new command from light board

float tempreture, humidity, lux;

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
     // put your setup code here, to run once:
     Serial.begin(9600);
     //  open software serial connection to the Bluetooth module.
     BTserial.begin(9600);
     newDataFromTHBoard = false;
     newDataFromLightBoard = false;
     tempreture = 0.00;
     humidity = 0.00;

     // set up the LCD's number of columns and rows:
     lcd.begin(20, 4);
}

void loop() {
     // put your main code here, to run repeatedly:
     lcd.setCursor(0, 0);
     lcd.println("Hello");
     lcd.setCursor(0, 1);
     lcd.println("12");

     recvDataFromTHBoard();                                           
     if (newDataFromTHBoard) { splitTemperatureAndHumidity();  }      

     recvDataFromLightBoard(); 
     if (newDataFromLightBoard) {}
     
     Serial.print("LDR = " );Serial.println(Serial.read());
     Serial.print("Bluetooth = " );Serial.println(BTserial.read());
     delay(1100);
}
void recvDataFromLightBoard() {
     static boolean recvInProgress = false;
     static byte ndx = 0;
     char startMarker = '<';
     char endMarker = '>';
     
     if (Serial.available() > 0) {
          char rc = Serial.read();
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
     Serial.print("recieved data = ");  Serial.println(receivedCharsFromLightBoard);
     memmove(receivedCharsFromLightBoard, receivedCharsFromLightBoard + 1, strlen(receivedCharsFromLightBoard));
     receivedCharsFromLightBoard[strlen(receivedCharsFromLightBoard)-1] = '\0';
     Serial.println(receivedCharsFromLightBoard );
     lux = atof( receivedCharsFromLightBoard);
}

void recvDataFromTHBoard() {
     static boolean recvInProgress = false;
     static byte ndx = 0;
     char startMarker = '<';
     char endMarker = '>';
     
     if (BTserial.available() > 0) {
          char rc = BTserial.read();
          Serial.print(rc);
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
     Serial.print("recieved data = ");  Serial.println(receivedCharsFromTHBoard);
     char* tempretureStr = strtok(receivedCharsFromTHBoard, ":");
     char* humidityStr = strtok(NULL, ":");
     memmove(tempretureStr, tempretureStr + 1, strlen(tempretureStr));
     Serial.println(tempretureStr);
     humidityStr[strlen(humidityStr)-1] = '\0';
     Serial.println(humidityStr );
     tempreture = atof( tempretureStr);
     humidity = atof( humidityStr);
}
