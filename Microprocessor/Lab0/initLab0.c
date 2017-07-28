/*
* File:   initLab0.c
* Author: 
*
* Created on December 27, 2014, 1:31 PM
*/

#include "p24FJ64GA002.h"
#include "initLab0.h"

#define FCY 14745600
#define TIME_DELAY 1

void initLEDs(){
	TRISBbits.TRISB15 = 0;
	TRISBbits.TRISB14 = 0;
	TRISBbits.TRISB13 = 0;
	TRISBbits.TRISB12 = 0;

	LED4 = OFF;
	LED5 = OFF;
	LED6 = OFF;
	LED7 = OFF;
}

void initTimer1(){
	// This is so that we do not have to calculate PR by hand.
	unsigned int prVal = (FCY*TIME_DELAY) / 256.0 - 1;
	PR1 = prVal;
	T1CONbits.TCKPS = 0b11;  // Start Timer1 with prescaler OF 256 and clock
                                 // source set to the internal instruction cycle
	IEC0bits.T1IE = 1; // Enable Timer1 interrupts
	IFS0bits.T1IF = 0; // Clear the Timer1 interrupt status flag
	T1CONbits.TON = 1; // Turn the timer 1 on
}

void initSW1(){
	/* Initialize the pin on the development board to be an input. According to
	* the datasheet, this is RB5 or pin 14
	*/
	TRISBbits.TRISB5 = 1;

	//Put the overall CN Interrupt flag down (set it to 0)
	IFS1bits.CNIF = 0;

	//Enable the overall change notification interrupt
	IEC1bits.CNIE = 1;

	/* Enable the change notification interrupt for the switch 1 connected to
	* the development board. This corresponds to CN27 according to the PIC24F
	* datasheet. The internal pull-up should not be used for the switch
	* on the development board because one is already there.
	*/
	CNEN2bits.CN27IE = 1;
}

/* This is a nice helper function which turns on a particular LED
*/
void turnOnLED(int led){
	if (led == 4){
		LED4 = ON;
		LED5 = OFF;
		LED6 = OFF;
		LED7 = OFF;
	}
	else if (led == 5){
		LED4 = OFF;
		LED5 = ON;
		LED6 = OFF;
		LED7 = OFF;
	}
	else if (led == 6){
		LED4 = OFF;
		LED5 = OFF;
		LED6 = ON;
		LED7 = OFF;
	}
	else if (led == 7){
		LED4 = OFF;
		LED5 = OFF;
		LED6 = OFF;
		LED7 = ON;
	}
}
