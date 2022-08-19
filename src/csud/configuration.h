/******************************************************************************
*	configuration.h
*	 by Alex Chadwick
*
*	A light weight implementation of the USB protocol stack fit for a simple
*	driver.
*
*	configuration.h contains definitions for all optional components
*	The makefile defines three main categories of definitions:
*		CONFIG: Whether or not this is a DEBUG driver
*		TARGET: The target system
*		TYPE:	What sort of driver to compile (e.g. standalone)
*****************************************************************************/

#include "../../include/kernel/peripheral.h"

#define ARM
#define ARM_V6
#define ENDIAN_LITTLE
#define BROADCOM_2835
#define HCD_DESIGNWARE_20
#define HCD_DESIGNWARE_BASE ((void*)(PERIPHERAL_BASE + HOST_CONTROLLER_OFFSET))

#define DEBUG 1
#define TARGET_RPI
#define TYPE_LOWLEVEL
#define MEM_INTERNAL_MANAGER
#define MEM_NO_RESERVE
