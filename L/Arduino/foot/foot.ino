#include<stdio.h>
#include <string.h>


#define BOUND 200
int fsrReadingTR, lastFsrReadingTR;
int fsrReadingHR, lastFsrReadingHR;
int fsrReadingTL, lastFsrReadingTL;
int fsrReadingHL, lastFsrReadingHL;

void setup(void) {
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  Serial.begin(9600);
  lastFsrReadingTR = 0;
  lastFsrReadingHR = 0;
  lastFsrReadingTL = 0;
  lastFsrReadingHL = 0;
}

void loop(void) {
  fsrReadingTR = analogRead(A0);
  fsrReadingTL = analogRead(A1);
  fsrReadingHR = analogRead(A2);
  fsrReadingHL = analogRead(A3);
  if (fsrReadingTR != lastFsrReadingTR || fsrReadingTL != lastFsrReadingTL || fsrReadingHL != lastFsrReadingHL || fsrReadingHR != lastFsrReadingHR ) {
    int fsrReadingTR_, fsrReadingTL_, fsrReadingHR_, fsrReadingHL_;
    if (fsrReadingTR < BOUND) fsrReadingTR_ = 0;
    else fsrReadingTR_ = 1;
    if (fsrReadingTL < BOUND) fsrReadingTL_ = 0;
    else fsrReadingTL_ = 1;
    if (fsrReadingHR < BOUND) fsrReadingHR_ = 0;
    else fsrReadingHR_ = 1;
    if (fsrReadingHL < BOUND) fsrReadingHL_ = 0;
    else fsrReadingHL_ = 1;
    Serial.println(linkData(fsrReadingTR_, fsrReadingTL_, fsrReadingHR_, fsrReadingHL_));
    lastFsrReadingTR = fsrReadingTR;
    lastFsrReadingHR = fsrReadingHR;
    lastFsrReadingTL = fsrReadingTL;
    lastFsrReadingHL = fsrReadingHL;
  }

}
String linkData(int fsrReadingTR, int fsrReadingTL, int fsrReadingHR, int fsrReadingHL) {
  String str1 = String(fsrReadingTR);
  String str2 = String(fsrReadingTL);
  String str3 = String(fsrReadingHR);
  String str4 = String(fsrReadingHL);
  return "<" + str1 + ":" + str2 + ":" + str3 + ":" + str4 + ">";
}
