#include <stdio.h>
//#include <stdlib.h>
#include <xc.h>
#include <htc.h>

#include "setup.h"

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