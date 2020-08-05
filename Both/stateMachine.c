#include <stdio.h>


#define Td1 100
#define Td2 100
#define Tt1 100
#define Tt2 100

typedef enum
{
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
    double_error,
    t5_error
} systemState;

typedef enum
{
    00,01,10,11
} L;

typedef enum
{
    ls,eq,gt,  //Less, equal, greater
    checked
} CHECHING;

L determinePositionOfBallInHand(int lr,ll){
    int ballInHandValue = 150;

    if (lr < ballInHandValue) lr =1;
    else{lr = 0;}
    
    if (ll < ballInHandValue) ll =1;
    else{ll = 0;}

    if (!(lr || ll)) return 00;
    else if (lr ^ ll ){return 01;}
    else if (lr || ll ){return 10;}
    else
    {return 11;}    
}

CHECHING checkThePressureOnHand(int p){
    int Pd1 100;
    int Pd2 100;
    if (p < Pd1){return ls;}
    else if (p> Pd2){return gt;}
    else
    {
        return eq;
    }
    
        
}

int main(int argc, char *argv[]){
    systemState state = fs_1;

    L ballInHandState = determinePositionOfBallInHand();
    CHECHING ballPressureStatus = checkThePressureOnHand();
    switch (state) {  // determine next state
    case fs_1:
        if(ballInHandState == 00)
            state = fs_1;
        else if (determinePositionOfBallInHand == 10 && ballPressureStatus == checked)
            state = fs_2;
        else if (determinePositionOfBallInHand == 11 && ballPressureStatus == checked)
            state = fs_9;
        break;
    case fs_2:
        state = fs_3;
    case fs_3:
        state = fs_3;
        
    default:
        break;
    }

    switch (state) {  // determine each state what should do
    case fs_1:
    case fs_2:
        state = fs_3;
    case fs_3:
        state = fs_3;
        
    default:
        break;
    }
}