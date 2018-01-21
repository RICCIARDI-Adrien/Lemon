/** @file System_Timer.h
 * Provide time-based utilities.
 * @author Adrien RICCIARDI
 */
#ifndef H_SYSTEM_TIMER_H
#define H_SYSTEM_TIMER_H

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Get the timer current value. This timer is incremented by one every millisecond and starts from zero on system boot.
 * @return The timer value (the unit is 1 ms).
 */
unsigned int SystemTimerGetValue(void);

#endif
