#!/bin/sh

INPUT_FILE=$1
OUTPUT_FILE=$2

# Add executable magic number (octal values and printf are used to be compatible with default Debian /bin/sh, which is the prehistorical dash)
printf "\022\064\126\170" > $OUTPUT_FILE

# Concatenate the remaining part of the binary
cat $INPUT_FILE >> $OUTPUT_FILE