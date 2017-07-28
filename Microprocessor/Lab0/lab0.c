// ******************************************************************************************* //
//
// File:         lab0.c
// Date:         02/04/2015
// Authors:      Miguel A. Garcia
//
// Description:  Lab 0 assignment for ECE 372 Spring 2015.
// ******************************************************************************************* //

#include "p24FJ64GA002.h"
#include "initLab0.h"
#include <stdio.h>

// ******************************************************************************************* //
// Configuration bits for CONFIG1 settings.
//
// These settings are appropriate for debugging the PIC microcontroller. If you need to
// program the PIC for standalone operation, change the COE_ON option to COE_OFF.

_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF &
	BKBUG_ON & COE_ON & ICS_PGx1 &
	FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768)

_CONFIG2(IESO_OFF & SOSCSEL_SOSC & WUTSEL_LEG & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_OFF &
	IOL1WAY_OFF & I2C1SEL_PRI & POSCMOD_XT)


typedef enum stateTypeEnum{
	//TODO: Define states by name
	led4,
	led5,
	led6,
	led7
} stateType;

volatile stateType curState;
volatile double timer;

int main(void)
{

	//TODO: Finish these functions in the provided c files
	initLEDs();
	initTimer1();
	initSW1();
	curState = led4;

	while (1)
	{
		switch (curState){
		case led4:
			turnOnLED(4);
			break;
		case led5:
			turnOnLED(5);
			break;
		case led6:
			turnOnLED(6);
			break;
		case led7:
			turnOnLED(7);
			break;
		}
	}
	return 0;
}

void _ISR _T1Interrupt(void){
	//TODO: Put down the timer 1 flag first!
	IFS0bits.T1IF = 0;
	//TODO: Change states if necessary.
	//Make sure if you use any variables that they are declared volatile!
	// LATBbits.LATB15 = !LATBbits.LATB15;

	timer = timer + 1;
        //Program enters to this if when SW1 is not press, which means it is released
	if (PORTBbits.RB5 == RELEASED){
		timer = 0.0;
	}
}

void _ISR _CNInterrupt(void){
	IFS1bits.CNIF = 0;

        if(PORTBbits.RB5 == PRESSED){   //If SW1 is pressed, please reset timer 
            TMR1 = 0;                   //Reset TMR1 register not matter what!!!
            timer = 0.0;                //Not required, but helps to understand what is happening
        }

	if ((PORTBbits.RB5 == RELEASED) && (timer < 2)){
		if (curState == led4) curState = led5;
		else if (curState == led5) curState = led6;
		else if (curState == led6) curState = led7;
		else if (curState == led7) curState = led4;
	}
	else if ((PORTBbits.RB5 == RELEASED) && (timer >= 2)){
		if (curState == led4) curState = led7;
		else if (curState == led5) curState = led4;
		else if (curState == led6) curState = led5;
		else if (curState == led7) curState = led6;
	}
}