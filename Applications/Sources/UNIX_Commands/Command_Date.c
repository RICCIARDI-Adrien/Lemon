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
/** Localized day name. */
static char *Pointer_Command_Date_String_Day_Name[] =
{
	STRING_COMMAND_DATE_DAY_SUNDAY,
	STRING_COMMAND_DATE_DAY_MONDAY,
	STRING_COMMAND_DATE_DAY_TUESDAY,
	STRING_COMMAND_DATE_DAY_WEDNESDAY,
	STRING_COMMAND_DATE_DAY_THURSDAY,
	STRING_COMMAND_DATE_DAY_FRIDAY,
	STRING_COMMAND_DATE_DAY_SATURDAY
};

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
// Private functions
//-------------------------------------------------------------------------------------------------
/** Determine day of week using Sakamoto's formula.
 * @param Date The date to retrieve day of week from.
 * @return Day of week in range [0;6]. 0 is Sunday, 6 is Saturday.
 */
static int CommandDateComputeDayOfWeek(TLibrariesRTCDate *Pointer_Date)
{
	static unsigned char Month_Codes[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
	int Year, Day_Of_Week;
	
	Year = Pointer_Date->Year; // Year value can be overwritten, so preserve caller value
	if (Pointer_Date->Month < 3) Year--;
	Day_Of_Week = (Year + (Year / 4) - (Year / 100) + (Year / 400) + Month_Codes[Pointer_Date->Month - 1] + Pointer_Date->Day) % 7;
	
	return Day_Of_Week;
}

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
int CommandMainDate(int argc, char __attribute__((unused)) *argv[])
{
	TLibrariesRTCDate Date;
	TLibrariesRTCTime Time;
	int Day_Of_Week;
	
	// Display date and time if no argument is provided
	if (argc == 1)
	{
		// Display current date
		LibrariesRTCGetDate(&Date);
		Day_Of_Week = CommandDateComputeDayOfWeek(&Date);
		#ifdef CONFIGURATION_GLOBAL_LANGUAGE_FRENCH
			// Day of week
			LibrariesScreenWriteString(Pointer_Command_Date_String_Day_Name[Day_Of_Week]);
			LibrariesScreenWriteCharacter(' ');
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
