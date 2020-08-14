#include<stdio.h>
#include <string.h>

#define BOUND_PRESSURE 200
#define BOUND_LIGHT 200

float rightPressure, leftPressure, previousRightPressure, previousLeftPressure;
float rightLight, leftLight, previousRightLight, previousLeftLight;

void setup() {
    pinMode(A0, INPUT); //LDR Right Hand
    pinMode(A1, INPUT); //LDR Left Hand
    pinMode(A2, INPUT); //LDR Right Hand
    pinMode(A3, INPUT); //LDR Left Hand
    Serial.begin(9600);
    previousRightPressure = 0.00;
    previousLeftPressure = 0.00;
    previousRightLight = 0.00;
    previousLeftLight = 0.00;
}

void loop() {
    rightPressure = analogRead(A0);
    leftPressure = analogRead(A1);
    rightLight = analogRead(A2);
    leftLight = analogRead(A3);
    if (rightPressure != previousRightPressure || leftPressure != previousLeftPressure || rightLight != previousRightLight || leftLight != previousLeftLight) {
        int rp, lp, rl, ll;

        if (rightPressure < BOUND_LIGHT) rp = 0;
        else rp = 1;
        if (leftPressure < BOUND_LIGHT) lp = 0;
        else lp = 1;
        if (rightLight < BOUND_LIGHT) rl = 0;
        else rl = 1;
        if (leftLight < BOUND_LIGHT) ll = 0;
        else ll = 1;
        
        Serial.print(linkData(rp, lp, rl, ll));
        
        previousRightPressure = rightPressure;
        previousLeftPressure = leftPressure;
        previousRightLight = rightLight;
        previousLeftLight = leftLight;
    }
}
String linkData(int rp, int lp, int rl, int ll) {
    return "<" + String(rp) + ":" + String(lp) + ":" + String(rl) + ":" + String(ll) + ">";
}
