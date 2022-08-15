/*
 * utf8.h
 */

#include "stddef.h"
#include "stdint.h"

#define REPLACEMENT_CHARACTER   0xfffd

char *utf8_encode(int c, char *buffer);
void ucs16_to_utf8(char *utf8_buffer, const uint16_t *ucs16_string, size_t utf8_buffer_size, size_t ucs16_string_length);
int utf8_decode(const char *s, char **next);
size_t utf8_strlen(const char *s);
