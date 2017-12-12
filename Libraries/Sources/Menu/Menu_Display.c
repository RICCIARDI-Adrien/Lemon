/** @file Menu_Display.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int MenuDisplay(TMenu *Pointer_Menu)
{
	int i, Selected_Number;
	
	// Display the menu until a valid choice has been done
	while (1)
	{
		SystemScreenClear();
		
		// Display the title only if it is shorter than the screen width
		SystemScreenSetFontColor(MENU_TITLE_COLOR);
		SystemScreenWriteCenteredString(Pointer_Menu->String_Title);
		ScreenWriteString("\n\n");
		
		// Display each item
		for (i = 0; (Pointer_Menu->String_Items[i] != NULL) && (i < MENU_MAXIMUM_ITEMS_COUNT); i++)
		{
			ScreenWriteString("    ");
			
			// Display the item number
			SystemScreenSetFontColor(MENU_ITEM_NUMBER_COLOR);
			ScreenWriteInteger(i + 1);
			ScreenWriteString(". ");
			
			// Display the item text
			SystemScreenSetFontColor(MENU_ITEM_TEXT_COLOR);
			ScreenWriteString(Pointer_Menu->String_Items[i]);
			SystemScreenWriteCharacter('\n');
		}
		
		// Display the user prompt on the screen bottom
		SystemScreenSetFontColor(SYSTEM_SCREEN_COLOR_BLUE);
		SystemScreenSetCursorPosition(SYSTEM_SCREEN_ROWS_COUNT - 1, 0);
		ScreenWriteString(Pointer_Menu->String_User_Prompt);
		
		// Wait for the user input
		Selected_Number = (int) SystemKeyboardReadUnsignedInteger();
		if ((Selected_Number > 0) && (Selected_Number <= i))
		{
			SystemScreenClear();
			return Selected_Number;
		}
	}
}