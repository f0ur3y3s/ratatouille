#include "svc.h"

BOOL svc_install (VOID)
{
    BOOL  retval                  = FALSE;
    WCHAR unquoted_path[MAX_PATH] = { 0 };

    if (!GetModuleFileNameW(NULL, (LPWSTR)&unquoted_path, MAX_PATH))
    {
        log_debug("Cannot install service: ");
        log_last_error(FALSE);
        goto EXIT;
    }

    WCHAR path[MAX_PATH] = { 0 };
    StringCbPrintf(path, MAX_PATH, TEXT("\"%s\""), unquoted_path);

    log_debug("Unquoted module filename: %ls\n", unquoted_path);
    log_debug("Quoted module filename: %ls\n", path);

    SC_HANDLE sch_manager = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);

    if (NULL == sch_manager)
    {
        log_debug("OpenSCManager failed: ");
        log_last_error(FALSE);
        goto EXIT;
    }

    SC_HANDLE sch_service
        = CreateServiceW(sch_manager,               // SCM database
                         SVCNAME,                   // name of service
                         SVCNAME,                   // service name to display
                         SERVICE_ALL_ACCESS,        // desired access
                         SERVICE_WIN32_OWN_PROCESS, // service type
                         // SERVICE_DEMAND_START,      // start type
                         SERVICE_AUTO_START,   // start type
                         SERVICE_ERROR_NORMAL, // error control type
                         path,                 // path to service's binary
                         NULL,                 // no load ordering group
                         NULL,                 // no tag identifier
                         NULL,                 // no dependencies
                         NULL,                 // LocalSystem account
                         NULL);                // no password

    if (NULL == sch_service)
    {
        log_debug("CreateService failed: ");
        log_last_error(FALSE);
        goto CLOSE_MANAGER_EXIT;
    }

    log_debug("Service installed successfully\n");
    retval = TRUE;
    CloseServiceHandle(sch_service);

CLOSE_MANAGER_EXIT:
    CloseServiceHandle(sch_manager);

EXIT:
    return retval;
}

BOOL svc_uninstall (VOID)
{
    BOOL      retval = FALSE;
    SC_HANDLE sch_manager
        = OpenSCManager(NULL,                   // local computer
                        NULL,                   // ServicesActive database
                        SC_MANAGER_ALL_ACCESS); // full access rights

    if (NULL == sch_manager)
    {
        log_debug("OpenSCManager failed: ");
        log_last_error(FALSE);
        goto EXIT;
    }

    SC_HANDLE sch_service = OpenService(sch_manager, // SCM database
                                        SVCNAME,     // name of service
                                        DELETE);     // need delete access

    if (NULL == sch_service)
    {
        log_debug("OpenService failed: ");
        log_last_error(FALSE);
        goto CLOSE_MANAGER_EXIT;
    }

    if (!DeleteService(sch_service))
    {
        log_debug("DeleteService failed: ");
        log_last_error(FALSE);
        goto CLOSE_SVC_EXIT;
    }

    log_debug("Service deleted successfully\n");
    retval = TRUE;

CLOSE_SVC_EXIT:
    CloseServiceHandle(sch_service);
CLOSE_MANAGER_EXIT:
    CloseServiceHandle(sch_manager);

EXIT:
    return retval;
}

BOOL svc_report_status (LPSERVICE_STATUS        p_svc_status,
                        SERVICE_STATUS_HANDLE * ph_svc_status,
                        DWORD                   dwCurrentState,
                        DWORD                   dwWin32ExitCode,
                        DWORD                   dwWaitHint)
{
    DWORD dwCheckPoint = 1;
    BOOL  retval       = FALSE;

    if ((NULL == p_svc_status) || (NULL == ph_svc_status))
    {
        log_debug("Invalid parameter(s)\n");
        goto EXIT;
    }

    p_svc_status->dwCurrentState  = dwCurrentState;
    p_svc_status->dwWin32ExitCode = dwWin32ExitCode;
    p_svc_status->dwWaitHint      = dwWaitHint;

    if (dwCurrentState == SERVICE_START_PENDING)
    {
        p_svc_status->dwControlsAccepted = 0;
    }
    else
    {
        p_svc_status->dwControlsAccepted = SERVICE_ACCEPT_STOP;
    }

    if ((dwCurrentState == SERVICE_RUNNING)
        || (dwCurrentState == SERVICE_STOPPED))
    {
        p_svc_status->dwCheckPoint = 0;
    }
    else
    {
        p_svc_status->dwCheckPoint = dwCheckPoint++;
    }

    retval = SetServiceStatus(*ph_svc_status, p_svc_status);

    if (retval)
    {
        log_debug("SetServiceStatus successful\n");
    }
    else
    {
        log_debug("SetServiceStatus failed: ");
        log_last_error(FALSE);
    }

EXIT:
    return retval;
}

BOOL svc_report_event (LPWSTR svc_name, LPWSTR lp_func, LPWSTR lp_msg)
{
    BOOL retval = FALSE;

    if ((NULL == svc_name) || (NULL == lp_func))
    {
        log_debug("Invalid parameter(s)\n");
        goto EXIT;
    }

    HANDLE h_event_src      = { 0 };
    LPWSTR lp_strings[2]    = { 0 };
    WCHAR  buffer[MAX_PATH] = { 0 };

    h_event_src = RegisterEventSourceW(NULL, svc_name);

    if (NULL != h_event_src)
    {
        if (lp_msg)
        {
            StringCchPrintfW(buffer, MAX_PATH, L"%ls: %ls", lp_func, lp_msg);
        }
        else
        {
            StringCchPrintfW(
                buffer, MAX_PATH, L"%ls: %d", lp_func, GetLastError());
        }

        lp_strings[0] = svc_name;
        lp_strings[1] = buffer;

        ReportEventW(h_event_src,               // event log handle
                     EVENTLOG_INFORMATION_TYPE, // event type
                     0,                         // event category
                     1337,                         // event identifier
                     NULL,                      // no security identifier
                     2,                         // size of lpszStrings array
                     0,                         // no binary data
                     lp_strings,                // array of strings
                     NULL);                     // no binary data

        DeregisterEventSource(h_event_src);
        retval = TRUE;
    }
    else
    {
        log_debug("RegisterEventSource failed: ");
        log_last_error(FALSE);
    }

EXIT:
    return retval;
}
