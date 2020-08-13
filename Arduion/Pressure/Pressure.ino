#include<stdio.h>
#include <string.h>

float rightPressure, leftPressure, previousRightPressure, previousLeftPressure;

void setup() {
    pinMode(A0, INPUT); //LDR Right Hand
    pinMode(A1, INPUT); //LDR Left Hand
    Serial.begin(9600);
    previousRightPressure = 0.00;
    previousLeftPressure = 0.00;
}

void loop() {
    rightPressure = analogRead(A0);
    leftPressure = analogRead(A1);
    if (rightPressure != previousRightPressure || leftPressure != previousLeftPressure) {
        Serial.print(linkData(rightPressure,leftPressure));
        previousRightPressure = rightPressure;
        previousLeftPressure = leftPressure;
    }
}
String linkData(float rightPressure, float leftPressure) {
    String rightPressureStr = String(rightPressure);
    String leftPressureStr = String(leftPressure);
    return "<" + rightPressureStr + ":" + leftPressureStr + ">";
}
