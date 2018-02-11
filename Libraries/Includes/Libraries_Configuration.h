/** @file Libraries_Configuration.h
 * Read from and write to a specific userspace file Libraries configuration data.
 * @author Adrien RICCIARDI
 */
#ifndef H_LIBRARIES_CONFIGURATION_H
#define H_LIBRARIES_CONFIGURATION_H

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
/** The configuration file name. */
#define LIBRARIES_CONFIGURATION_FILE_NAME "System.cfg"
/** The maximum length in characters of a value string. */
#define LIBRARIES_CONFIGURATION_FILE_MAXIMUM_VALUE_SIZE 128

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Read a configuration value from the configuration file.
 * @param String_Key The value key (the string that identifies the value).
 * @param String_Value On output, contain the requested value. This buffer must be large enough to hold the whole value.
 * @return 0 if the value was successfully retrieved,
 * @return 1 if the configuration file was not found,
 * @return 2 if the provided key is empty,
 * @return 3 if a read error occurred,
 * @return 4 if the key was not found.
 */
int LibrariesConfigurationReadValue(char *String_Key, char *String_Value);

#endif
