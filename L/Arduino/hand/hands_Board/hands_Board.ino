#include<stdio.h>
#include <string.h>

#define BOUND 200
int photocellReadingL, lastPhotocellReadingL;     // the analog reading from the analog resistor divider
int photocellReadingR, lastPhotocellReadingR;     // the analog reading from the analog resistor divider

void setup(void) {
  pinMode(A0, INPUT); //LDR Left Hand  // the cell and 10K pulldown are connected to A0
  pinMode(A1, INPUT); //LDR Right Hand   // the cell and 10K pulldown are connected to A1
  // We'll send debugging information via the Serial monitor
  Serial.begin(9600);
  lastPhotocellReadingL = 0;
  lastPhotocellReadingR = 0;
}

void loop(void) {
  photocellReadingL = analogRead(A0);
  photocellReadingR = analogRead(A1);
  //  Serial.print("<1:>");
  if (photocellReadingL != lastPhotocellReadingL || photocellReadingR != lastPhotocellReadingR) {
    int result;
    // We'll have a few threshholds, qualitatively determined
    if ( ( photocellReadingR < BOUND ) || (photocellReadingL < BOUND )) result = 0;
    else result = 1;

    Serial.print("<" + String(result) + ":" + ">");
    lastPhotocellReadingL = photocellReadingL;
    lastPhotocellReadingR = photocellReadingR;
  }
}
