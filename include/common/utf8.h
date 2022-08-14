/*
 * utf8.h
 */

#include "stddef.h"
#include "stdint.h"

char *utf8_encode(int c, char *buffer);
int utf8_decode(const char *s, char **next);
size_t utf8_strlen(const char *s);
