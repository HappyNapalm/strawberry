#include <stdio.h>
//#include <stdlib.h>
#include <xc.h>
#include <htc.h>
#include "animations.h"

void LED_setup (void)
{
    //Layout of board has PORTC and PORTD as LED drivers
    TRISD = 0x00;
    TRISC = 0x00;
    //LATD = 0x00;
    //LATC = 0x00;
    //Mode Switching button
    TRISB = 0xff;
    //Remember to set up a software debounce
}

void Timer_and_Interrupt_setup (void)
{
    //Let this timer run as a free running timer
    //make this a 16 bit timer?
    T0CON = 0b10010101;
    //INTCONbits.TMR0IE = 1;
    OSCCON = 0b01100111;
    //8Mhz clock. Let's keep this slow Base all timing off of this
    //read up on how to make free running timer
    
    T3CON = 0b10110101;
    
    ANSEL = 0x00;
    ANSELH = 0x00;
}

void setup(void)
{
    //INTCONbits.GIE = 0;
    LED_setup();
    Timer_and_Interrupt_setup();
    //INTCONbits.GIE = 1;
}

unsigned char read_eeprom(unsigned char ucAddress)
{
  unsigned char Data;
  EEADR = ucAddress;       // Write The Address From Which We Wonna Get Data
  EECON1bits.EEPGD = 0;  // Cleared To Point To EEPROM Not The Program Memory
  EECON1bits.RD = 1;     // Start The Read Operation
  Data = EEDATA;         // Read The Data
  return (Data);

}

void write_eeprom(unsigned char ucAddress, unsigned char ucData)
{
  while(EECON1bits.WR);  // Waits Until Last Attempt To Write Is Finished
  EEADR = ucAddress;       // Writes The Addres To Which We'll Wite Our Data
  EEDATA = ucData;         // Write The Data To Be Saved
  EECON1bits.EEPGD = 0;  // Cleared To Point To EEPROM Not The Program Memory
  EECON1bits.WREN = 1;   // Enable The Operation !
  INTCONbits.GIE = 0;    // Disable All Interrupts Untill Writting Data Is Done
  EECON2 = 0x55;         // Part Of Writing Mechanism..
  EECON2 = 0xAA;         // Part Of Writing Mechanism..
  EECON1bits.WR = 1;     // Part Of Writing Mechanism..
  INTCONbits.GIE = 1;    // Re-Enable Interrupts
  EECON1bits.WREN = 0;   // Disable The Operation
  EECON1bits.WR = 0;     // Ready For Next Writting Operation             
}

void reset_eeprom(void)
{
    unsigned char byte_count;
    for(byte_count = 0; byte_count < 100; byte_count++)
    {
        write_eeprom(byte_count,0xff);
    }
}

void HEX_2_LED(unsigned char EEPROM_MEM_ADDRESS)
{
    unsigned char LED_1;
    unsigned char LED_2;
    LED_1 = EEPROM_MEM_ADDRESS & 0x0F;
    LED_2 = (EEPROM_MEM_ADDRESS & 0xF0 >> 4);
    LED_Output_Single(LED_1,1);
    LED_Output_Single(LED_2,1);
}

//On power up make it so you cycle through the EEPROM from 0-95 (six passes)
//Read until you find one that mets the following criteria:
//  (1) Not 0xFF
//  (2) Highest value found
//  (3) Then increment the value
//  (4) Read the value and modulate by 16 to get the animation timer.
//  (5) Assume that 0xFF at address 0 means we have restarted
//  (6) If at address 95, then we can reset.


void start_up(void)
{
    unsigned char ucDataOut = 0;
    unsigned char ucValid = 0;

    /*
    while (uc != 100)
    {
        guwCheckTime = check_time0();
        if( guwCheckTime > 10000)
        {
            if (uc < 100){
                ucDataOut = read_eeprom(uc);
                if((uc%2) == 1)
                {
                    LATD = ucDataOut;
                    LATC = ~ucDataOut;
                }
                else
                {
                    LATD = ~ucDataOut;
                    LATC = ucDataOut;
                }
                
            }
            uc++;
            clear_timer0();
        }
    }
     */
    
    //Test to save 15 to eeprom address 100. And recover it on power cycle.
//    ucDataOut = read_eeprom(99);
//    
//    clear_timer0();
//    guwCheckTime = check_time0();
//    while(guwCheckTime < 10000)
//    {
//        LATD = ucDataOut;
//        guclwCheckTime = check_time0();
//    }
//    write_eeprom(99,15);
    
    ucDataOut = read_eeprom(0x0A);
    if(ucDataOut > 7)
    {
        ucDataOut = 0;
    }
    gucAnimationState = ucDataOut;
    write_eeprom(0x0A,(ucDataOut+1));
}
