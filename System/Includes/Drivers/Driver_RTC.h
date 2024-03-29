/** @file Driver_RTC.h
 * Retrieve or configure RTC date and time.
 * @author Adrien RICCIARDI
 */
#ifndef H_DRIVER_RTC_H
#define H_DRIVER_RTC_H

//-------------------------------------------------------------------------------------------------
// Types
//-------------------------------------------------------------------------------------------------
/** Date values. They all are binary. */
typedef struct __attribute__((packed)) // This structure is duplicated in user space, so make really sure that no padding can be done
{
	int Day; //!< From 1 to 31, depending on month.
	int Month; //!< 1 = January, 12 = December.
	int Year; //!< Full year value, like "2019".
} TRTCDate;

/** Time values. They all are binary. */
typedef struct __attribute__((packed)) // This structure is duplicated in user space, so make really sure that no padding can be done
{
	int Hours; //!< From 0 to 23.
	int Minutes; //!< From 0 to 59.
	int Seconds; //!< From 0 to 59.
} TRTCTime;

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Retrieve date from RTC chip.
 * @param Pointer_Date On output, contain the read date.
 */
void RTCGetDate(TRTCDate *Pointer_Date);

/** Retrieve time from RTC chip.
 * @param Pointer_Time On output, contain the read time.
 */
void RTCGetTime(TRTCTime *Pointer_Time);

#endif
