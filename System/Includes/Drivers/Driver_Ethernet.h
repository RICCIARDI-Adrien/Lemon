/** @file Driver_Ethernet.h
 * Gather abstracted functions supported by all ethernet controllers.
 * @author Adrien RICCIARDI
 */
#ifndef H_DRIVER_ETHERNET_H
#define H_DRIVER_ETHERNET_H

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Configure the underlying ethernet hardware for reception and transmission.
 * @return 0 if the device was successfully initialized,
 * @return 1 if no ethernet controller was detected,
 * @return 2 if the detected ethernet controller is not handled by the compiled driver (make sure the right driver was selected),
 */
int EthernetInitialize(void);

/** Wait for an ethernet frame to come.
 * @param Pointer_Buffer_Size On output, contain the frame size in bytes.
 * @param Pointer_Buffer On output, contain the frame content. The buffer must be CONFIGURATION_ETHERNET_BUFFER_SIZE bytes large.
 */
void EthernetReceivePacket(unsigned int *Pointer_Buffer_Size, void *Pointer_Buffer);

// TODO
int EthernetSendPacket(unsigned int Buffer_Size, void *Pointer_Buffer);

#endif