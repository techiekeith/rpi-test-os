/*
 * strings.c
 */

#include "../../include/common/strings.h"
#include "../../include/common/string.h"

inline void bzero(void *dest, size_t bytes)
{
    memset(dest, 0, bytes);
}

inline void bcopy(const void *src, void *dest, size_t bytes)
{
    memmove(dest, src, bytes);
}
