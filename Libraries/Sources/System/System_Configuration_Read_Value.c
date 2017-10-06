/** @file System_Configuration_Read_Value.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int SystemConfigurationReadValue(char *String_Key, char *String_Value)
{
	unsigned int File_ID, Read_Bytes_Count;
	char Character;
	int i, Is_Key_Found, Return_Value = 4;
	
	// Make sure the provided key is not empty
	if (String_Key[0] == 0) return 2;
	
	// Try to open the configuration file
	if (SystemFileOpen(SYSTEM_CONFIGURATION_FILE_NAME, SYSTEM_FILE_OPENING_MODE_READ, &File_ID) != 0) return 1;

	// Browse the file until the key is found
	do
	{
		// Search for the required key
		i = 0;
		Is_Key_Found = 1;
		while (String_Key[i] != 0)
		{
			// Read a single character
			if (SystemFileRead(File_ID, &Character, 1, &Read_Bytes_Count) != 0) return 3;
			if (Read_Bytes_Count == 0) goto Exit; // The file terminated before the key was found
			
			// Match the key in the same time
			if (String_Key[i] != Character)
			{
				// Go to the next key (read the file up to find a new line character or to reach the file end)
				do
				{
					if (SystemFileRead(File_ID, &Character, 1, &Read_Bytes_Count) != 0) return 3;
					if (Read_Bytes_Count == 0) goto Exit; // The file terminated before the key was found
				} while (Character != '\n');
				
				Is_Key_Found = 0;
				break;
			}
			
			i++;
		}
		
		// Is the next character a '=' ?
		if (Is_Key_Found)
		{
			if (SystemFileRead(File_ID, &Character, 1, &Read_Bytes_Count) != 0) return 3; // Read a single character
			if (Read_Bytes_Count == 0) goto Exit; // The file terminated before the key was found
			if (Character != '=') Is_Key_Found = 0; // This is not the right key, it is a longer key that has the same beginning
		}
	} while (!Is_Key_Found);
	
	// The key was found, get its value
	for (i = 0; i < SYSTEM_CONFIGURATION_FILE_MAXIMUM_VALUE_SIZE; i++)
	{
		// Read a single character
		if (SystemFileRead(File_ID, &Character, 1, &Read_Bytes_Count) != 0) return 3;
		if (Read_Bytes_Count == 0) break; // End of file is reached
		
		// Is the end of the value reached ?
		if (Character == '\n') break;
		
		// Append the character to the string
		String_Value[i] = Character;
	}
	
	// Terminate the string
	String_Value[i] = 0;
	Return_Value = 0;
	
Exit:
	FileClose(File_ID);
	return Return_Value;
}