#pragma once

#define WIN32_LEAN_AND_MEAN

#include <WinSock2.h>
#include <stdbool.h>
#include "debug.h"

BOOL net_init_wsa (LPWSADATA p_wsadata);
BOOL net_cleanup_wsa(VOID);