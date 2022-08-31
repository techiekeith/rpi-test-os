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
#include "../../include/common/string.h"
#include "../../include/kernel/board.h"
#include "../../include/kernel/interrupt.h"
#include "../../include/kernel/io.h"
#include "../../include/kernel/mailbox.h"
#include "../../include/kernel/system_timer.h"
#include "../../include/uspi/uspios.h"

int usb_power_on();

DEBUG_INIT("uspi_compat");

typedef struct {
    uint64_t when;
    TKernelTimerHandler *pHandler; // ignore entry if NULL
    void *pParam;
    void *pContext;
} timer_queue_t;

// 20 is the maximum in the reference code, so should be good enough here
#define MAX_USPI_TIMERS 20
#define NEVER 0xffffffffffffffffULL
timer_queue_t uspi_timers[MAX_USPI_TIMERS];
uint64_t next_expected_timer_event;

void MsDelay (unsigned nMilliSeconds)
{
    system_timer_busy_wait(1000 * nMilliSeconds);
}

void usDelay (unsigned nMicroSeconds)
{
    system_timer_busy_wait(nMicroSeconds);
}

static uint64_t handle_timers()
{
    uint64_t now = get_system_timer_counter();
    uint64_t next = NEVER;
    for (int i = 0; i < MAX_USPI_TIMERS; i++)
    {
        if (uspi_timers[i].pHandler != NULL)
        {
            if (now > uspi_timers[i].when)
            {
                TKernelTimerHandler *pHandler = uspi_timers[i].pHandler;
//                debug_printf("Timer handle %d triggered.\r\n", i + 1);
                uspi_timers[i].pHandler = NULL;
                (*pHandler)(i + 1, uspi_timers[i].pParam, uspi_timers[i].pContext);
            }
            else if (next > uspi_timers[i].when)
            {
                next = uspi_timers[i].when;
            }
        }
    }
    return next;
}

static void usb_driver_timer_interrupt_handler(void *unused)
{
    (void) unused;

    system_timer_3_irq_clearer(NULL);
    uint64_t now, next;
    do {
        next = handle_timers();
        now = get_system_timer_counter();
    } while (now > next);
    next_expected_timer_event = next;
    if (next != NEVER)
    {
        system_timer_set_3_abs(next_expected_timer_event);
    }
}

static void usb_driver_timer_interrupt_init()
{
    memset(uspi_timers, 0, MAX_USPI_TIMERS * sizeof(timer_queue_t));
    next_expected_timer_event = NEVER;
    register_irq_handler(SYSTEM_TIMER_3, NULL, NULL, usb_driver_timer_interrupt_handler, NULL);
}

unsigned StartKernelTimer (unsigned nDelay, TKernelTimerHandler *pHandler, void *pParam, void *pContext)
{
//    DEBUG_START("StartKernelTimer");

    for (int i = 0; i < MAX_USPI_TIMERS; i++)
    {
        if (uspi_timers[i].pHandler == NULL)
        {
            uint64_t when = get_system_timer_counter() + nDelay * 1000000 / HZ;
            uspi_timers[i].when = when;
            uspi_timers[i].pHandler = pHandler;
            uspi_timers[i].pParam = pParam;
            uspi_timers[i].pContext = pContext;
            if (next_expected_timer_event > when)
            {
                next_expected_timer_event = when;
                system_timer_set_3_abs(when);
            }
//            debug_printf("Registered new timer with handle %d (now=%lld, when=%lld).\r\n",
//                         i + 1, now, uspi_timers[i].when);
//            DEBUG_END();
            return i + 1;
        }
    }

//    debug_printf("Cannot register new timer, no timers available.\r\n");
//
//    DEBUG_END();
    return 0;
}

void CancelKernelTimer (unsigned hTimer)
{
    DEBUG_START("CancelKernelTimer");

    if (hTimer < 1 || hTimer > MAX_USPI_TIMERS)
    {
        debug_printf("Invalid timer handle %d.\r\n", hTimer);
        DEBUG_END();
        return;
    }
    if (uspi_timers[hTimer - 1].pHandler == NULL)
    {
        debug_printf("Timer handle %d is inactive.\r\n", hTimer);
        DEBUG_END();
        return;
    }
    uspi_timers[hTimer - 1].pHandler = NULL;
    debug_printf("Timer handle %d cancelled.\r\n", hTimer);

    DEBUG_END();
}

void ConnectInterrupt (unsigned nIRQ, TInterruptHandler *pHandler, void *pParam)
{
    DEBUG_START("ConnectInterrupt");

    if (nIRQ == USB_CONTROLLER)
    {
        register_irq_handler(nIRQ, NULL, NULL, pHandler, pParam);
        // Register the interrupt handler for the timers as well.
        usb_driver_timer_interrupt_init();
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
