#include <AltSoftSerial.h>
AltSoftSerial BTserial;
// Time and state definitions
#define FIVE_SEC_FAULT 4
#define T_DRIBBLE 10000
#define T_t 10000
#define RUNNING_FAULT 17
#define T1 10000
#define T2 10000

// pin definitions

#define FAULT_PIN 5
#define HR_PIN 3
#define TR_PIN 4
#define HL_PIN 6
#define TL_PIN 7
//#define FAULT_PIN 5
//
int state_g;
int hr = 0;
int tr = 0;
int hl = 0;
int tl = 0;
int temp = 0 ;
unsigned long start1_g , finish1_g, duration1_g, start2_g, finish2_g, duration2_g;
unsigned long start3_g , finish3_g, duration3_g, start4_g, finish4_g, duration4_g;
int state_d;
bool check_foot ;
int l;
unsigned long start1_d , finish1_d, duration1_d, start2_d, finish2_d, duration2_d, start3_d, finish3_d, duration3_d;
const byte interruptPin = 2 ;

const byte maxDataLength = 20;
//Foot Board
char receivedCharsFromFootBoard[maxDataLength + 1] ;
boolean newDataFromFoot = false;

//Hand Board
char receivedCharsFromHandBoard[maxDataLength + 1] ;
boolean newDataFromHand = false;

void setup() {
  Serial.begin(9600);

  state_d = 0;
  check_foot = false ;
  l = digitalRead(2);
  pinMode(interruptPin, INPUT_PULLUP);
  pinMode(FAULT_PIN , OUTPUT);
  digitalWrite(FAULT_PIN , 0);
  //  attachInterrupt(0, isr, CHANGE);


  state_g = 0;
  TIMSK2 = (TIMSK2 & B11111110) | 0x01;
  TCCR2B = (TCCR2B & B11111000) | 0x07;
  pinMode(HR_PIN , INPUT_PULLUP);
  pinMode(TR_PIN , INPUT_PULLUP);
  pinMode(HL_PIN , INPUT_PULLUP);
  pinMode(TL_PIN , INPUT_PULLUP);

  newDataFromFoot = false;
  newDataFromHand = false;
}

void leg_fsm_runner()
{
  if (state_d == 6)
  {
    start3_d = millis();
  }
  if (state_d == 2)
  {
    finish1_d = millis();
    duration1_d = finish1_d - start1_d ;
  }
  while (check_foot)
  {
    if (state_d == 2)
    {
      finish1_d = millis();
      duration1_d = finish1_d - start1_d ;
      if (duration1_d >= 10000)
      {
        state_d = FIVE_SEC_FAULT ;
        return ;
      }
    }
    if (state_d == 6)
    {
      finish3_d = millis();
      duration3_d = finish3_d - start3_d;
      if (duration3_d > T_t)
      {
        state_d = 0 ;
        return ;
      }
    }

    fsm_g() ;

  }

}


void fsm_d()
{
  recvDataFromHandBoard();
  if (newDataFromHand) {
    splitHandDataBoard();
    check_foot = false ;
    state_g = 0;
  }
  recvDataFromFootBoard();
  if (newDataFromFoot) {
    splitFootDataBoard();
  }
  switch (state_d)
  {
    case 0 :
      Serial.println("fsm_d s0 ");
      s0_d();
      break;
    case 1 :
      Serial.println("fsm_d s1 ");
      s1_d();
      break;
    case 2 :
      Serial.println("fsm_d s2 ");
      s2_d();
      break;
    case 3 :
      Serial.println("fsm_d s3 ");
      s3_d();
      break;
    case 4 :
      Serial.println("fsm_d s4 - five seconds fault ");
      s4_d();
      break;
    case 5 :
      Serial.println("fsm_d s5 ");
      s5_d();
      break;
    case 6 :
      Serial.println("fsm_d s6");
      s6_d();
      break;
    case 7 :
      Serial.println("fsm_d s7");
      s7_d();
      break;
    case 8 :
      Serial.println("fsm_d s8");
      s8_d();
      break;
  }
}
// void isr()
// {
// l = digitalRead(2);
// }
void s0_d()
{
  while (l == 1)
    delayMicroseconds(1);
  state_d = 1;
  Serial.println("Inside s0");
  //fsm_d();
}
void s1_d()
{
  start1_d = millis();
  //delay(1000);
  state_d = 2;
  //fsm_d();
}
void s2_d()
{
  //  finish1_d = millis();
  //  duration1_d = finish1_d - start1_d ;
  //  while ( ( l == 0 ) && (duration1_d < 5000) )
  //  {
  //    finish1_d = millis();
  //    duration1_d = finish1_d - start1_d ;
  //  }
  check_foot = true ;
  leg_fsm_runner();
  if ( l == 1 )
  {
    state_d = 3;
    //fsm_d();
  }
  //  else if (duration1_d >= 5000)
  //  {
  //    state_d = FIVE_SEC_FAULT;
  //    five_sec_flag = true ;
  //    //fsm_d();
  //  }
}

void s3_d()
{
  start2_d = millis();
  state_d = 5 ;
  //fsm_d();
}
void s4_d()
{
  Serial.println("LAMP");
  digitalWrite(FAULT_PIN , 1);
  detachInterrupt(0);
}
void s5_d()
{
  Serial.println("Inside s5");
  while ( l == 1 )
  {
    finish2_d = millis();
    duration2_d = finish2_d - start2_d;
    if (duration2_d > T_DRIBBLE)
      break;
    delayMicroseconds(1);
  }
  if (( l == 0 ) && (duration2_d < T_DRIBBLE ) )
  {

    state_d = 6 ;
    //fsm_d();
  }
  else if (( l == 1 ) && (duration2_d > T_DRIBBLE ))
  {
    state_d = 0;
    //fsm_d();
  }
}
void s6_d()
{
  check_foot = true ;
  leg_fsm_runner() ;

  if (l == 1)
  {
    state_d = 7;
    //fsm_d();
  }
  // else
  //{
  //    state_d = 0;
  //fsm_d();
  // }

}
void s7_d()
{
  state_d = 8 ;
  //fsm_d();
}
void s8_d()
{
  start2_d = millis();
  state_d = 5 ;
  //fsm_d();
}

//  ==================================================
// marz

// l ri bayad hamishe check konim ke age 1 shod , bepare biroon az in fsm_g


// ISR(TIMER2_OVF_vect) {
// hr = digitalRead(HR_PIN);
// hl = digitalRead(HL_PIN);
// tr = digitalRead(TR_PIN);
// tl = digitalRead(TL_PIN);
// }

void fsm_g()
{
  switch (state_g)
  {
    case 0 :
      Serial.println("fsm_g s0 ");
      s0_g();
      break;
    case 1 :
      Serial.println("fsm_g s1 ");
      s1_g();
      break;
    case 2 :
      Serial.println("fsm_g s2 ");
      s2_g();
      break;
    case 3 :
      Serial.println("fsm_g s3 ");
      s3_g();
      break;
    case 4 :
      Serial.println("fsm_g s4 ");
      s4_g();
      break;
    case 5 :
      Serial.println("fsm_g s5 ");
      s5_g();
      break;
    case 6 :
      Serial.println("fsm_g s6");
      s6_g();
      break;
    case 7 :
      Serial.println("fsm_g s7");
      s7_g();
      break;
    case 8 :
      Serial.println("fsm_g s8");
      s8_g();
      break;
    case 9 :
      Serial.println("fsm_g s9");
      s9_g();
      break;
    case 10 :
      Serial.println("fsm_g s10");
      s10_g();
      break;
    case 11 :
      Serial.println("fsm_g s11");
      s11_g();
      break;
    case 12 :
      Serial.println("fsm_g s12");
      s12_g();
      break;
    case 13 :
      Serial.println("fsm_g s13");
      s13_g();
      break;
    case 14 :
      Serial.println("fsm_g s14");
      s14_g();
      break;
    case 15 :
      Serial.println("fsm_g s15");
      s15_g();
      break;
    case 16 :
      Serial.println("fsm_g s16");
      s16_g();
      break;
    case 17 :
      Serial.println("fsm_g RUNNING FAULT");
      s17_g();
      break;
  }
}
//void isr_g()
//{
//  hr = digitalRead(HR_PIN);
//  hl = digitalRead(HL_PIN);
//  tr = digitalRead(TR_PIN);
//  tl = digitalRead(TL_PIN);
//}
void s0_g()
{

  if ( (( hr == 1) || (tr == 1)) && (( hl == 1) || (tl == 1)) )
  {
    state_g =  1 ;
    Serial.println("Inside first if in s0");
    //fsm_g();
  }
  else if ( (( hl == 0) && (tl == 0)) && (( hr == 1) || (tr == 1)) )
  {
    state_g =  7 ;
    Serial.println("Inside second if in s0");
    //fsm_g();
  }

  else if ( (( hr == 0) && (tr == 0)) && (( hl == 1) || (tl == 1)) )
  {
    state_g =  8 ;
    Serial.println("Inside third if in s0");
    //fsm_g();
  }
  else
  {
    //fsm_g();
  }
}

void s1_g()
{
  if ( (( hl == 0) && (tl == 0)) && (( hr == 1) || (tr == 1)) )
  {
    state_g =  2 ;
    Serial.println("Inside first if in s1");
    //fsm_g();
  }
  else if ( (( hr == 0) && (tr == 0)) && (( hl == 1) || (tl == 1)) )
  {
    state_g =  4 ;
    Serial.println("Inside second if in s1");
    //fsm_g();
  }
  else if ( (( hr == 0) && (tr == 0)) && (( hl == 0) && (tl == 0)) )
  {
    state_g =  6 ;
    Serial.println("Inside third if in s1");
    //fsm_g();
  }
  else
  {
    //fsm_g();
  }
}
void s2_g()
{
  Serial.println("HERE");
  if ( (( hl == 1) || (tl == 1)) && (( hr == 1) || (tr == 1)) )
  {
    state_g =  3 ;
    Serial.println("Inside first if in s2");
    //fsm_g();
  }
  else if ( (( hr == 0) && (tr == 0)) && (( hl == 1) || (tl == 1)) ) // tina
  {
    state_g =  RUNNING_FAULT ;
    Serial.println("Inside second if in s2");
    //fsm_g();
  }
  else
  {
    //fsm_g();
  }
}

void s3_g()
{
  if ( ( hr == 0) && (tr == 0) )
  {
    state_g =  RUNNING_FAULT ;
    Serial.println("Inside first if in s3");
    //fsm_g();
  }
  else
  {
    state_g =  3 ;
    Serial.println("Inside second if in s3");
    //fsm_g();
  }
}

void s4_g()
{
  if ( (( hl == 1) || (tl == 1)) && (( hr == 1) || (tr == 1)) )
  {
    state_g =  5 ;
    Serial.println("Inside first if in s4");
    //fsm_g();
  }
  else if ( (( hl == 0) && (tl == 0)) && (( hr == 1) || (tr == 1)) ) // tina
  {
    state_g =  RUNNING_FAULT ;
    Serial.println("Inside second if in s4");
    //fsm_g();
  }
  else
  {
    //fsm_g();
  }
}
void s5_g()
{
  if ( ( hl == 0) && (tl == 0) )
  {
    state_g =  RUNNING_FAULT ;
    Serial.println("Inside first if in s5");
    //fsm_g();
  }
  else
  {
    state_g =  5 ;
    Serial.println("Inside second if in s5");
    //fsm_g();
  }
}

void s6_g()
{
  if ( (( hl == 1) || (tl == 1)) || (( hr == 1) || (tr == 1)) ) // tina
  {
    state_g =  RUNNING_FAULT ;
    Serial.println("Inside second if in s6");
    //fsm_g();
  }
  else
  {
    Serial.println("Inside else s6");
    //fsm_g();
  }
}
void s7_g()
{
  if ( (( hl == 1) || (tl == 1)) && (( hr == 0) && (tr == 0)) )
  {
    state_g =  9 ;
    Serial.println("Inside first if in s7");
    //fsm_g();
  }
  else if ( (( hl == 1) || (tl == 1)) && (( hr == 1) || (tr == 1)) )
  {
    state_g =  3 ;
    Serial.println("Inside second if in s7");
    //fsm_g();
  }
  else
  {
    //fsm_g();
  }
}
void s8_g()
{
  if ( (( hr == 1) || (tr == 1)) && (( hl == 0) && (tl == 0)) )
  {
    state_g =  13 ;
    Serial.println("Inside first if in s8");
    //fsm_g();
  }
  else if ( (( hl == 1) || (tl == 1)) && (( hr == 1) || (tr == 1)) )
  {
    state_g =  5 ;
    Serial.println("Inside second if in s8");
    //fsm_g();
  }
  else
  {
    //fsm_g();
  }
}

void s9_g()
{
  start1_g = millis();
  state_g = 10 ;
  //fsm_g();
}

void s10_g()
{
  //  finish1_g = millis();
  //  duration1_g = finish1_g - start1_g ;
  while ( 1 )
  {
    finish1_g = millis();
    duration1_g = finish1_g - start1_g ;
    //delay(3);
    Serial.println("duration");
    //Serial.println(duration1_g);
    if ( duration1_g > T1)
    {
      state_g = RUNNING_FAULT ;
      break;
      //  fsm_g();
    }
    else if ( (( hr == 1) || (tr == 1)) && (( hl == 0) && (tl == 0)) )
    {
      state_g = 11 ;
      break;
      //  fsm_g();
    }
    else if ( (( hl == 1) || (tl == 1)) && (( hr == 1) || (tr == 1)) )
    {
      state_g = RUNNING_FAULT ;
      break;
      //  fsm_g();
    }
    else
    {
      state_g = 10 ;
      break;
      //  fsm_g();
    }
  }
}

void s11_g()
{
  start2_g = millis();
  state_g = 12 ;
  //fsm_g();
}

void s12_g()
{
  //  finish2_g = millis();
  //  duration2_g = finish2_g - start2_g ;
  while ( 1 )
  {
    finish2_g = millis();
    duration2_g = finish2_g - start2_g ;

    if ( duration2_g > T2)
    {
      state_g = RUNNING_FAULT ;
      break;
      //  fsm_g();
    }
    else if (( hl == 1) || (tl == 1))
    {
      state_g = RUNNING_FAULT ;
      break;
      //  fsm_g();
    }
    else
    {
      state_g = 12 ;
      break;
      //  fsm_g();
    }
  }
}

void s13_g()
{
  start3_g = millis();
  state_g = 14 ;
  // fsm_g();
}

void s14_g()
{
  //  finish3_g = millis();
  //  duration3_g = finish3_g - start3_g ;
  while ( 1 )
  {
    finish3_g = millis();
    duration3_g = finish3_g - start3_g ;

    if ( duration3_g > T1)
    {
      state_g = RUNNING_FAULT ;
      break;
      //   fsm_g();
    }
    else if ( (( hl == 1) || (tl == 1)) && (( hr == 0) && (tr == 0)) )
    {
      state_g = 15 ;
      break;
      // fsm_g();
    }
    else if ( (( hl == 1) || (tl == 1)) && (( hr == 1) || (tr == 1)) )
    {
      state_g = RUNNING_FAULT ;
      break;
      //  fsm_g();
    }
    else
    {
      state_g = 14 ;
      break;
      //  fsm_g();
    }
  }
}
void s15_g()
{
  start4_g = millis();
  state_g = 16 ;
  // fsm_g();
}

void s16_g()
{
  //  finish4_g = millis();
  //  duration4_g = finish4_g - start4_g ;
  while ( 1 )
  {
    finish4_g = millis();
    duration4_g = finish4_g - start4_g ;

    if ( duration4_g > T2)
    {
      state_g = RUNNING_FAULT ;
      break;
      //   fsm_g();
    }
    else if (( hr == 1) || (tr == 1))
    {
      state_g = RUNNING_FAULT ;
      break;
      //  fsm_g();
    }
    else
    {
      state_g = 16 ;
      break;
      //  fsm_g();
    }
  }
}

void s17_g()
{
  Serial.println("RUNNING FUALT");
  digitalWrite(FAULT_PIN , 1);
  detachInterrupt(0);

}




//void loop() { // run over and over
// if(state_g != RUNNING_FAULT)
// {
//  fsm_g();
// }
// else if (flag == false)
// {
//   Serial.println("RUNNING FAULT !!!");
//   flag = true ;
// }
// else
// {
// }
//}



void loop()
{
  fsm_d();
}


//Foot Board
void recvDataFromFootBoard() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  if (BTserial.available() > 0) {
    char rc = BTserial.read();
    // Serial.print("p_r: "); Serial.println(rc);
    if (recvInProgress == true) {
      if (rc != endMarker) {
        if (ndx < maxDataLength) {
          receivedCharsFromFootBoard[ndx] = rc;
          ndx++;
        }
      }
      else {
        receivedCharsFromFootBoard[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
        newDataFromFoot = true;
      }
    }
    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
}
void splitFootDataBoard() {
  newDataFromFoot = false;
  Serial.print("Recieved Data From Foot-Borad: ");  Serial.println(receivedCharsFromFootBoard);
  char* str1 = strtok(receivedCharsFromFootBoard, ":");
  char* str2 = strtok(NULL, ":");
  char* str3 = strtok(NULL, ":");
  char* str4 = strtok(NULL, ":");
  tr = atoi(str1);
  tl = atoi(str2);
  hr = atoi(str3);
  hl = atoi(str4);
  Serial.print("tr: "); Serial.println(tr);
  Serial.print("tl: "); Serial.println(tl);
  Serial.print("hr: "); Serial.println(hr);
  Serial.print("hl: "); Serial.println(hl);
}

//Hand Board
void recvDataFromHandBoard() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  if (Serial.available() > 0) {
    char rc = Serial.read();
    // Serial.print("p_r: "); Serial.println(rc);
    if (recvInProgress == true) {
      if (rc != endMarker) {
        if (ndx < maxDataLength) {
          receivedCharsFromHandBoard[ndx] = rc;
          ndx++;
        }
      }
      else {
        receivedCharsFromHandBoard[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
        newDataFromHand = true;
      }
    }
    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
}
void splitHandDataBoard() {
  newDataFromHand = false;
  Serial.print("Recieved Data From Hand-Borad: ");  Serial.println(receivedCharsFromHandBoard);
  char* str1 = strtok(receivedCharsFromHandBoard, ":");
  l = atoi( str1);
  Serial.print("l: "); Serial.println(l);
}
