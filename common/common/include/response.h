#pragma once

#ifndef RESPONSE_H
#define RESPONSE_H

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

typedef enum response_code_t
{
    RESPONSE_OK        = 0x00,
    RESPONSE_ERROR     = 0x01,
    RESPONSE_NOT_FOUND = 0x02,
    RESPONSE_TIMEOUT   = 0x03,
} response_code_t;

#pragma pack(push, 1)

typedef struct response_t
{
    UINT8  response_code;
    UINT32 reserved : 24; // 24-bit reserved field
    UINT32 packet_len;
} response_t;

#pragma pack(pop)

#endif
