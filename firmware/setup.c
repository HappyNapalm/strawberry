#include <stdio.h>
//#include <stdlib.h>
#include <xc.h>
#include <htc.h>

void LED_setup (void)
{
    //Layout of board has PORTC and PORTD as LED drivers
    TRISD = 0x00;
    TRISC = 0x00;
    //LATD = 0x00;
    //LATC = 0x00;
    //Mode Switching button
    TRISB = 0b00000001;
    //Remember to set up a software debounce
}

void Timer_and_Interrupt_setup (void)
{
    //Let this timer run as a free running timer
    //make this a 16 bit timer?
    T0CON = 0b10011000;
    //INTCONbits.TMR0IE = 1;
    OSCCON = 0b01100111;
    //8Mhz clock. Let's keep this slow Base all timing off of this
    //read up on how to make free running timer
}

void setup(void)
{
    //INTCONbits.GIE = 0;
    LED_setup();
    Timer_and_Interrupt_setup();
    //INTCONbits.GIE = 1;
}
