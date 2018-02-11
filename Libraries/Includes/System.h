/** @file System.h
 * System-specific functions that do not belong to any other categories.
 * @author Adrien RICCIARDI
 */
#ifndef H_SYSTEM_H
#define H_SYSTEM_H

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Request a system service.
 * @param Request_Code The service identifier code.
 * @param Integer_1 First integer parameter if needed.
 * @param Integer_2 Second integer parameter if needed.
 * @param Pointer_1 First pointer parameter if needed.
 * @param Pointer_2 Second pointer parameter if needed.
 * @return The service-specific return value.
 * @see System_Calls.h for all services description.
 */
int LibrariesSystemCall(TSystemCall Request_Code, int Integer_1, int Integer_2, void *Pointer_1, void *Pointer_2);

/** Abort the current program execution and return to system. */
void LibrariesSystemExitProgram(void);

#endif
