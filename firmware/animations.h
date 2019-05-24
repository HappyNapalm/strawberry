/* 
 * File:   animations.h
 * Author: church
 *
 * Created on April 5, 2019, 5:24 PM
 */

#ifndef ANIMATIONS_H
#define	ANIMATIONS_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

unsigned short check_time0 (void);
void clear_timer0( void );
void LED_Output_Single (unsigned char ucLED, unsigned char bSignal);
void clear_LED (void);
void all_on_LED(void);


void LED_Pattern_Master(unsigned char ucAnimationCount);
void check_input(void);
extern unsigned char gucAnimationState;
void check_input(void);
extern unsigned short guwCheckTime;
void Animation_Loop_Timer(void);


#endif	/* ANIMATIONS_H */

