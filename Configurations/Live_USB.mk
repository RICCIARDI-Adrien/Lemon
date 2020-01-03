# Start by including templates so needed variables can be easily overwritten
include $(LEMON_PATH_CONFIGURATIONS)/Template_Installer_Files_List_Network_Enabled.mk

GLOBAL_PROCESSOR_TYPE = pentium

SYSTEM_HARD_DISK_DRIVER = ram

SYSTEM_FILE_SYSTEM_MAXIMUM_FILES_LIST_ENTRIES = 512
# Set a 256MB RAM file system, so available RAM size must be greater than 256MB
SYSTEM_FILE_SYSTEM_MAXIMUM_BLOCKS_LIST_ENTRIES = 65536

SYSTEM_ETHERNET_CONTROLLER_DRIVER = 82540em

SYSTEM_RAM_SIZE = 1024
