#include <stdio.h>
//#include <stdlib.h>
#include <xc.h>
#include <htc.h>

#include "setup.h"

#define MAX_ANIMATIONS 15
#define BUTTON PORTBbits.RB0
#define NUM_LEDS 15


unsigned char gucAnimationState;




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

static unsigned char gbButtonState;
static unsigned char gucDebounceTime = 200;

void check_input(void){
    unsigned short uw;
    uw =  check_time0();
    if (BUTTON)
    {
        gbButtonState = 1;
    }
    if (uw > gucDebounceTime)
    {
        gucAnimationState++;
    }
}

/*
 *LATD/LATC and equivalent LED.
 * D1 -> Pin 37 -> RC3              D2 -> Pin 1 -> RC7
 * D3 -> Pin 41 -> RD3              D4 -> Pin 5 -> RD7
 * D5 -> Pin 36 -> RC2              D6 -> Pin 44 -> RC6
 * D7 -> Pin 40 -> RD2              D8 -> Pin 4 -> RD6
 * D9 -> Pin 35 -> RC1              D10 -> Pin 43 -> RC5
 * D11 -> Pin 39 -> RD1             D12 -> Pin 3 -> RD5
 * D13 -> Pin 32 -> RC0             D14 -> Pin 42 -> RC4
 * D15 -> Pin 38 -> RD0             D16 -> Pin 2 -> RD4
 */

//This function maps the led number to the pin in the table above. ucLED and bSignal
//are filtered to behave as expected in the function, reducing it the lower nibble
//for the led being addressed.
void LED_Output_Single (unsigned char ucLED, unsigned char bSignal)
{
    bSignal = bSignal ? 1 : 0;
    ucLED = ucLED & 0x0F;
    switch (ucLED)
    {
        case 0:
            LATCbits.LATC3 = bSignal;
            break;
        case 1:
            LATCbits.LATC7 = bSignal;
            break;
        case 2:
            LATDbits.LATD3 = bSignal;
            break;
        case 3:
            LATDbits.LATD5 = bSignal;
            break;
        case 4: 
            LATCbits.LATC2 = bSignal;
            break;
        case 5:
            LATCbits.LATC6 = bSignal;
            break;
        case 6:
            LATDbits.LATD2 = bSignal;
            break;
        case 7:
            LATDbits.LATD6 = bSignal;
            break;
        case 8:
            LATCbits.LATC1 = bSignal;
            break;
        case 9:
            LATCbits.LATC5 = bSignal;
            break;
        case 10:
            LATDbits.LATD1 = bSignal;
            break;
        case 11:
            LATDbits.LATD5 = bSignal;
            break;
        case 12:
            LATCbits.LATC0 = bSignal;
            break;
        case 13:
            LATCbits.LATC4 = bSignal;
            break;
        case 14:
            LATDbits.LATD0 = bSignal;
            break;
        case 15:
            LATDbits.LATD4 = bSignal;
            break;
    }
}

void clear_LED (void)
{
    LATD = 0x00;
    LATC = 0x00;
}


//Cycle up between LEDs, setting them as timer elapses
void LED_Pattern_01 (void)
{
    unsigned short uwCurrent;
    static unsigned char ucCount;
    
    uwCurrent = check_time0();
    if(uwCurrent > (32000))
    {
        clear_timer0();
        if (ucCount > 15)
        {
            clear_LED();
            ucCount = 0;
        }
        LED_Output_Single(ucCount, 1);
        ucCount++;
    }
}

//Cycle up setting LEDs then cycle down clearing LEDs
void LED_Pattern_02 (void)
{
    unsigned short uwCurrent;
    static unsigned char ucCount;
    static unsigned char bDir;
    
    uwCurrent = check_time0();
    if(uwCurrent > (32000))
    {
        clear_timer0();
        if (ucCount > 15)
        {
            ucCount = 0;
            bDir = !bDir;
        }
        LED_Output_Single(ucCount, bDir);
        ucCount++;
    }
}
void LED_Pattern_03 (void)
{
    
}
void LED_Pattern_04 (void)
{
    
}
void LED_Pattern_05 (void)
{
    
}
void LED_Pattern_06 (void)
{
    
}
void LED_Pattern_07 (void)
{
    
}
void LED_Pattern_08 (void)
{
    
}
void LED_Pattern_09 (void)
{
    
}
void LED_Pattern_10 (void)
{
    
}
void LED_Pattern_11 (void)
{
    
}
void LED_Pattern_12 (void)
{
    
}
void LED_Pattern_13 (void)
{
    
}
void LED_Pattern_14 (void)
{
    
}
void LED_Pattern_15 (void)
{
    
}
void LED_Pattern_16 (void)
{
    
}
void LED_Pattern_Master(unsigned char ucAnimationCount)
{
    switch(ucAnimationCount)
    {
        case 0:
            LED_Pattern_01();
            break;
        case 1:
            LED_Pattern_02();
            break;
        case 2:
            LED_Pattern_03();
            break;
        case 3:
            LED_Pattern_04();
            break;
        case 4:
            LED_Pattern_05();
            break;
        case 5:
            LED_Pattern_06();
            break;
        case 6:
            LED_Pattern_07();
            break;
        case 7:
            LED_Pattern_08();
            break;
        case 8:
            LED_Pattern_09();
            break;
        case 9:
            LED_Pattern_10();
            break;
        case 10:
            LED_Pattern_11();
            break;
        case 11:
            LED_Pattern_12();
            break;
        case 12:
            LED_Pattern_13();
            break;
        case 13:
            LED_Pattern_14();
            break;
        case 14:
            LED_Pattern_15();
            break;
        case 15:
            LED_Pattern_16();
            break;
    }
}