/*
 * mmio.h
 */

#pragma once

#include "../common/stddef.h"
#include "../common/stdint.h"

void mmio_write(uint32_t reg, uint32_t data);
void mmio_write_out(uint32_t reg, volatile void *buffer, size_t words);
uint32_t mmio_read(uint32_t reg);
void mmio_read_in(uint32_t reg, volatile void *buffer, size_t words);
