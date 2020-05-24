/** @file Command_Date.c
 * A simplified version of the "date" command.
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>
#include "Commands.h"
#include "Strings.h"

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** Localized month name. */
static char *Pointer_Command_Date_String_Month_Name[] =
{
	STRING_COMMAND_DATE_MONTH_JANUARY,
	STRING_COMMAND_DATE_MONTH_FEBRUARY,
	STRING_COMMAND_DATE_MONTH_MARCH,
	STRING_COMMAND_DATE_MONTH_APRIL,
	STRING_COMMAND_DATE_MONTH_MAY,
	STRING_COMMAND_DATE_MONTH_JUNE,
	STRING_COMMAND_DATE_MONTH_JULY,
	STRING_COMMAND_DATE_MONTH_AUGUST,
	STRING_COMMAND_DATE_MONTH_SEPTEMBER,
	STRING_COMMAND_DATE_MONTH_OCTOBER,
	STRING_COMMAND_DATE_MONTH_NOVEMBER,
	STRING_COMMAND_DATE_MONTH_DECEMBER
};

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
int CommandMainDate(int argc, char __attribute__((unused)) *argv[])
{
	TLibrariesRTCDate Date;
	TLibrariesRTCTime Time;
	
	// Display date and time if no argument is provided
	if (argc == 1)
	{
		// Display current date
		LibrariesRTCGetDate(&Date);
		#ifdef CONFIGURATION_GLOBAL_LANGUAGE_FRENCH
			// TODO day of week
			// Day
			LibrariesScreenWriteInteger(Date.Day);
			LibrariesScreenWriteCharacter(' ');
			// Month
			LibrariesScreenWriteString(Pointer_Command_Date_String_Month_Name[Date.Month - 1]); // Array starts from zero
			LibrariesScreenWriteCharacter(' ');
			// Year
			LibrariesScreenWriteInteger(Date.Year);
			LibrariesScreenWriteCharacter(' ');
		#endif
		
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
