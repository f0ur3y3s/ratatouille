#include "debug.h"

#ifdef _DEBUG
VOID log_debug (const char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
}

DWORD log_last_error (BOOL is_wsa)
{
    wchar_t * p_wmessage = NULL;
    DWORD     error_code = 0;

    if (is_wsa)
    {
        error_code = WSAGetLastError();
    }
    else
    {
        error_code = GetLastError();
    }

    DWORD format_retval = FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM
            | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        error_code,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPWSTR)&p_wmessage,
        0,
        NULL);

    if (0 == format_retval)
    {
        log_debug("FormatMessage failed\n");
        goto EXIT;
    }

    log_debug("[-] %S", p_wmessage);
    LocalFree(p_wmessage);

EXIT:
    return (format_retval);
}

#else
static VOID log_debug (const char * fmt, ...)
{
}

static DWORD log_last_error (BOOL is_wsa)
{
}
#endif