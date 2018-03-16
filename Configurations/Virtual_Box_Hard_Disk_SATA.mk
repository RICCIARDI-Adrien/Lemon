# Start by including templates so needed variables can be easily overwritten
include $(LEMON_PATH_CONFIGURATIONS)/Template_Installer_Files_List_Network_Enabled.mk
include $(LEMON_PATH_CONFIGURATIONS)/Template_File_System_Properties.mk

GLOBAL_PROCESSOR_TYPE = pentium

SYSTEM_HARD_DISK_DRIVER = sata
SYSTEM_HARD_DISK_LOGICAL_BLOCK_ADDRESSING_MODE = 48

SYSTEM_ETHERNET_CONTROLLER_DRIVER = 82540em

SYSTEM_RAM_SIZE = 16
