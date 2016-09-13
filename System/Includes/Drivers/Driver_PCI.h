/** @file Driver_PCI.h
 * Generic access to the PCI bus and all its units.
 * @author Adrien RICCIARDI
 */
#ifndef H_DRIVER_PCI_H
#define H_DRIVER_PCI_H

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
/** Intel vendor ID. */
#define PCI_VENDOR_ID_INTEL 0x8086

/** Mass storage class base code. */
#define PCI_CLASS_CODE_BASE_MASS_STORAGE 0x01
/** Network controller class base code. */
#define PCI_CLASS_CODE_BASE_NETWORK_CONTROLLER 0x02

/** Ethernet controller sub-class code. */
#define PCI_CLASS_CODE_SUB_CLASS_ETHERNET 0x00
/** SATA controller sub-class code. */
#define PCI_CLASS_CODE_SUB_CLASS_SATA 0x06

//-------------------------------------------------------------------------------------------------
// Types
//-------------------------------------------------------------------------------------------------
/** A standard PCI Configuration Space Header. */
typedef struct __attribute__((packed))
{
	unsigned short Vendor_ID;
	unsigned short Device_ID;
	unsigned short Command;
	unsigned short Status;
	unsigned char Revision_ID;
	unsigned char Class_Code_Interface;
	unsigned char Class_Code_Sub_Class;
	unsigned char Class_Code_Base;
	unsigned char Cache_Line_Size; //! In 32-bit units.
	unsigned char Latency_Timer;
	unsigned char Header_Type;
	unsigned char Built_In_Self_Test;
	unsigned int Base_Address_Registers[6];
	unsigned int Cardbus_CIS_Pointer;
	unsigned short Subsystem_Vendor_ID;
	unsigned short Subsystem_ID;
	unsigned int Base_Address_Register_Expansion_ROM;
	unsigned char Capabilities_Pointer;
	unsigned char Reserved[7];
	unsigned char Interrupt_Line;
	unsigned char Interrupt_Pin;
	unsigned char Latency_Timer_Minimum_Latency;
	unsigned char Latency_Timer_Maximum_Latency;
} TPCIConfigurationSpaceHeader;

/** All needed informations to access to an existing PCI device. */
typedef struct __attribute__((packed))
{
	int Bus_ID;
	int Unit_ID;
	int Function_ID;
} TPCIDeviceID;

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Search for the first device matching the provided IDs.
 * @param Vendor_ID The device vendor ID.
 * @param Device_ID The device ID.
 * @param Pointer_Device_ID On output, contain the device identification information.
 * @return 0 if the device was found,
 * @return 1 if the device is not present on the bus.
 * @warning Only the first matching device will be returned.
 */
int PCIFindDeviceFromIDs(unsigned short Vendor_ID, unsigned short Device_ID, TPCIDeviceID *Pointer_Device_ID);

/** Search for the first device matching the provided class.
 * @param Class_Code_Base The class base code.
 * @param Class_Code_Sub_Class The sub-class code.
 * @param Pointer_Device_ID On output, contain the device identification information.
 * @return 0 if the device was found,
 * @return 1 if the device is not present on the bus.
 * @warning Only the first matching device will be returned.
 */
int PCIFindDeviceFromClass(unsigned char Class_Code_Base, unsigned char Class_Code_Sub_Class, TPCIDeviceID *Pointer_Device_ID);

/** Get the Configuration Space Header of a specified device.
 * @param Pointer_Device_ID The device to get header from.
 * @param Pointer_Configuration_Space_Header On output, contain the device Configuration Space Header.
 * @return 0 if the header was successfully retrieved,
 * @return 1 if the specified device does not exist.
 */
int PCIGetConfigurationSpaceHeader(TPCIDeviceID *Pointer_Device_ID, TPCIConfigurationSpaceHeader *Pointer_Configuration_Space_Header);

/** Display all system PCI devices on screen.
 * @note This function is available only in debug mode.
 */
void PCIDebugShowDevices(void);

#endif