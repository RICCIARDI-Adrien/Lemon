/** @file Driver_Ethernet.h
 * Gather abstracted functions supported by all ethernet controllers.
 * @author Adrien RICCIARDI
 */
#ifndef H_DRIVER_ETHERNET_H
#define H_DRIVER_ETHERNET_H

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** TODO
 * @return 0 if the device was successfully initialized,
 * @return 1 if no ethernet controller was detected,
 * @return 2 if the detected ethernet controller is not handled by the compiled driver (make sure the right driver was selected),
 */
int EthernetInitialize(void);

// TODO
int EthernetReceivePacket(unsigned int Buffer_Size, void *Pointer_Buffer);

// TODO
int EthernetSendPacket(unsigned int Buffer_Size, void *Pointer_Buffer);

#endif