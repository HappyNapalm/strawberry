
/*
 * File:   main.c
 * Author: Church
 *
 * Created on February 22, 2019, 5:04 AM
 */
#include <stdio.h>
//#include <stdlib.h>
#include <xc.h>
#include <htc.h>

/** C O N F I G U R A T I O N   B I T S ******************************/
#pragma config FOSC = INTIO67
//#pragma config WDTEN = OFF, LVP = OFF, MCLRE = OFF
#pragma config WDTEN = OFF, LVP = OFF

/** I N C L U D E S **************************************************/
//#include "p18f45k20.h"
/********************************************************************/

//Ok mate, your are going to need to manipulate output bits and timers.
//remember to debounce your button inputs and your light outputs.
//You will need to read about sleep mode. Set that up after X-time 
//Make it about 5 minutes(?) since startup or the button read high.
//Remember how fun it was setting up registers?
//Go idle at 5 minutes. Read the manual to make sure that you have
//everything setup right!

#define BUTTON PORTAbits.RA0
#define NUM_LEDS 15

void LED_setup (void)
{
    //Layout of board has PORTC and PORTD as LED drivers
    TRISD = 0x00;
    TRISC = 0x00;
    //LATD = 0x00;
    //LATC = 0x00;
    //Mode Switching button
    TRISA = 0b00000001;
    //Remember to set up a software debounce
    //!!!Make it so this is an interrupt!!!
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

unsigned short check_time0 (void)
{
    unsigned short uw;
    uw = (TMR0H << 7) + TMR0L;
    return uw;
}

void clear_timer0( void )
{
    TMR0H = 0;
    TMR0L = 0;
    
}

//ucLED is 0-15. 
void LED_Output_Single (unsigned char ucLED, unsigned char bSignal)
{
    //Write value bSignal to the correct LATch. 
    if(ucLED > 7)
    {
        //8-15 LEDS
        LATD = (bSignal << (ucLED - 7));
    }
    else
    {
        //0-7 LEDS
        LATC = (bSignal << (ucLED));
    }
}

void clear_LED (void)
{
    LATD = 0x00;
    LATC = 0x00;
}


void main (void)
{
    unsigned short uwCurrent;
    static unsigned char ucCount;
    setup();
    clear_LED();
    while(1)
    {

        uwCurrent = check_time0();
        if(uwCurrent > (32000))
        {
            clear_timer0();
            if (ucCount > 15)
            {
                ucCount = 0;
            }
            LED_Output_Single(ucCount, 1);
            ucCount++;
        }
        
    }
 

}