/** @file Driver_RTC.c
 * See Driver_RTC.h for description.
 * @author Adrien RICCIARDI
 */
#include <Drivers/Driver_RTC.h>
#include <Hardware_Functions.h>

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** Seconds register address. */
#define RTC_REGISTER_ADDRESS_SECONDS 0
/** Minutes register address. */
#define RTC_REGISTER_ADDRESS_MINUTES 2
/** Hours register address. */
#define RTC_REGISTER_ADDRESS_HOURS 4
/** Status A register address. */
#define RTC_REGISTER_ADDRESS_STATUS_A 0x0A

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Read a RTC register content.
 * @param Address The register address.
 * @return The read value.
 */
static unsigned char RTCReadRegister(unsigned char Address)
{
	// Set register address
	outb(0x70, Address & 0x7F); // Clear "NMI disable" bit to keep NMI enabled
	// Retrieve register content
	return inb(0x71);
}

/** Convert a Binary Coded Digital representation of a number to raw binary.
 * @param BCD_Value The BCD number to convert.
 * @return The corresponding binary value.
 */
static unsigned char RTCConvertBCDToBinary(unsigned char BCD_Value)
{
	unsigned char Tens_Nibble, Units_Nibble;
	
	Tens_Nibble = (BCD_Value >> 4) & 0x0F;
	Units_Nibble = BCD_Value & 0x0F;
	
	return (Tens_Nibble * 10) + Units_Nibble;
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void RTCGetTime(TRTCTime *Pointer_Time)
{
	TRTCTime Time_2;
	
	// Make sure that read values are good (because they can be altered in the middle of reading by a time update) by reading two times the value and comparing them, if they are the same no update occurred
	do
	{
		// Wait for "update" flag to become clear, telling that time and date are not being currently updated
		while (RTCReadRegister(RTC_REGISTER_ADDRESS_STATUS_A) & 0x80);
		
		// Read all needed values once
		Pointer_Time->Seconds = RTCReadRegister(RTC_REGISTER_ADDRESS_SECONDS);
		Pointer_Time->Minutes = RTCReadRegister(RTC_REGISTER_ADDRESS_MINUTES);
		Pointer_Time->Hours = RTCReadRegister(RTC_REGISTER_ADDRESS_HOURS);
		
		// Make sure "update" flag is still clear
		while (RTCReadRegister(RTC_REGISTER_ADDRESS_STATUS_A) & 0x80);
		
		// Read all needed values a second time, in case values were updated during previous read
		Time_2.Seconds = RTCReadRegister(RTC_REGISTER_ADDRESS_SECONDS);
		Time_2.Minutes = RTCReadRegister(RTC_REGISTER_ADDRESS_MINUTES);
		Time_2.Hours = RTCReadRegister(RTC_REGISTER_ADDRESS_HOURS);
	} while ((Pointer_Time->Seconds != Time_2.Seconds) || (Pointer_Time->Minutes != Time_2.Minutes) || (Pointer_Time->Hours != Time_2.Hours));
	
	// Convert read data to binary
	Pointer_Time->Seconds = RTCConvertBCDToBinary(Pointer_Time->Seconds);
	Pointer_Time->Minutes = RTCConvertBCDToBinary(Pointer_Time->Minutes);
	Pointer_Time->Hours = RTCConvertBCDToBinary(Pointer_Time->Hours);
}
