#include "initSW1.h"
#include "p24FJ64GA002.h"

void initSW1(){
    TRISBbits.TRISB5 = 1;    // makes Switch 1 an input
    IFS1bits.CNIF = 0;    // Change Notification FLAG.
    CNEN2bits.CN27IE = 1; // Interrupt Enable for Switch 1.
    IEC1bits.CNIE = 1;    // Change Notification Interrupt Enable.
}
