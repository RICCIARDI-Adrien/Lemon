/** @file Command_Uptime.c
 * Tell since how much time the system is running.
 * @author Adrien RICCIARDI
 */
#include <System.h>
#include "Commands.h"
#include "Strings.h"

//-------------------------------------------------------------------------------------------------
// Command entry point
//-------------------------------------------------------------------------------------------------
int CommandMainUptime(int argc, char __attribute__((unused)) *argv[])
{
	unsigned int Seconds, Days, Hours, Minutes;
	
	// Check parameters
	if (argc != 1)
	{
		SystemScreenWriteString(STRING_COMMAND_UPTIME_USAGE);
		return -1;
	}
	
	// Get the running time in seconds
	Seconds = SystemTimerGetValue() / 1000;
	
	// Convert it to a human-readable value
	// Days count
	Days = Seconds / (24 * 60 * 60);
	Seconds -= Days * (24 * 60 * 60); // Adjust remaining seconds
	// Hours count
	Hours = Seconds / (60 * 60);
	Seconds -= Hours * (60 * 60); // Adjust remaining seconds
	// Minutes count
	Minutes = Seconds / 60;
	Seconds -= Minutes * 60; // Adjust remaining seconds
	
	// Display up time
	SystemScreenWriteString(STRING_COMMAND_UPTIME_BOOTED_TIME_1);
	SystemScreenWriteUnsignedInteger(Days);
	SystemScreenWriteString(STRING_COMMAND_UPTIME_BOOTED_TIME_2);
	SystemScreenWriteUnsignedInteger(Hours);
	SystemScreenWriteString(STRING_COMMAND_UPTIME_BOOTED_TIME_3);
	SystemScreenWriteUnsignedInteger(Minutes);
	SystemScreenWriteString(STRING_COMMAND_UPTIME_BOOTED_TIME_4);
	SystemScreenWriteUnsignedInteger(Seconds);
	SystemScreenWriteString(STRING_COMMAND_UPTIME_BOOTED_TIME_5);
	
	return 0;
}