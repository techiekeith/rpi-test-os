/*
 * stdarg.h
 */

#pragma once

typedef char *va_list;

#define _VA_ALIGN                   4
#define _SLOTSIZEOF(t)              ((sizeof(t) + _VA_ALIGN - 1) & ~(_VA_ALIGN - 1))
#define _APALIGN(t, ap)             (((va_list)0 - (ap)) & (__alignof(t) - 1))
#define _ADDRESSOF(v)               (&(v))

#define va_start(ap, v)             ((void)(ap = (va_list)_ADDRESSOF(v) + _SLOTSIZEOF(v)))
#define va_arg(ap, t)               (*(t*)((ap += _SLOTSIZEOF(t) + _APALIGN(t,ap)) - _SLOTSIZEOF(t)))
#define va_end(ap)                  ((void)(ap = (va_list)0))
