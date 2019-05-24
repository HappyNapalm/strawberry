#include <stdio.h>
//#include <stdlib.h>
#include <xc.h>
#include <htc.h>

#include "setup.h"

#define MAX_ANIMATIONS 16
#define BUTTON PORTBbits.RB0
#define NUM_LEDS 16

unsigned char gucAnimationState;
unsigned short guwCheckTime;

#define LONG_ANIMATION_TIME 16000
#define SHORT_ANIMATION_TIME 2000
#define BLINKY_ANIMATION_TIME 500
#define STROBE_TIME 100

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
static unsigned char gucDebounceTime = 50;

void clear_LED (void)
{
    LATD = 0x00;
    LATC = 0x00;
}

void all_on_LED (void)
{
    LATD = 0xFF;
    LATC = 0xFF;
}


/*
void check_input(void){
    unsigned short uc;
    static unsigned short uc_old;
    unsigned char bButtonStateOld;
    gbButtonState = BUTTON;
    if (
            bButtonStateOld == gbButtonState
         && BUTTON == 1
        )
    {
        uc =  check_time0();
        if (uc > (uc_old + gucDebounceTime))
        {
            LATD = ~LATD;
            LATC = ~LATC;
            gucAnimationState++;
            //gbWoot = !(gbWoot);
            uc_old = uc;
            clear_timer0();
        }
    }
    else
    {
        bButtonStateOld = gbButtonState;
    }
}
*/
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
            LATDbits.LATD7 = bSignal;
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



void array_LED_Off (unsigned char aucLEDs[], unsigned char ucSize)
{
    for (unsigned char x = 0; x < ucSize;x++)
    {
        LED_Output_Single(aucLEDs[x],0);
    }
}

void array_LED_On (unsigned char aucLEDs[], unsigned char ucSize)
{
    for (unsigned char x = 0; x < ucSize;x++)
    {
        LED_Output_Single(aucLEDs[x],1);
    }
}

//Cycle up between LEDs, setting them as timer elapses
void LED_Pattern_01 (void)
{
    static unsigned char ucCount;
    guwCheckTime = check_time0();
    if(guwCheckTime > (LONG_ANIMATION_TIME))
    {
        clear_timer0();
        if (ucCount > 32)
        {
            ucCount = 0;
        }
        
        LED_Output_Single((ucCount%NUM_LEDS), !(ucCount/NUM_LEDS));
        ucCount++;
    }
}

//Cycle up setting LEDs then cycle down clearing LEDs
void LED_Pattern_02 (void)
{
    static unsigned char ucCount;
    static signed char bDir;
    
    guwCheckTime = check_time0();
    if(guwCheckTime > (LONG_ANIMATION_TIME))
    {
        clear_timer0();
        if (ucCount > NUM_LEDS)
        {
            ucCount = 0;
        }
        if(ucCount == NUM_LEDS)
        {
            bDir = -1;
        }
        if(ucCount == 0)
        {
            bDir = 1;
        }
        LED_Output_Single(ucCount, !!(1 - bDir));
        ucCount = ucCount + bDir;
    }
}
unsigned char gaucOutRing[]={3,12,8,4,0,2,13,9,5,1,11,7,15};
//outer ring counts up, then flash.
void LED_Pattern_03 (void)
{
    //unsigned char aucOutRing[]={3,12,8,4,0,2,13,9,5,1,11,7,15};
    static unsigned char ucCount;
    static unsigned char phase;
    
    guwCheckTime = check_time0();
    if(ucCount == sizeof(gaucOutRing))
    {
        phase = 1;
        ucCount = 0;
    }
    if (!phase)
    {
        if(guwCheckTime > (LONG_ANIMATION_TIME))
        {
            LED_Output_Single((gaucOutRing[ucCount++]),1);
            clear_timer0();
        }
    }
    else
    {
        ucCount = 0;
        if(guwCheckTime > SHORT_ANIMATION_TIME)
        {
            clear_timer0();
            if(phase == 1){
                all_on_LED();
            }
            else
            {
                if((phase%2))
                {
                    LATD = ~LATC;
                }
                else
                {
                    LATC = LATD;
                }
            }
            phase++;
        }
        if(phase > 45)
        {
            phase = 0;
        }
    }
    if(ucCount > sizeof(gaucOutRing))
    {
        ucCount = 0;
    }
}
//LATC then LATD
void LED_Pattern_04 (void)
{
    static unsigned char dir;
    guwCheckTime = check_time0();
    
    if(guwCheckTime > (SHORT_ANIMATION_TIME))
    {
        clear_timer0();
        if (dir)
        {
            LATD = 0x00;
            LATC = 0xFF;
        }
        else
        {
            LATC = 0x00;
            LATD = 0xFF;
        }
        dir = ~dir;
    }
    
}
void LED_Pattern_05 (void)
{
    guwCheckTime = check_time0();
    static unsigned char ucCount;
    if(guwCheckTime > SHORT_ANIMATION_TIME)
    {
        all_on_LED();
        LED_Output_Single((gaucOutRing[ucCount++]),0);
        clear_timer0();
    }
    if(ucCount > sizeof(gaucOutRing))
    {
        ucCount = 0;
    }
}
//horizontal lines
void LED_Pattern_06 (void)
{
    guwCheckTime = check_time0();
    unsigned char aucHorizon[5][4] = {
        {11, 7, 15, 3},
        {1,14,12,0xff},
        {5,10,8,0xff},
        {9,6,4,0xff},
        {13,0,2,0xff}
    };
    
    static unsigned char ucDimen;
    
    if (guwCheckTime > SHORT_ANIMATION_TIME)
    {
        clear_timer0();
        clear_LED();
        array_LED_On(aucHorizon[ucDimen],3);
        if(aucHorizon[ucDimen][3] != 0xff)
        {
            LED_Output_Single(aucHorizon[ucDimen][3], 1);
        }
        ucDimen++;
        if(ucDimen > 5){
            ucDimen = 0;
        }
    }
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
    guwCheckTime = check_time0();
    static unsigned char ucCount;
    unsigned char ODD_LEDs[] = {0,2,4,6,8,10,12,14};
    unsigned char EVEN_LEDs[] = {1,3,5,7,9,11,13,15};
    //unsigned char ucSize;
    if(guwCheckTime > LONG_ANIMATION_TIME)
    {
        clear_timer0();
        if( ucCount%2 )
        {
            //ucSize = sizeof(ODD_LEDs[]);
            if(ucCount/2)
            {
                array_LED_Off(ODD_LEDs,sizeof(ODD_LEDs));
            }
            else
            {
                array_LED_On(ODD_LEDs,sizeof(ODD_LEDs));
            }
        }
        else
        {
            if(ucCount/3)
            {
                array_LED_Off(EVEN_LEDs,sizeof(EVEN_LEDs));
            }
            else
            {
                array_LED_On(EVEN_LEDs,sizeof(EVEN_LEDs));
            }
        }
        ucCount++;
    }
    if(ucCount > 4)
    {
        ucCount = 0;
    }
}
void LED_Pattern_12 (void)
{
    
}
void LED_Pattern_13 (void)
{
    
}
//Blink all quicker and quicker
void LED_Pattern_14 (void)
{
    guwCheckTime = check_time0();
    static unsigned char ucCount;
    if(ucCount/6 == 0||(ucCount/6 == 4))
    {
        if(guwCheckTime > LONG_ANIMATION_TIME)
        {
            clear_timer0();
            if (ucCount%2)
            {
                all_on_LED();
            }
            else
            {
                clear_LED();
            }
            ucCount++;
        }
    }
    else if ((ucCount/6 == 1)||(ucCount/6 == 3))
    {
        if(guwCheckTime > SHORT_ANIMATION_TIME)
        {
            clear_timer0();
            if (ucCount%2)
            {
                all_on_LED();
            }
            else
            {
                clear_LED();
            }
            ucCount++;
        }
    }
    else if (ucCount/6 == 2)
    {
        if(guwCheckTime > BLINKY_ANIMATION_TIME)
        {
            clear_timer0();
            if (ucCount%2)
            {
                all_on_LED();
            }
            else
            {
                clear_LED();
            }
            ucCount++;
        }
    }
    else
    {
        ucCount = 0;
    }
}
void LED_Pattern_15 (void)
{
    
}
void LED_Pattern_16 (void)
{
    
}
void LED_Pattern_Master(unsigned char ucAnimationCount)
{
    ucAnimationCount = ucAnimationCount & 0x08;
    static unsigned char ucLocalAnimation;
    if (ucLocalAnimation != ucAnimationCount){
        clear_LED();
        ucLocalAnimation = ucAnimationCount;
    }
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
            //LED_Pattern_7();
            LED_Pattern_11();
            break;
        case 7:
            //LED_Pattern_08();
            LED_Pattern_14();
            break;
        /*
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
         */
    }
}

void clear_timer3(void){
     TMR3H = 0x0000;
}

unsigned short check_timer3(void){
    unsigned short uw;
    uw = (TMR3H << 7) + TMR3L;
    return(uw);
}
#define LOOP_TIME 33000
void Animation_Loop_Timer(void)
{
    static unsigned short uw;
    
    uw = check_timer3();
    if (uw > LOOP_TIME)
    {
        clear_LED();
        gucAnimationState++;
        clear_timer3();
    }
    if(gucAnimationState > 7){
        gucAnimationState = 0;
    }
    LED_Pattern_Master(gucAnimationState);
}
 