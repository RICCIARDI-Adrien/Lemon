/** @file Driver_Timer.h
 * Driver for hardware timer 0.
 * @author Adrien RICCIARDI
 */
#ifndef H_DRIVER_TIMER_H
#define H_DRIVER_TIMER_H

//-------------------------------------------------------------------------------------------------
// Variables
//-------------------------------------------------------------------------------------------------
/** The raw timer value. This counter is incremented on each timer interrupt. It does roll-over when overflow. */
extern volatile unsigned int Timer_Counter;

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Configure the timer 0 to generate an interrupt at a 1 ms period rate. */
void TimerInitialize(void);

/** Pause system and running application for the specified periods of 1 ms.
 * @param Periods_Count How many periods of 1 ms to wait.
 */
void TimerWait(unsigned int Periods_Count);

/** Interrupt handler's interface, which is called directly by the kernel. */
void TimerInterruptHandler(void);

#endif