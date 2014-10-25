/** @file Standard_Functions.c
 * @see Standard_Functions.h for description.
 * @author Adrien RICCIARDI
 */
#include <Standard_Functions.h>

//-------------------------------------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------------------------------------
int strcmp(char *String_1, char *String_2)
{	
	while ((*String_1 != 0) || (*String_2 != 0))
	{
		if (*String_1 < *String_2) return -1;
		else if (*String_1 > *String_2) return 1;
		String_1++;
		String_2++;
	}	
	return 0;
}

int strncmp(char *String_1, char *String_2, unsigned int Bytes_Count)
{
	while (((*String_1 != 0) || (*String_2 != 0)) && (Bytes_Count > 0))
	{
		if (*String_1 < *String_2) return -1;
		else if (*String_1 > *String_2) return 1;
		String_1++;
		String_2++;
		Bytes_Count--;
	}	
	return 0;
}

void strcpy(char *String_Destination, char *String_Source)
{
	while (*String_Source != 0)
	{
		*String_Destination = *String_Source;
		String_Destination++;
		String_Source++;
	}
	// Terminate destination string
	*String_Destination = 0;
}

void strncpy(char *String_Destination, char *String_Source, unsigned int Bytes_Count)
{
	while ((*String_Source != 0) && (Bytes_Count > 0))
	{
		*String_Destination = *String_Source;
		String_Destination++;
		String_Source++;
		Bytes_Count--;
	}
	// Terminate destination string
	*String_Destination = 0;
}

void memset(void *Pointer_Buffer, unsigned char Value, unsigned int Bytes_Count)
{
	unsigned char *Pointer_Buffer_Bytes;
	
	Pointer_Buffer_Bytes = Pointer_Buffer;
	
	while (Bytes_Count > 0)
	{
		*Pointer_Buffer_Bytes = Value;
		Pointer_Buffer_Bytes++;
		Bytes_Count--;
	}
}

void memcpy(void *Pointer_Destination_Buffer, void *Pointer_Source_Buffer, unsigned int Bytes_Count)
{
	unsigned char *Pointer_Destination_Buffer_Bytes, *Pointer_Source_Buffer_Bytes;
	
	Pointer_Destination_Buffer_Bytes = Pointer_Destination_Buffer;
	Pointer_Source_Buffer_Bytes = Pointer_Source_Buffer;
	
	while (Bytes_Count > 0)
	{
		*Pointer_Destination_Buffer_Bytes = *Pointer_Source_Buffer_Bytes;
		Pointer_Destination_Buffer_Bytes++;
		Pointer_Source_Buffer_Bytes++;
		Bytes_Count--;
	}	
}

void strcat(char *String_Destination, char *String)
{
	// Go to the end of the destination string
	while (*String_Destination != 0) String_Destination++;
	
	// Add the other string to the end of destination string
	while (*String != 0)
	{
		*String_Destination = *String;
		String_Destination++;
		String++;
	}
	// Terminate the destination string
	*String_Destination = 0;
}

unsigned int strlen(char *String)
{
	unsigned int Length = 0;
	
	while (*String != 0)
	{
		String++;
		Length++;
	}
	return Length;
}

char *itoa(unsigned int Number)
{
	static char String[11]; // A 32-bit integer can't have more than 10 digits
	char Temp_Character;
	unsigned int Integer_Division;
	int i = 0, j = 0;

	// Handle special case 0
	if (Number == 0)
	{
		String[0] = '0';
		String[1] = 0;
		return String;
	}
	
	while (Number > 0)
	{
		Integer_Division = Number / 10;
		// Convert remaining number into a character
		String[i] = (Number - (Integer_Division * 10)) + 48;
		i++;
		// Next digit
		Number = Integer_Division;
	}
	// End string
	String[i] = 0;
	
	// i is pointing on the end of the string
	i--; // Avoid terminating zero

	// Reverse string by inverting beginning and ending characters until all the string is parsed
	while (j < i)
	{
		Temp_Character = String[j];
		String[j] = String[i];
		String[i] = Temp_Character;
		// Next digits
		j++;
		i--;
	}
	return String;
}