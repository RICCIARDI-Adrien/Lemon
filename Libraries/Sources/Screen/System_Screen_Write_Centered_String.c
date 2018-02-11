/** @file System_Screen_Write_Centered_String.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void SystemScreenWriteCenteredString(char *String)
{
	int Length, Column, Current_Cursor_Row, Current_Cursor_Column;
	
	// Find the string length
	for (Length = 0; String[Length] != 0; Length++);
	
	// Compute the centered horizontal location
	if (Length >= SYSTEM_SCREEN_COLUMNS_COUNT) Column = 0; // The string is too long to be centered
	else
	{
		Column = (SYSTEM_SCREEN_COLUMNS_COUNT - Length) / 2;
	
		// Set the cursor at the right location
		LibrariesSystemCall(SYSTEM_CALL_SCREEN_GET_CURSOR_POSITION, 0, 0, &Current_Cursor_Row, &Current_Cursor_Column);
		LibrariesSystemCall(SYSTEM_CALL_SCREEN_SET_CURSOR_POSITION, Current_Cursor_Row, Column, NULL, NULL);
	}
	
	// Display the string
	LibrariesSystemCall(SYSTEM_CALL_SCREEN_WRITE_STRING, 0, 0, String, NULL);
}
