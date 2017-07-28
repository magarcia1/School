/* 
 * File:   Lab3p1.c
 * Author: MiguelAngel
 *
 * Created on March 23, 2015, 11:36 AM
 */

#include "p24FJ64GA002.h"
#include "lcd.h"
#include "timer.h"
#include "adc.h"
#include "pwm.h"
#include "initSW1.h"
#include <stdio.h>

_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & BKBUG_ON & COE_OFF & ICS_PGx1 &
          FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768 )

_CONFIG2( IESO_OFF & SOSCSEL_SOSC & WUTSEL_LEG & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_OFF &
          IOL1WAY_OFF & I2C1SEL_PRI & POSCMOD_XT )

#define true 1   // define true to use with bool data type.
#define false 0  // define false for bool data type.

typedef enum stateTypeEnum
{
    //TODO: Define states by name
        forward,
        backward
} stateType;


volatile int val;
volatile double voltage;
volatile stateType currState = forward;




int main(void) {

    char v[10];
    initPWMLeft();
    initPWMRight();
    initADC();
    initLCD();
    initSW1();
    clearLCD();

    while(1){
        clearLCD();
        voltage = 3.285* ((double) val)/1023;
        sprintf(v,"%.3f V",voltage);
        printStringLCD(v);
        delayMs(17); // 60 fps

        switch (currState){
            case forward:
                spinForward();
                break;
            case backward:
                spinBackward();
                break;
        }


    }
    return 1;
}

// ******************************************************************************************* //

void _ISR _ADC1Interrupt(void){
    IFS0bits.AD1IF = 0;
    val = ADC1BUF0;

    if ( voltage <= 3.285/2.0 )
    {
    OC2RS = (unsigned int) (PR3 * (double) 2 * voltage/3.285); //for right
    OC1RS = (unsigned int) (PR3); //for left
    }
    else
    {
        OC2RS = (unsigned int) (PR3);
        OC1RS = (unsigned int) (PR3 * (double) 2 * (1 - voltage/3.285)); //for left

    }


}

void _ISR _CNInterrupt(void) {
    IFS1bits.CNIF = 0;
//    delayMs(10);

    if ( _RB5 == PRESSED ){
        if (currState == forward){
            currState = backward;
        }
        else if (currState == backward){
            currState = forward;
        }
    }

}