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
//1 Led at a time
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
            ucCount = 0;
        }
        LED_Output_Single(ucCount, 1);
        ucCount++;
    }
}
void LED_Pattern_02 (void)
{
    
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