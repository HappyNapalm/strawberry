
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

#include "setup.h"
#include "animations.h"

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




void main (void)
{

    setup();
    clear_LED();
    while(1)
    {
        check_input();
        LED_Pattern_Master(gucAnimationState);
    }
 

}