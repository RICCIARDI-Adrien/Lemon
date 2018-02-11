/** @file System_Menu_Display.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int SystemMenuDisplay(TSystemMenu *Pointer_Menu)
{
	int i, Selected_Number;
	
	// Display the menu until a valid choice has been done
	while (1)
	{
		SystemScreenClear();
		
		// Display the title only if it is shorter than the screen width
		SystemScreenSetFontColor(LIBRARIES_MENU_TITLE_COLOR);
		SystemScreenWriteCenteredString(Pointer_Menu->String_Title);
		SystemScreenWriteString("\n\n");
		
		// Display each item
		for (i = 0; (Pointer_Menu->String_Items[i] != NULL) && (i < LIBRARIES_MENU_MAXIMUM_ITEMS_COUNT); i++)
		{
			SystemScreenWriteString("    ");
			
			// Display the item number
			SystemScreenSetFontColor(LIBRARIES_MENU_ITEM_NUMBER_COLOR);
			SystemScreenWriteInteger(i + 1);
			SystemScreenWriteString(". ");
			
			// Display the item text
			SystemScreenSetFontColor(LIBRARIES_MENU_ITEM_TEXT_COLOR);
			SystemScreenWriteString(Pointer_Menu->String_Items[i]);
			SystemScreenWriteCharacter('\n');
		}
		
		// Display the user prompt on the screen bottom
		SystemScreenSetFontColor(LIBRARIES_SCREEN_COLOR_BLUE);
		SystemScreenSetCursorPosition(LIBRARIES_SCREEN_ROWS_COUNT - 1, 0);
		SystemScreenWriteString(Pointer_Menu->String_User_Prompt);
		
		// Wait for the user input
		Selected_Number = (int) SystemKeyboardReadUnsignedInteger();
		if ((Selected_Number > 0) && (Selected_Number <= i))
		{
			SystemScreenClear();
			return Selected_Number;
		}
	}
}