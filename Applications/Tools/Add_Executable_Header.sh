#!/bin/sh

INPUT_FILE=$1
OUTPUT_FILE=$2

# Add executable magic number
echo -ne "\x12\x34\x56\x78" > $OUTPUT_FILE

# Concatenate the remaining part of the binary
cat $INPUT_FILE >> $OUTPUT_FILE