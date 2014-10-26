/** @file Driver_Timer.c
 * @see Driver_Timer.h for description.
 * @author Adrien RICCIARDI
 */
#include <Architecture.h>
#include <Drivers/Driver_Timer.h>
#include <Hardware_Functions.h>

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** Timer configuration port. */
#define TIMER_PORT_CONFIGURATION 0x43
/** Timer data port. */
#define TIMER_PORT_DATA 0x40

/** Frequency divider value to achieve a 50 ms interrupt rate period. */
#define TIMER_PERIOD_50_MS 59660

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
static volatile unsigned int Timer_Wait_Counter;
static volatile int Timer_Is_Wait_Enabled = 0;

//-------------------------------------------------------------------------------------------------
// Public variable
//-------------------------------------------------------------------------------------------------
volatile unsigned int Timer_Counter = 0;

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void TimerInitialize(void)
{
	// Configure timer 0 for mode 3 and 16-bit operations
	outb(TIMER_PORT_CONFIGURATION, 0x36);
	ArchitectureIODelay();
	
	// Send period
	// Send least significant byte
	outb(TIMER_PORT_DATA, (unsigned char) TIMER_PERIOD_50_MS);
	ArchitectureIODelay();
	// Send most significant byte
	outb(TIMER_PORT_DATA, TIMER_PERIOD_50_MS >> 8);
	ArchitectureIODelay();
}

void TimerWait(unsigned int Periods_Count)
{
	unsigned int Current_Counter_Value;
	
	if (Periods_Count > 0)
	{
		// Interrupts are disabled when entering the interrupt vector
		asm("sti");
		
		// Wait for the next period to start
		Current_Counter_Value = Timer_Counter;
		while (Timer_Counter == Current_Counter_Value);
		
		// Enable sleeping counter
		Timer_Wait_Counter = Periods_Count;
		Timer_Is_Wait_Enabled = 1;
		
		while (Timer_Is_Wait_Enabled);
	}
}

void TimerInterruptHandler(void)
{
	Timer_Counter++;
	
	// Handle sleeping remaining time
	if (Timer_Is_Wait_Enabled)
	{
		Timer_Wait_Counter--;
		if (Timer_Wait_Counter == 0) Timer_Is_Wait_Enabled = 0;
	}
}