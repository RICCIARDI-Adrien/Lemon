/** @file Command_Date.c
 * A simplified version of the "date" command.
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>
#include "Commands.h"
#include "Strings.h"

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
int CommandMainDate(int argc, char __attribute__((unused)) *argv[])
{
	TLibrariesRTCTime Time;
	
	// Display date and time if no argument is provided
	if (argc == 1)
	{
		// Display current date
		// TODO
		
		// Display current time
		LibrariesRTCGetTime(&Time);
		// Hours
		LibrariesScreenWriteInteger(Time.Hours);
		LibrariesScreenWriteCharacter(':');
		// Minutes
		if (Time.Minutes < 10) LibrariesScreenWriteCharacter('0'); // Add leading zero if needed
		LibrariesScreenWriteInteger(Time.Minutes);
		LibrariesScreenWriteCharacter(':');
		// Seconds
		if (Time.Seconds < 10) LibrariesScreenWriteCharacter('0'); // Add leading zero if needed
		LibrariesScreenWriteInteger(Time.Seconds);
		LibrariesScreenWriteCharacter('\n');
	}
	
	return 0;
}
