/** @file Driver_Timer.h
 * Driver for hardware timer 0.
 * @author Adrien RICCIARDI
 * @version 1.0 : 12/12/2011
 * @version 1.1 : 28/07/2012
 * @version 1.2 : 13/12/2013, added period configuration function and TimerSleep() function.
 */
#ifndef H_DRIVER_TIMER_H
#define H_DRIVER_TIMER_H

//------------------------------------------------------------------------------------------------------------------------------------
// Public variable
//------------------------------------------------------------------------------------------------------------------------------------
/** The raw timer value. This counter is incremented on each timer interrupt. It does roll-over when overflow. */
extern volatile unsigned int Timer_Counter;

//------------------------------------------------------------------------------------------------------------------------------------
// Public functions
//------------------------------------------------------------------------------------------------------------------------------------
/** Configure the timer 0 to generate an interrupt at a 50 ms period rate. */
void TimerInitialize(void);

/** Pause system and running application for the specified periods of 50 ms.
 * @param Periods_Count How many periods of 50 ms to wait.
 */
void TimerWait(unsigned int Periods_Count);

/** Interrupt handler's interface, which is called directly by the kernel. */
void TimerInterruptHandler(void);

#endif