#include <stdio.h>


#define Td1 100
#define Td2 100
#define Tt1 100
#define Tt2 100
#define FIVE_SEC_FAULT 4
#define LOSS_BALL_TIME 100
unsigned long startTimer, duration;
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
    00, 01, 10, 11
} L;

typedef enum {
    ls, eq, gt,  //Less, equal, greater
    checked, failed
} CHECHING;

L determinePositionOfBallInHand(int lr, ll) {
    int ballInHandValue = 150;

    if (lr < ballInHandValue) lr = 1;
    else { lr = 0; }

    if (ll < ballInHandValue) ll = 1;
    else { ll = 0; }

    if (!(lr || ll)) return 00;
    else if (lr ^ ll) { return 01; }
    else if (lr || ll) { return 10; }
    else { return 11; }
}

CHECHING checkThePressureOnHand(int p) {
    int Pd1 100;
    int Pd2 100;
    if (p < Pd1) { return ls; }
    else if (p > Pd2) { return gt; }
    else {
        return eq;
    }
}

CHECHING checkingTt(int duration) {
}

int main(int argc, char *argv[]) {
    systemState state = fs_1;
    int serial = 0;
    L ballInHandState = determinePositionOfBallInHand();
    CHECHING ballPressureStatus = checkThePressureOnHand();
    switch (state) {  // determine next state
        case fs_1:
            if (ballInHandState == 00)
                state = fs_1;
            else if (determinePositionOfBallInHand == 10 && ballPressureStatus == checked)
                state = fs_2;
            else if (determinePositionOfBallInHand == 11 && ballPressureStatus == checked)
                state = fs_9;
            else
                serial = 0;
            break;
        case fs_2:
            state = fs_3;
            break;
        case fs_3:
            duration = startTimer - millis();
            if (duration > FIVE_SEC_FAULT)
                state = fs_Error5s;
            else if (determinePositionOfBallInHand == 10)
                state = fs_3;
            else if (determinePositionOfBallInHand == 11)
                state = fs_10;
            else if (determinePositionOfBallInHand == 00)
                state = fs_3;
            else
                serial = 0; // serial.print
            break;
        case fs_4:
            state = fs_5;
            break;
        case fs_5:
            duration = startTimer - millis();
            if (determinePositionOfBallInHand == 00 && duration > LOSS_BALL_TIME)
                state = fs_1;
            else if (determinePositionOfBallInHand == 01 && duration > LOSS_BALL_TIME)
                state = fs_6;
            else
                serial = 0; // serial.print
            break;
        case fs_5:
            state = fs_8;
            break;
        case fs_7:
            state = fs_5;
            break;
        case fs_8:
            duration = startTimer - millis();
            if (determinePositionOfBallInHand == 10 && checkingTt(duration) == checked)
                state = fs_8;
            else if(determinePositionOfBallInHand == 11)
                state = fs_10;
            else if(determinePositionOfBallInHand == 00 && checkingTt(duration) == checked)
                state = fs_7;
            else if(determinePositionOfBallInHand == 10 && checkingTt(duration) == failed)
                state = fs_13;
            else
                serial = 0 ; // serial.print
            break;
        case fs_9:
            state = fs_10;
            break;
        case fs_10:
            duration = startTimer - millis();
            if (determinePositionOfBallInHand == 11 && checkingTt(duration) == checked && ballPressureStatus == checked)
                state = fs_10;
            else if(determinePositionOfBallInHand == 10)
                state = fs_3;
            else if(determinePositionOfBallInHand == 00 && checkingTt(duration) == checked)
                state = fs_11;
            else if(checkingTt(duration) == failed)
                state = fs_13;
            else
                serial = 0 ; // serial.print
            break;
        case fs_11:
            state = fs_12;
            break;
        case fs_12:
            duration = startTimer - millis();
            if (determinePositionOfBallInHand == 11 && checkingTt(duration) == checked && ballPressureStatus == checked)
                state = fs_10;
            else if(duration > LOSS_BALL_TIME)
                state = fs_1;
            else if(determinePositionOfBallInHand == 01 && checkingTt(duration) == checked)
                state = fs_errorDouble;
            else
                serial = 0 ; // serial.print
            break;
        case fs_13:
            duration = startTimer - millis();
            if (determinePositionOfBallInHand == 00)
                state = fs_14;
            else if(duration > FIVE_SEC_FAULT)
                state = fs_Error5s;
            else if(duration < FIVE_SEC_FAULT && checkingTt(duration) == failed)
                state = fs_13;
            else
                serial = 0 ; // serial.print
            break;
        case fs_14:
            state = fs_15;
            break;
        case fs_15:
            duration = startTimer - millis();
            if (determinePositionOfBallInHand == 00 && duration < LOSS_BALL_TIME)
                state = fs_15;
            else if(determinePositionOfBallInHand == 01 && duration < LOSS_BALL_TIME)
                state = fs_errorDouble;
            else if(duration > LOSS_BALL_TIME)
                state = fs_1;
            else
                serial = 0 ; // serial.print
            break;
        default:
            break;
    }

    switch (state) {  // determine each state what should do
        case fs_1:
        case fs_2:
            startTimer = millis();
            break;
        case fs_3:
        case fs_4:
            startTimer = millis();
            break;
        case fs_5:
        case fs_6:
            startTimer = millis();
            break;
        case fs_7:
            startTimer = millis();
            break;
        case fs_8:
        case fs_9:
            startTimer = millis();
            break;
        case fs_10:
        case fs_11:
            startTimer = millis();
            break;
        case fs_12:
        case fs_13:
        case fs_14:
            startTimer = millis();
            break;
        case fs_15:
        case fs_Error5s:
        case fs_errorDouble;
        default:
            break;
    }
}
