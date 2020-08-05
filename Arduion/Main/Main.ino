#include<stdio.h>
#include <string.h>
#include <AltSoftSerial.h>
AltSoftSerial BTserial;
const byte maxDataLength = 20;
//Photoresistor Board
char receivedCharsFromPhotoresistorBoard[maxDataLength+1] ;
boolean newDataFromPhotoresistor = false;
float rightLight, leftLight;
void setup() {
    Serial.begin(9600);
    BTserial.begin(9600);
    newDataFromPhotoresistor = false;
}

void loop() {
    recvDataFromPhotoresistorBoard();
    if (newDataFromPhotoresistor) {splitRightLightAndLeftRight(); }
}

void recvDataFromPhotoresistorBoard() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    if (BTserial.available() > 0) {
        char rc = BTserial.read();
        Serial.print("p_r: "); Serial.println(rc);
        if (recvInProgress == true) {
            if (rc != endMarker){
                if (ndx < maxDataLength) { receivedCharsFromPhotoresistorBoard[ndx] = rc; ndx++;  }
            }
            else {
                receivedCharsFromPhotoresistorBoard[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newDataFromPhotoresistor = true;
            }
        }
        else if (rc == startMarker) { recvInProgress = true; }
    }
}
void splitRightLightAndLeftRight() {
    newDataFromPhotoresistor = false;
    Serial.print("Recieved Data From Photoresistor-Borad: ");  Serial.println(receivedCharsFromPhotoresistorBoard);
    char* rightLightStr = strtok(receivedCharsFromPhotoresistorBoard, ":");
    char* leftLightStr = strtok(NULL, ":");
    rightLight = atof( rightLightStr);
    leftLight = atof( leftLightStr);
    Serial.print("rightLight: "); Serial.println(rightLight);
    Serial.print("leftLight: "); Serial.println(leftLight);
}
