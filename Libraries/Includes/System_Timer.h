/** @file System_Timer.h
 * Provide time-based utilities.
 * @author Adrien RICCIARDI
 */
#ifndef H_LIBRARIES_TIMER_H
#define H_LIBRARIES_TIMER_H

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Get the timer current value. This timer is incremented by one every millisecond and starts from zero on system boot.
 * @return The timer value (the unit is 1 ms).
 */
unsigned int SystemTimerGetValue(void);

/** Wait for the specified number of milliseconds.
 * @param Periods_Count How many milliseconds to wait.
 */
void SystemTimerWait(unsigned int Milliseconds_Count);

#endif
