/** @file Libraries_Menu_Display.c
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int LibrariesMenuDisplay(TLibrariesMenu *Pointer_Menu)
{
	int i, Selected_Number;
	
	// Display the menu until a valid choice has been done
	while (1)
	{
		LibrariesScreenClear();
		
		// Display the title only if it is shorter than the screen width
		LibrariesScreenSetFontColor(LIBRARIES_MENU_TITLE_COLOR);
		LibrariesScreenWriteCenteredString(Pointer_Menu->String_Title);
		LibrariesScreenWriteString("\n\n");
		
		// Display each item
		for (i = 0; (Pointer_Menu->String_Items[i] != NULL) && (i < LIBRARIES_MENU_MAXIMUM_ITEMS_COUNT); i++)
		{
			LibrariesScreenWriteString("    ");
			
			// Display the item number
			LibrariesScreenSetFontColor(LIBRARIES_MENU_ITEM_NUMBER_COLOR);
			LibrariesScreenWriteInteger(i + 1);
			LibrariesScreenWriteString(". ");
			
			// Display the item text
			LibrariesScreenSetFontColor(LIBRARIES_MENU_ITEM_TEXT_COLOR);
			LibrariesScreenWriteString(Pointer_Menu->String_Items[i]);
			LibrariesScreenWriteCharacter('\n');
		}
		
		// Display the user prompt on the screen bottom
		LibrariesScreenSetFontColor(LIBRARIES_SCREEN_COLOR_BLUE);
		LibrariesScreenSetCursorPosition(LIBRARIES_SCREEN_ROWS_COUNT - 1, 0);
		LibrariesScreenWriteString(Pointer_Menu->String_User_Prompt);
		
		// Wait for the user input
		Selected_Number = (int) LibrariesKeyboardReadUnsignedInteger();
		if ((Selected_Number > 0) && (Selected_Number <= i))
		{
			LibrariesScreenClear();
			return Selected_Number;
		}
	}
}