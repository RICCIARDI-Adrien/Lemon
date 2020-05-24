/** @file Libraries_RTC.h
 * Retrieve or configure RTC date and time.
 * @author Adrien RICCIARDI
 */
#ifndef H_LIBRARIES_RTC_H
#define H_LIBRARIES_RTC_H

//-------------------------------------------------------------------------------------------------
// Types
//-------------------------------------------------------------------------------------------------
/** Date values. They all are binary. */
typedef struct __attribute__((packed)) // This structure is duplicated in kernel space, so make really sure that no padding can be done
{
	int Day; //!< From 1 to 31, depending on month.
	int Month; //!< 1 = January, 12 = December.
	int Year; //!< Full year value, like "2019".
} TLibrariesRTCDate;

/** Time values. They all are binary. */
typedef struct __attribute__((packed)) // This structure is duplicated in kernel space, so make really sure that no padding can be done
{
	int Hours; //!< From 0 to 23.
	int Minutes; //!< From 0 to 59.
	int Seconds; //!< From 0 to 59.
} TLibrariesRTCTime;

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Retrieve date from RTC chip.
 * @param Pointer_Date On output, contain the read date.
 */
void LibrariesRTCGetDate(TLibrariesRTCDate *Pointer_Date);

/** Retrieve time from RTC chip.
 * @param Pointer_Time On output, contain the read time.
 */
void LibrariesRTCGetTime(TLibrariesRTCTime *Pointer_Time);

#endif
