#include "net.h"

BOOL net_init_wsa (LPWSADATA p_wsadata)
{
    BOOL retval = false;

    if (NULL == p_wsadata)
    {
        log_debug("p_wsadata is NULL\n");
        goto EXIT;
    }

    if (0 != WSAStartup(MAKEWORD(2, 2), p_wsadata))
    {
        log_debug("WSAStartup failed\n");
        log_last_error(TRUE);
        goto EXIT;
    }

    retval = true;

EXIT:
    return retval;
}

BOOL net_cleanup_wsa(VOID)
{
    BOOL retval = false;

    if (0 != WSACleanup())
    {
        log_debug("WSACleanup failed\n");
        log_last_error(TRUE);
        goto EXIT;
    }

    retval = true;

EXIT:
    return retval;
}