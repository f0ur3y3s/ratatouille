#pragma once

#ifndef REQUEST_H
#define REQUEST_H

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

typedef enum opcode_t
{
    STATUS   = 0x01,
    FILE_IO  = 0x02,
    EXEC     = 0x03,
    INTERACT = 0x0F,
} opcode_t;

typedef enum stat_flags_t
{
    STAT_GEN    = 0x01,
    STAT_UPTIME = 0x02,
} stat_flags_t;

typedef enum fio_flags_t
{
    FIO_LIST     = 0x01,
    FIO_UPLOAD   = 0x02,
    FIO_DOWNLOAD = 0x03,
    FIO_DELETE   = 0x04,
} fio_flags_t;

typedef enum exec_flags_t
{
    EXEC_RUN  = 0x01,
    EXEC_KILL = 0x02,
} exec_flags_t;

#pragma pack(push, 1)

typedef struct request_t
{
    UINT8 opcode;
    UINT8 flag;
    UINT16 reserved;
    UINT32 packet_len;
};

#pragma pack(pop)

#endif
/*** EOF ***/