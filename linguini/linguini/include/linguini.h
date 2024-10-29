#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include "debug.h"
#include "svc.h"

//#define WIDEN(x) L## #x

typedef enum dbg_modes_t
{
    DBG_MODE_ERR = -1,
    DBG_MODE_NONE = 0,
    DBG_MODE_CLI,
    DBG_MODE_SERVICE
} dbg_modes_t;