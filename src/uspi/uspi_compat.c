//
// uspi_compat.c (was uspibind.cpp)
//
// USPi - An USB driver for Raspberry Pi written in C
// Copyright (C) 2014  R. Stange <rsta2@o2online.de>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
#include "../../include/common/stdarg.h"
#include "../../include/common/stdbool.h"
#include "../../include/common/string.h"
#include "../../include/kernel/board.h"
#include "../../include/kernel/interrupt.h"
#include "../../include/kernel/io.h"
#include "../../include/kernel/mailbox.h"
#include "../../include/kernel/system_timer.h"
#include "../../include/uspi/uspios.h"

int usb_power_on();

DEBUG_INIT("uspi_compat");

void MsDelay (unsigned nMilliSeconds)
{
    system_timer_busy_wait(1000 * nMilliSeconds);
}

void usDelay (unsigned nMicroSeconds)
{
    system_timer_busy_wait(nMicroSeconds);
}

static void handle_uspi_interrupt(int handle, void **args)
{
    TKernelTimerHandler *pHandler = (TKernelTimerHandler *) args[0];
    void *pParam = (void *) args[1];
    void *pContext = (void *) args[2];
    pHandler(handle, pParam, pContext);
}

unsigned StartKernelTimer (unsigned nDelay, TKernelTimerHandler *pHandler, void *pParam, void *pContext)
{
    void *args[3] = { pHandler, pParam, pContext };
    return register_interval_handler("uspi", nDelay, false, handle_uspi_interrupt, 3, args);
}

void CancelKernelTimer (unsigned hTimer)
{
    deregister_interval_handler(hTimer);
}

void ConnectInterrupt (unsigned nIRQ, TInterruptHandler *pHandler, void *pParam)
{
    DEBUG_START("ConnectInterrupt");

    if (nIRQ == USB_CONTROLLER)
    {
        register_irq_handler(nIRQ, NULL, NULL, pHandler, pParam);
    }
    else
    {
        debug_printf("This method may only be used for USB interrupts.\r\n");
    }

    DEBUG_END();
}

int SetPowerStateOn (unsigned nDeviceId)
{
    DEBUG_START("SetPowerStateOn");
    int success;

    if (nDeviceId == DEVICE_USB_HOST_CONTROLLER)
    {
        success = !usb_power_on();
    }
    else
    {
        debug_printf("This method may only be used to power on the HCD.\r\n");
        success = 0;
    }

    DEBUG_END();
    return success;
}

int GetMACAddress (unsigned char Buffer[6])
{
    memcpy (Buffer, &mac_address, 6);
    return 1;
}

void LogWrite (const char *pSource, unsigned Severity, const char *pMessage, ...)
{
    (void) Severity; // TODO use this

    DEBUG_START(pSource);
    va_list var;
    va_start (var, pMessage);

    debug_vprintf(pMessage, var);
    debug_writeline();

    va_end (var);
    DEBUG_END();
}

#ifndef NDEBUG

void debug_stacktrace();
void uspi_assertion_failed (const char *pExpr, const char *pFile, unsigned nLine)
{
    DEBUG_START("uspi_assertion_failed");
    debug_stacktrace();
    debug_printf(pExpr, pFile, nLine);
    debug_writeline();
    DEBUG_END();
}

void DebugHexdump (const void *pBuffer, unsigned nBufLen, const char *pSource)
{
    (void) pSource; // TODO use this
    debug_hexdump((void *)pBuffer, (void *)(pBuffer + nBufLen));
}

#endif
