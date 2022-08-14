/*
 * strings.h
 */

#pragma once

#include "stddef.h"

void bzero(void *dest, size_t bytes);
void bcopy(const void *src, void *dest, size_t bytes);
