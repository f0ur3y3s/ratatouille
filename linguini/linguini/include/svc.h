#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <winsvc.h>
#include <strsafe.h>
#include "debug.h"

#define SVCNAME   L"linguini"
#define INSTALL   L"install"
#define UNINSTALL L"uninstall"
#define CLI       L"cli"

BOOL svc_install(VOID);
BOOL svc_uninstall(VOID);
BOOL svc_report_status (LPSERVICE_STATUS        p_svc_status,
                        SERVICE_STATUS_HANDLE * ph_svc_status,
                        DWORD                   dwCurrentState,
                        DWORD                   dwWin32ExitCode,
                        DWORD                   dwWaitHint);
BOOL svc_report_event (LPWSTR svc_name, LPWSTR lp_func, LPWSTR lp_msg);


