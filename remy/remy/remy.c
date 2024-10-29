#include "remy.h"

INT main(INT argc, CHAR** argv)
{
    log_debug("Debugging started\n");

    WSADATA wsa_data = { 0 };

    if (!net_init_wsa(&wsa_data))
    {
        log_debug("Failed to initialize WSA\n");
        goto EXIT;
    }

    // Your application logic here

    if (!net_cleanup_wsa())
    {
        log_debug("Failed to cleanup WSA\n");
        goto EXIT;
    }

EXIT:
    return 0;
}