#include<stdio.h>
#include <string.h>

float rightLight, leftLight, previousRightLight, previousLeftLight;

void setup() {
    pinMode(A0, INPUT); //LDR Right Hand
    pinMode(A1, INPUT); //LDR Left Hand
    Serial.begin(9600);
    previousRightLight = 0.00;
    previousLeftLight = 0.00;
}

void loop() {
    rightLight = analogRead(A0);
    leftLight = analogRead(A1);
    if (rightLight != previousRightLight || leftLight != previousLeftLight) {
        Serial.print(linkData(rightLight,leftLight));
        previousRightLight = rightLight;
        previousLeftLight = leftLight;
    }
}
String linkData(float rightLight, float leftLight) {
    String rightLightStr = String(rightLight);
    String leftLightStr = String(leftLight);
    return "<" + rightLightStr + ":" + leftLightStr + ">";
}
