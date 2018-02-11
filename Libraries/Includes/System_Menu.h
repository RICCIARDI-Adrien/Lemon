/** @file System_Menu.h
 * Display text-based menus.
 * @author Adrien RICCIARDI
 */
#ifndef H_LIBRARIES_MENU_H
#define H_LIBRARIES_MENU_H

#include <System.h> // To have the colors definition

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
/** The menu title color. */
#define LIBRARIES_MENU_TITLE_COLOR LIBRARIES_SCREEN_COLOR_GREEN
/** A menu item number color. */
#define LIBRARIES_MENU_ITEM_NUMBER_COLOR LIBRARIES_SCREEN_COLOR_GRAY
/** A menu item text color. */
#define LIBRARIES_MENU_ITEM_TEXT_COLOR LIBRARIES_SCREEN_COLOR_BLUE

/** The maximum amount of menu items. */
#define LIBRARIES_MENU_MAXIMUM_ITEMS_COUNT 20

//-------------------------------------------------------------------------------------------------
// Types
//-------------------------------------------------------------------------------------------------
/** A menu description. */
typedef struct
{
	char *String_Title; //!< The menu title.
	char *String_User_Prompt; //!< The prompt displayed to the user.
	char *String_Items[LIBRARIES_MENU_MAXIMUM_ITEMS_COUNT]; //!< All menu items, the array must be NULL-terminated if not all items are filled.
} TSystemMenu;

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Display a menu with a centered title and all items preceded by a number and wait for the user to select an item.
 * @param Pointer_Menu The menu to display.
 * @return The selected item number (starting from 1 for the first item).
 */
int SystemMenuDisplay(TSystemMenu *Pointer_Menu);

#endif
