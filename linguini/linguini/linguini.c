#include "linguini.h"

static VOID WINAPI svc_main (DWORD num_args, LPWSTR * lp_args);
static VOID WINAPI svc_ctrl_handler (DWORD dw_ctrl);
static VOID        svc_init (DWORD num_args, LPWSTR * lp_args);
static INT         parse_args (DWORD num_args, LPWSTR * lp_args);

SERVICE_STATUS        g_svc_status      = { 0 };
SERVICE_STATUS_HANDLE gh_svc_status     = { 0 };
HANDLE                gh_svc_stop_event = NULL;

INT wmain (INT argc, WCHAR ** argv)
{
    INT retval = 0;

    retval = parse_args(argc, argv);

    switch (retval)
    {
        case DBG_MODE_CLI: // Run in CLI mode
            svc_init(argc, argv);
            goto EXIT;
        case DBG_MODE_SERVICE: // Run as a service
            break;
        case DBG_MODE_NONE: // Do nothing
            goto EXIT;
        default:
            goto EXIT;
    }

    SERVICE_TABLE_ENTRYW dispatch_table[] = {
        { SVCNAME, svc_main },
    };

    StartServiceCtrlDispatcherW(dispatch_table);

EXIT:
    return retval;
}

static VOID WINAPI svc_main (DWORD num_args, LPWSTR * lp_args)
{
    gh_svc_status = RegisterServiceCtrlHandlerW(SVCNAME, svc_ctrl_handler);

    if (!gh_svc_status)
    {
        svc_report_event(L"RegisterServiceCtrlHandler", __FUNCTIONW__, NULL);
        return;
    }

    g_svc_status.dwServiceType             = SERVICE_WIN32_OWN_PROCESS;
    g_svc_status.dwServiceSpecificExitCode = 0;

    svc_report_status(
        &g_svc_status, &gh_svc_status, SERVICE_START_PENDING, NO_ERROR, 3000);
    svc_init(num_args, lp_args);
}

static VOID WINAPI svc_ctrl_handler (DWORD dw_ctrl)
{
    switch (dw_ctrl)
    {
        case SERVICE_CONTROL_STOP:
            svc_report_status(&g_svc_status,
                              &gh_svc_status,
                              SERVICE_STOP_PENDING,
                              NO_ERROR,
                              0);

            SetEvent(gh_svc_stop_event);
            svc_report_status(&g_svc_status,
                              &gh_svc_status,
                              g_svc_status.dwCurrentState,
                              NO_ERROR,
                              0);
            break;

        case SERVICE_CONTROL_INTERROGATE:
            break;

        default:
            break;
    }

    return;
}

static VOID svc_init (DWORD num_args, LPWSTR * lp_args)
{
    gh_svc_stop_event = CreateEventW(NULL, TRUE, FALSE, NULL);

    if (!gh_svc_stop_event)
    {
        svc_report_event(SVCNAME, __FUNCTIONW__, L"CreateEvent failed");
        return;
    }

    svc_report_status(
        &g_svc_status, &gh_svc_status, SERVICE_RUNNING, NO_ERROR, 0);
    svc_report_event(SVCNAME, __FUNCTIONW__, L"I'm a chef!");
    log_debug("Im a chef!");

    for (;;)
    {
        WaitForSingleObject(gh_svc_stop_event, INFINITE);
        svc_report_event(SVCNAME, __FUNCTIONW__, L"Service stopped");
        svc_report_status(
            &g_svc_status, &gh_svc_status, SERVICE_STOPPED, NO_ERROR, 0);
        return;
    }
    svc_report_status(
        &g_svc_status, &gh_svc_status, SERVICE_STOPPED, NO_ERROR, 0);
    return;
}

#ifdef _DEBUG
static INT parse_args (DWORD num_args, LPWSTR * lp_args)
{
    for (DWORD idx = 0; idx < num_args; idx++)
    {
        log_debug("Argument %d: %ls\n", idx, lp_args[idx]);
    }

    INT retval = DBG_MODE_ERR;

    if (2 == num_args)
    {
        if (wcsncmp(lp_args[1], INSTALL, wcslen(INSTALL)) == 0)
        {
            if (svc_install())
            {
                retval = DBG_MODE_NONE;
            }
        }
        else if (wcsncmp(lp_args[1], UNINSTALL, wcslen(UNINSTALL)) == 0)
        {
            if (svc_uninstall())
            {
                retval = DBG_MODE_NONE;
            }
        }
        else if (wcsncmp(lp_args[1], CLI, wcslen(CLI)) == 0)
        {
            log_debug("Running in CLI mode\n");
            retval = DBG_MODE_CLI;
        }
        else
        {
            log_debug("Invalid argument: %s\n", lp_args[1]);
            goto EXIT;
        }
    }
    else
    {
        retval = DBG_MODE_SERVICE;
    }

EXIT:
    return retval;
}
#else
static INT parse_args (DWORD num_args, LPWSTR * lp_args)
{
    return DBG_MODE_SERVICE;
}
#endif
