/** @file RTC_Get_Time.c
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void LibrariesRTCGetTime(TLibrariesRTCTime *Pointer_Time)
{
	LibrariesSystemCall(SYSTEM_CALL_RTC_GET_TIME, 0, 0, Pointer_Time, NULL);
}
