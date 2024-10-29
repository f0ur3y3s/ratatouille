#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>

#pragma comment(lib, "ws2_32.lib")

VOID  log_debug (const char * fmt, ...);
DWORD log_last_error (BOOL is_wsa);
