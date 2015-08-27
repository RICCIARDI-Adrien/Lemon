/** @file Buffer.c
 * @see Buffer.h for description.
 * @author Adrien RICCIARDI
 */
#include "Buffer.h"
#include "Configuration.h"

//-------------------------------------------------------------------------------------------------
// Public variables
//-------------------------------------------------------------------------------------------------
char Buffer[CONFIGURATION_BUFFER_MAXIMUM_SIZE];
unsigned int Buffer_Characters_Count = 0;

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int BufferAddCharacter(unsigned int Index, char Character)
{
	unsigned int i;
	
	// Make sure the index is valid
	if (Index >= CONFIGURATION_BUFFER_MAXIMUM_SIZE) return 1;
	// Is there enough room in the buffer ?
	if (Buffer_Characters_Count >= CONFIGURATION_BUFFER_MAXIMUM_SIZE) return 1;
	// Is the new character location not too far from the text end ?
	if (Index > Buffer_Characters_Count) return 1;
	
	// Shift all characters one place to the right if the character is added in the middle of the buffer
	for (i = Buffer_Characters_Count; i != Index; i--) Buffer[i] = Buffer[i - 1];
	
	// Add the character
	Buffer[Index] = Character;
	Buffer_Characters_Count++;
	
	return 0;
}

unsigned int BufferFindLineBeginning(unsigned int Line_Index)
{
	unsigned int Lines_Count = 0, i = 0, Characters_Counter = 0;
	
	// Parse the whole buffer until the line is found
	while ((i < Buffer_Characters_Count) && (Lines_Count < Line_Index))
	{
		// Start counting at the loop beginning to count from 1
		Characters_Counter++;
		
		if ((Buffer[i] == '\n') || (Characters_Counter == CONFIGURATION_SCREEN_COLUMNS_COUNT))
		{
			Lines_Count++;
			Characters_Counter = 0; // Start counting a new line
		}
		
		i++;
	}
	
	return i;
}

unsigned int BufferGetLinesCount(void)
{
	unsigned int Lines_Count = 0, i = 0, Characters_Counter = 0;
	
	// Parse the whole buffer
	while (i < Buffer_Characters_Count)
	{
		// Start counting at the loop beginning to count from 1
		Characters_Counter++;
		
		if ((Buffer[i] == '\n') || (Characters_Counter == CONFIGURATION_SCREEN_COLUMNS_COUNT))
		{
			Lines_Count++;
			Characters_Counter = 0; // Start counting a new line
		}
		
		i++;
	}
	
	// Take into account the last line which can be made of only few characters
	if (Characters_Counter > 0) Lines_Count++;
	
	return Lines_Count;
}

/*char *BufferGetLine(unsigned int Index)
{
	unsigned int Lines_Count = 0, i = 0, Characters_Counter = 0;
	
	// Parse the whole buffer until the line is found
	while ((i < Buffer_Characters_Count) && (Lines_Count < Index))
	{
		// Start counting at the loop beginning to count from 1
		Characters_Counter++;
		
		if ((Buffer[i] == '\n') || (Characters_Counter == CONFIGURATION_SCREEN_COLUMNS_COUNT))
		{
			Lines_Count++;
			Characters_Counter = 0; // Start counting a new line
		}
		
		i++;
	}
	
	// The line was not found, so the specified index is invalid
	if (i >= Buffer_Characters_Count) return NULL;
	return &Buffer[i];
}*/

void BufferDisplayLine(unsigned int Line_Index)
{
	//unsigned int Lines_Count = 0, i = 0, Characters_Counter = 0;
	unsigned int i, Characters_Counter;
	char Character;
	
	// Parse the whole buffer until the line is found
	/*while ((i < Buffer_Characters_Count) && (Lines_Count < Line_Index))
	{
		// Start counting at the loop beginning to count from 1
		Characters_Counter++;
		
		if ((Buffer[i] == '\n') || (Characters_Counter == CONFIGURATION_SCREEN_COLUMNS_COUNT))
		{
			Lines_Count++;
			Characters_Counter = 0; // Start counting a new line
		}
		
		i++;
	}*/
	
	i = BufferFindLineBeginning(Line_Index);
	
	// The line was found, display it
	if (i < Buffer_Characters_Count)
	{
		for (Characters_Counter = 0; Characters_Counter < CONFIGURATION_SCREEN_COLUMNS_COUNT; Characters_Counter++)
		{
			// Display the character
			Character = Buffer[i]; // Cache the character value
			ScreenWriteCharacter(Character);
			i++; // Must be incremented before the test as there may be only one character to display
			
			// Is the end of the line reached ?
			if (Character == '\n') break;
			// Is the end of the buffer reached ?
			if (i >= Buffer_Characters_Count) break;
		}
	}
}

int BufferGetLineLength(unsigned int Line_Index, unsigned int *Pointer_Length)
{
	unsigned int i, Characters_Counter;
	char Character;
	
	i = BufferFindLineBeginning(Line_Index);
	
	// Is the line existing ?
	if (i >= Buffer_Characters_Count) return 1;
	
	// Compute the line length
	for (Characters_Counter = 0; Characters_Counter < CONFIGURATION_SCREEN_COLUMNS_COUNT; Characters_Counter++)
	{
		// Cache the character value
		Character = Buffer[i];

		// Is the end of the line reached ?
		if (Character == '\n') break;
		// Is the end of the buffer reached ?
		if (i >= Buffer_Characters_Count) break;
	}
	*Pointer_Length = Characters_Counter;
	
	return 0;
}