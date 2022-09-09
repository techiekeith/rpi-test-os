/*
 * unicode_block.h
 */

#pragma once

typedef enum {
    SAA505X_CHARSET = 0,
    ACORN_CHARSET,
    COMMODORE_PET_CHARSET,
    COMMODORE_64_CHARSET,
    SINCLAIR_CHARSET,
    LAST_CHARSET = SINCLAIR_CHARSET
} charset_t;

typedef struct {
    int start;
    int end;
    int *block;
} unicode_block_idx_t;
