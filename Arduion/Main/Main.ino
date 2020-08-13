#include<stdio.h>
#include <string.h>
#include <AltSoftSerial.h>
AltSoftSerial BTserial;
const byte maxDataLength = 20;

//Photoresistor Board
char receivedCharsFromPhotoresistorBoard[maxDataLength+1] ;
boolean newDataFromPhotoresistor = false;
float rightLight, leftLight;

//Pressure Board
char receivedCharsFromPressureBoard[maxDataLength+1] ;
boolean newDataFromPressure = false;
float rightPressure, leftPressure;

//State machine
typedef enum {
    fs_1,
    fs_2,
    fs_3,
    fs_4,
    fs_5,
    fs_6,
    fs_7,
    fs_8,
    fs_9,
    fs_10,
    fs_11,
    fs_12,
    fs_13,
    fs_14,
    fs_15,
    fs_errorDouble,
    fs_Error5s
} systemState;

typedef enum {
    ZERO_ZERO, ZERO_ONE, ONE_ZERO, ONE_ONE
} L;

typedef enum {
    CHECKED, FAILED
} CHECHING;

#define FIVE_SEC_FAULT 4
#define MAX_DURATION_OF_RELEASE_OF_BALL 100
unsigned long startTimer, duration;
L ballInHandState;
CHECHING ballPressureStatus;
systemState state;
void setup() {
    Serial.begin(9600);
    BTserial.begin(9600);
    newDataFromPhotoresistor = false;
    newDataFromPressure = false;
    ballInHandState = ZERO_ZERO;
    ballPressureStatus = CHECKED;
    state = fs_1;
}

void loop() {
    recvDataFromPhotoresistorBoard();
    if (newDataFromPhotoresistor) {
        splitRightLightAndLeftLight();
        ballInHandState = determinePositionOfBallInHand(rightLight, leftLight);
    }
    recvDataFromPressureBoard();
    if (newDataFromPressure) {
        splitRightPressureAndLeftPressure();
        ballPressureStatus = checkThePressureOnHand(rightPressure, leftPressure);
    }

    switch (state) {  // determine next state
        case fs_1:
            Serial.println("fs_1");
            if (ballInHandState == ZERO_ZERO)
                state = fs_1;
            else if (ballInHandState == ONE_ZERO)
                state = fs_2;
            else if (ballInHandState == ONE_ONE)
                state = fs_9;
            else
                Serial.println("fs_1_");
            break;
        case fs_2:
            Serial.println("fs_2");
            state = fs_3;
            break;
        case fs_3:
            Serial.println("fs_3");
            duration = startTimer - millis();
            if (duration > FIVE_SEC_FAULT)
                state = fs_Error5s;
            else if (ballInHandState == ONE_ZERO)
                state = fs_3;
            else if (ballInHandState == ONE_ONE)
                state = fs_10;
            else if (ballInHandState == ZERO_ZERO)
                state = fs_4;
            else
                Serial.println("fs_3_");
            break;
        case fs_4:
            Serial.println("fs_4");
            state = fs_5;
            break;
        case fs_5:
            Serial.println("fs_5");
            duration = startTimer - millis();
            if (ballInHandState == ZERO_ZERO && duration > MAX_DURATION_OF_RELEASE_OF_BALL)
                state = fs_1;
            else if (ballInHandState == ZERO_ONE && duration > MAX_DURATION_OF_RELEASE_OF_BALL) //TODO what is Te
                state = fs_6;
            else if (ballInHandState == ZERO_ONE && duration > MAX_DURATION_OF_RELEASE_OF_BALL) //TODO what is Te
                state = fs_6;
            else
                Serial.println("fs_5_");
            break;
        case fs_6:
            Serial.println("fs_6");
            state = fs_8;
            break;
        case fs_7:
            Serial.println("fs_7");
            state = fs_5;
            break;
        case fs_8:
            Serial.println("fs_8");
            duration = startTimer - millis();
            if (ballInHandState == ONE_ZERO && checkingTt(duration) == CHECKED)
                state = fs_8;
            else if(ballInHandState == ONE_ONE)
                state = fs_10;
            else if(ballInHandState == ZERO_ZERO && checkingTt(duration) == CHECKED)
                state = fs_7;
            else if(ballInHandState == ONE_ZERO && checkingTt(duration) == FAILED)
                state = fs_13;
            else
                Serial.println("fs_8_");
            break;
        case fs_9:
            Serial.println("fs_9");
            state = fs_10;
            break;
        case fs_10:
            Serial.println("fs_10");
            duration = startTimer - millis();
            if (ballInHandState == ONE_ONE && checkingTt(duration) == CHECKED && ballPressureStatus == CHECKED)
                state = fs_10;
            else if(ballInHandState == ONE_ZERO)
                state = fs_3;
            else if(ballInHandState == ZERO_ZERO && checkingTt(duration) == CHECKED)
                state = fs_11;
            else if(checkingTt(duration) == FAILED)
                state = fs_13;
            else
                Serial.println("fs_10_");
            break;
        case fs_11:
            Serial.println("fs_11");
            state = fs_12;
            break;
        case fs_12:
            Serial.println("fs_12");
            duration = startTimer - millis();
            if (ballInHandState == ONE_ONE && checkingTt(duration) == CHECKED && ballPressureStatus == CHECKED)
                state = fs_10;
            else if(duration > MAX_DURATION_OF_RELEASE_OF_BALL)
                state = fs_1;
            else if(ballInHandState == ZERO_ONE && checkingTt(duration) == CHECKED)
                state = fs_errorDouble;
            else
                Serial.println("fs_12_");
            break;
        case fs_13:
            Serial.println("fs_13");
            duration = startTimer - millis();
            if (ballInHandState == ZERO_ZERO)
                state = fs_14;
            else if(duration > FIVE_SEC_FAULT)
                state = fs_Error5s;
            else if(duration < FIVE_SEC_FAULT && checkingTt(duration) == FAILED)
                state = fs_13;
            else
                Serial.println("fs_13_");
            break;
        case fs_14:
            Serial.println("fs_14");
            state = fs_15;
            break;
        case fs_15:
            Serial.println("fs_15");
            duration = startTimer - millis();
            if (ballInHandState == ZERO_ZERO && duration < MAX_DURATION_OF_RELEASE_OF_BALL)
                state = fs_15;
            else if(ballInHandState == ZERO_ONE && duration < MAX_DURATION_OF_RELEASE_OF_BALL)
                state = fs_errorDouble;
            else if(duration > MAX_DURATION_OF_RELEASE_OF_BALL)
                state = fs_1;
            else
                Serial.println("fs_15_");
            break;
        default:
            break;
    }
    switch (state) {  // determine each state what should do
        case fs_1:
            break;
        case fs_2:
            startTimer = millis();
            break;
        case fs_3:
            break;
        case fs_4:
            startTimer = millis();
            break;
        case fs_5:
            break;
        case fs_6:
            startTimer = millis();
            break;
        case fs_7:
            startTimer = millis();
            break;
        case fs_8:
            break;
        case fs_9:
            startTimer = millis();
            break;
        case fs_10:
            break;
        case fs_11:
            startTimer = millis();
            break;
        case fs_12:
            break;
        case fs_13:
            break;
        case fs_14:
            startTimer = millis();
            break;
        case fs_15:
            break;
        case fs_Error5s:
            Serial.println("5 second error");
            break;
        case fs_errorDouble:
            Serial.println("double error");
            break;
        default:
            break;
    }
}



//Photoresistor Board
void recvDataFromPhotoresistorBoard() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    if (BTserial.available() > 0) {
        char rc = BTserial.read();
        // Serial.print("p_r: "); Serial.println(rc);
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
void splitRightLightAndLeftLight() {
    newDataFromPhotoresistor = false;
    Serial.print("Recieved Data From Photoresistor-Borad: ");  Serial.println(receivedCharsFromPhotoresistorBoard);
    char* rightLightStr = strtok(receivedCharsFromPhotoresistorBoard, ":");
    char* leftLightStr = strtok(NULL, ":");
    rightLight = atof( rightLightStr);
    leftLight = atof( leftLightStr);
    Serial.print("rightLight: "); Serial.println(rightLight);
    Serial.print("leftLight: "); Serial.println(leftLight);
}

//Pressure Board
void recvDataFromPressureBoard() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    if (Serial.available() > 0) {
        char rc = Serial.read();
        // Serial.print("p_r: "); Serial.println(rc);
        if (recvInProgress == true) {
            if (rc != endMarker){
                if (ndx < maxDataLength) { receivedCharsFromPressureBoard[ndx] = rc; ndx++;  }
            }
            else {
                receivedCharsFromPressureBoard[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newDataFromPressure = true;
            }
        }
        else if (rc == startMarker) { recvInProgress = true; }
    }
}
void splitRightPressureAndLeftPressure() {
    newDataFromPressure = false;
    Serial.print("Recieved Data From Pressure-Borad: ");  Serial.println(receivedCharsFromPressureBoard);
    char* rightPressureStr = strtok(receivedCharsFromPressureBoard, ":");
    char* leftPressureStr = strtok(NULL, ":");
    rightPressure = atof( rightPressureStr);
    leftPressure = atof( leftPressureStr);
    Serial.print("rightPressure: "); Serial.println(rightPressure);
    Serial.print("leftPressure: "); Serial.println(leftPressure);
}

L determinePositionOfBallInHand(float lr, float ll) {
    float ballInHandValue = 120.0;
    int r,l;
    if (lr < ballInHandValue) r = 1;
    else { r = 0; }

    if (ll < ballInHandValue) l = 1;
    else { l = 0; }

    if (!(r || l)) return ZERO_ZERO;
    else if (r ^ l) { return ZERO_ONE; }
    else if (r || l) { return ONE_ZERO; }
    else { return ONE_ONE; }
}

CHECHING checkThePressureOnHand(float pr, float pl) {
    float Pd1 = 1000.;
    float Pd2 = 120.0;
    //if ((pd1 < pr) && (pr < Pd2) && (pd1 < pl) && (pl < Pd2))
      return CHECKED;
    //else return FAILED;
}

CHECHING checkingTt(int duration) {
    return CHECKED;
}
