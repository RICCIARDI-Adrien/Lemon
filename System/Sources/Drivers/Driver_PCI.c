/** @file Driver_PCI.c
 * @see Driver_PCI.h for description.
 * @author Adrien RICCIARDI
 */
#include <Configuration.h>
#include <Debug.h>
#include <Drivers/Driver_PCI.h>
#include <Hardware_Functions.h>

#if CONFIGURATION_PCI_IS_DRIVER_ENABLED == 1

//-------------------------------------------------------------------------------------------------
// Private constants and macros
//-------------------------------------------------------------------------------------------------
/** Maximum buses count. */
#define PCI_MAXIMUM_BUS_COUNT 256
/** Maximum units count in a bus. */
#define PCI_MAXIMUM_UNIT_COUNT 32
/** Maximum functions count in a unit. */
#define PCI_MAXIMUM_FUNCTION_COUNT 8
/** Maximum registers count for a function. */
#define PCI_MAXIMUM_REGISTER_COUNT 64

/** "CONFIG_ADDRESS" I/O port. */
#define PCI_PORT_CONFIGURATION_ADDRESS 0x0CF8
/** "CONFIG_DATA" I/O port. */
#define PCI_PORT_CONFIGURATION_DATA 0x0CFC

/** A non-existing PCI device will return this vendor ID value. */
#define PCI_VENDOR_ID_NO_DEVICE 0xFFFF

/** Create a CONFIG_ADDRESS 32-bit value.
 * @param Is_Configuration_Data_Enabled Set to 0 if CONFIG_DATA register is not activated, set to 1 if CONFIG_DATA register is activated.
 * @param Bus_ID The bus ID, in range [0 ; PCI_MAXIMUM_BUS_COUNT - 1].
 * @param Unit_ID The unit ID, in range [0 ; PCI_MAXIMUM_UNIT_COUNT - 1].
 * @param Function_ID The function ID, in range [0 ; PCI_MAXIMUM_FUNCTION_COUNT - 1].
 * @param Register_ID The register within a unit's function, in range [0 ; PCI_MAXIMUM_REGISTER_COUNT - 1].
 * @param Type The transfer type.
 * @return the corresponding CONFIG_ADDRESS register layout.
 */
#define PCI_FILL_CONFIGURATION_ADDRESS(Is_Configuration_Data_Enabled, Bus_ID, Unit_ID, Function_ID, Register_ID, Type) (((Is_Configuration_Data_Enabled & 0x00000001) << 31) | (Bus_ID << 16) | (Unit_ID << 11) | (Function_ID << 8) | (Register_ID << 2) | Type)

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int PCIFindDeviceFromIDs(unsigned short Vendor_ID, unsigned short Device_ID, TPCIDeviceID *Pointer_Device_ID)
{
	int Bus_ID, Unit_ID, Function_ID;
	unsigned int Searched_Device_Mask, Current_Device_Mask;
	
	// Create the mask to compare with the first 4 bytes of each unit Configuration Space Header
	Searched_Device_Mask = (Device_ID << 16) | Vendor_ID;
	
	// Probe the bus and find all PCI units
	for (Bus_ID = 0; Bus_ID < PCI_MAXIMUM_BUS_COUNT; Bus_ID++)
	{
		for (Unit_ID = 0; Unit_ID < PCI_MAXIMUM_UNIT_COUNT; Unit_ID++)
		{
			for (Function_ID = 0; Function_ID < PCI_MAXIMUM_FUNCTION_COUNT; Function_ID++)
			{
				// Probe this unit slot
				outd(PCI_PORT_CONFIGURATION_ADDRESS, PCI_FILL_CONFIGURATION_ADDRESS(1, Bus_ID, Unit_ID, Function_ID, 0, 0));
				
				// Is that the desired device ?
				Current_Device_Mask = ind(PCI_PORT_CONFIGURATION_DATA);
				if (Current_Device_Mask == Searched_Device_Mask)
				{
					Pointer_Device_ID->Bus_ID = Bus_ID;
					Pointer_Device_ID->Unit_ID = Unit_ID;
					Pointer_Device_ID->Function_ID = Function_ID;
					
					return 0;
				}
			}
		}
	}
	
	return 1;
}

int PCIFindDeviceFromClass(unsigned char Class_Code_Base, unsigned char Class_Code_Sub_Class, TPCIDeviceID *Pointer_Device_ID)
{
	int Bus_ID, Unit_ID, Function_ID;
	unsigned int Searched_Class_Mask, Current_Class_Mask, Vendor_ID;
	
	// Create the mask to compare with the class fields of each unit Configuration Space Header
	Searched_Class_Mask = (Class_Code_Base << 8) | Class_Code_Sub_Class;
	
	// Probe the bus and find all PCI units
	for (Bus_ID = 0; Bus_ID < PCI_MAXIMUM_BUS_COUNT; Bus_ID++)
	{
		for (Unit_ID = 0; Unit_ID < PCI_MAXIMUM_UNIT_COUNT; Unit_ID++)
		{
			for (Function_ID = 0; Function_ID < PCI_MAXIMUM_FUNCTION_COUNT; Function_ID++)
			{
				// Probe this unit slot
				outd(PCI_PORT_CONFIGURATION_ADDRESS, PCI_FILL_CONFIGURATION_ADDRESS(1, Bus_ID, Unit_ID, Function_ID, 0, 0));
				
				// Is there a unit responding ?
				Vendor_ID = ind(PCI_PORT_CONFIGURATION_DATA) & 0x0000FFFF;
				if (Vendor_ID != PCI_VENDOR_ID_NO_DEVICE)
				{
					// Load the class fields
					outd(PCI_PORT_CONFIGURATION_ADDRESS, PCI_FILL_CONFIGURATION_ADDRESS(1, Bus_ID, Unit_ID, Function_ID, 2, 0)); // The fields are located in the 32-bit double-word register 2
					Current_Class_Mask = ind(PCI_PORT_CONFIGURATION_DATA) >> 16;
					
					// Do the class and subclass match ?
					if (Current_Class_Mask == Searched_Class_Mask)
					{
						Pointer_Device_ID->Bus_ID = Bus_ID;
						Pointer_Device_ID->Unit_ID = Unit_ID;
						Pointer_Device_ID->Function_ID = Function_ID;
						
						return 0;
					}
				}
			}
		}
	}
	
	return 1;
}

int PCIGetConfigurationSpaceHeader(TPCIDeviceID *Pointer_Device_ID, TPCIConfigurationSpaceHeader *Pointer_Configuration_Space_Header)
{
	unsigned int i, *Pointer_Configuration_Space_Header_Double_Word = (unsigned int *) Pointer_Configuration_Space_Header;
	
	// Make sure the device is present
	// Load the vendor and device IDs register
	outd(PCI_PORT_CONFIGURATION_ADDRESS, PCI_FILL_CONFIGURATION_ADDRESS(1, Pointer_Device_ID->Bus_ID, Pointer_Device_ID->Unit_ID, Pointer_Device_ID->Function_ID, 0, 0));
	Pointer_Configuration_Space_Header_Double_Word[0] = ind(PCI_PORT_CONFIGURATION_DATA);
	
	// Is the device responding ?
	if (Pointer_Configuration_Space_Header->Vendor_ID == PCI_VENDOR_ID_NO_DEVICE) return 1; // No device in this slot
	
	// Load the remaining data
	for (i = 1; i < sizeof(TPCIConfigurationSpaceHeader) / sizeof(unsigned int); i++)
	{
		outd(PCI_PORT_CONFIGURATION_ADDRESS, PCI_FILL_CONFIGURATION_ADDRESS(1, Pointer_Device_ID->Bus_ID, Pointer_Device_ID->Unit_ID, Pointer_Device_ID->Function_ID, i, 0));
		Pointer_Configuration_Space_Header_Double_Word[i] = ind(PCI_PORT_CONFIGURATION_DATA);
	}
	
	return 0;
}

#if CONFIGURATION_IS_DEBUG_ENABLED == 1
	void PCIDebugShowDevices(void)
	{
		TPCIDeviceID Device_ID = {0};
		TPCIConfigurationSpaceHeader Configuration_Space_Header;
		
		// Probe the bus and find all PCI units
		for (Device_ID.Bus_ID = 0; Device_ID.Bus_ID < PCI_MAXIMUM_BUS_COUNT; Device_ID.Bus_ID++)
		{
			for (Device_ID.Unit_ID = 0; Device_ID.Unit_ID < PCI_MAXIMUM_UNIT_COUNT; Device_ID.Unit_ID++)
			{
				for (Device_ID.Function_ID = 0; Device_ID.Function_ID < PCI_MAXIMUM_FUNCTION_COUNT; Device_ID.Function_ID++)
				{
					// Probe this unit slot
					if (PCIGetConfigurationSpaceHeader(&Device_ID, &Configuration_Space_Header) != 0) continue;
					
					// A unit has been found
					DEBUG_DISPLAY_CURRENT_FUNCTION_NAME();
					ScreenWriteString("Found PCI unit : bus = ");
					ScreenWriteString(itoa(Device_ID.Bus_ID));
					ScreenWriteString(", unit = ");
					ScreenWriteString(itoa(Device_ID.Unit_ID));
					ScreenWriteString(", function = ");
					ScreenWriteString(itoa(Device_ID.Function_ID));
					ScreenWriteString("\nvendor ID = 0x");
					DebugWriteHexadecimalInteger(Configuration_Space_Header.Vendor_ID);
					ScreenWriteString(", device ID = 0x");
					DebugWriteHexadecimalInteger(Configuration_Space_Header.Device_ID);
					ScreenWriteString("\nbase class = ");
					ScreenWriteString(itoa(Configuration_Space_Header.Class_Code_Base));
					ScreenWriteString(", sub-class = ");
					ScreenWriteString(itoa(Configuration_Space_Header.Class_Code_Sub_Class));
					ScreenWriteCharacter('\n');
					KeyboardReadCharacter();
				}
			}
		}
	}
#endif

#endif