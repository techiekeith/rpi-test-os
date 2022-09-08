/*
 * unicode_block.h
 */

#pragma once

typedef struct {
    int start;
    int end;
    int *block;
} unicode_block_idx_t;
