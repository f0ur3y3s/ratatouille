#pragma once

#ifndef BUFFER_H
#define BUFFER_H

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include "debug.h"

#define MIN_BUFFER_SIZE 16

typedef struct buffer_t
{
    ULONG  offset;
    ULONG  buffer_len;
    LPBYTE lp_data;
} buffer_t;

/**
 * @brief
 *
 * @param p_out_buffer
 * @return BOOL
 */
BOOL buffer_init (buffer_t ** pp_out_buffer, ULONG buffer_len);

    /**
 * @brief
 *
 * @param pp_buffer
 * @return BOOL
 */
BOOL buffer_destroy (buffer_t ** pp_buffer);

/**
 * @brief
 *
 * @param p_buffer
 * @param lp_data
 * @param data_len
 * @return BOOL
 */
BOOL buffer_append (buffer_t * p_buffer, LPBYTE lp_data, ULONG data_len);

/**
 * @brief
 *
 * @param p_buffer
 * @param new_size
 * @return BOOL
 */
BOOL buffer_resize (buffer_t * p_buffer, ULONG new_size);

#endif

/*** EOF ***/