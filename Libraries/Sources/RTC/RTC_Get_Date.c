/** @file RTC_Get_Date.c
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void LibrariesRTCGetDate(TLibrariesRTCDate *Pointer_Date)
{
	LibrariesSystemCall(SYSTEM_CALL_RTC_GET_DATE, 0, 0, Pointer_Date, NULL);
}
