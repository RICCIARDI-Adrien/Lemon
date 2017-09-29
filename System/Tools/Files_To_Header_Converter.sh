#!/bin/sh

# Go to the Lemon tree root to ease path calculation (this script is assumed to be run by the System makefile from Lemon/System directory)
cd ..

OUTPUT_FILE=System/Includes/Installer_Embedded_Files_Data.h

# Create the file header
echo "/** @file Installer_Embedded_Files_Data.h" > $OUTPUT_FILE
echo " * Contain all files to embed into the image converted to C arrays." >> $OUTPUT_FILE
echo " * @warning This file has been automatically generated, do not edit." >> $OUTPUT_FILE
echo " */" >> $OUTPUT_FILE
echo "#ifndef H_INSTALLER_EMBEDDED_FILES_DATA_H" >> $OUTPUT_FILE
echo "#define H_INSTALLER_EMBEDDED_FILES_DATA_H" >> $OUTPUT_FILE
echo "" >> $OUTPUT_FILE
echo "#include <Configuration.h> // To have CONFIGURATION_FILE_NAME_LENGTH constant" >> $OUTPUT_FILE
echo "" >> $OUTPUT_FILE
echo "//-------------------------------------------------------------------------------------------------------------------------------" >> $OUTPUT_FILE
echo "// Types" >> $OUTPUT_FILE
echo "//-------------------------------------------------------------------------------------------------------------------------------" >> $OUTPUT_FILE
echo "/** Represent an embedded file. */" >> $OUTPUT_FILE
echo "typedef struct" >> $OUTPUT_FILE
echo "{" >> $OUTPUT_FILE
echo "	char String_Name[CONFIGURATION_FILE_NAME_LENGTH + 1];" >> $OUTPUT_FILE
echo "	unsigned int Size_Bytes;" >> $OUTPUT_FILE
echo "	unsigned char *Pointer_Data;" >> $OUTPUT_FILE
echo "} TEmbeddedFile;" >> $OUTPUT_FILE
echo "" >> $OUTPUT_FILE
echo "//-------------------------------------------------------------------------------------------------------------------------------" >> $OUTPUT_FILE
echo "// Variables" >> $OUTPUT_FILE
echo "//-------------------------------------------------------------------------------------------------------------------------------" >> $OUTPUT_FILE

# Append the MBR and the kernel to the files to embed into the installer list
Files_List="System/Objects/System_MBR.bin System/Objects/System_Kernel.bin $1"

# Dump all files data to specific arrays
File_Index=0
for File in $Files_List
do
	# Get file size
	Size=$(ls -l $File | awk '{print $5}')
	echo "Converting file '$File' ($Size bytes)..."
	
	# Declare the file data array
	echo -n "unsigned char File_$File_Index" >> $OUTPUT_FILE # Writing must be done in two times to make the shell successfully parse the File_Index variable
	echo "_Data[] =" >> $OUTPUT_FILE
	echo "{" >> $OUTPUT_FILE
	echo -n "	" >> $OUTPUT_FILE # Add a tabulation at the beginning of the array data
	
	# Convert the file content to hexadecimal
	hexdump -v -e '/1 "0x%02X, "' $File >> $OUTPUT_FILE # -v allows to keep all zeros, /1 tells to read file one byte at a time
	
	# Terminate the array
	echo "" >> $OUTPUT_FILE
	echo "};" >> $OUTPUT_FILE
	echo "" >> $OUTPUT_FILE
	
	# Must use this incrementation form to allow building with Debian dash
	File_Index=$((File_Index+1))
done

# Declare the Embedded_Files variable
echo "TEmbeddedFile Embedded_Files[] =" >> $OUTPUT_FILE
echo "{" >> $OUTPUT_FILE

# Force MBR and kernel file names because the real file names are too long
Files_List="MBR Kernel $1"

# Associate a file name to a file data
File_Index=0
for File in $Files_List
do
	# Keep only the file name
	File_Name=$(basename $File)
	
	# Multiple lines are needed to create an array entry because the File_Index variable is located in the middle of strings
	echo -n "	{ " >> $OUTPUT_FILE
	echo -n "\"$File_Name\", sizeof(File_$File_Index" >> $OUTPUT_FILE
	echo -n "_Data), File_$File_Index" >> $OUTPUT_FILE
	echo "_Data }," >> $OUTPUT_FILE
	
	File_Index=$((File_Index+1))
done

# Terminate the Embedded_Files variable
echo "};" >> $OUTPUT_FILE

# Terminate the file
echo "#endif" >> $OUTPUT_FILE
