#include "buffer.h"

BOOL buffer_init (buffer_t ** pp_out_buffer, ULONG buffer_len)
{
    BOOL retval = FALSE;

    if ((NULL == pp_out_buffer) || (NULL != (*pp_out_buffer))
        || (0 == buffer_len))
    {
        log_debug("buffer init invalid params\n");
        goto EXIT;
    }

    buffer_t * p_buffer
        = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(buffer_t));

    if (NULL == p_buffer)
    {
        log_debug("HeapAlloc failed\n");
        goto EXIT;
    }

    LPVOID lp_heap_data = HeapAlloc(
        GetProcessHeap(), HEAP_ZERO_MEMORY, buffer_len * sizeof(BYTE));

    if (NULL == lp_heap_data)
    {
        log_debug("HeapAlloc failed\n");
        goto CLEANUP_BUFFER_EXIT;
    }

    p_buffer->offset     = 0;
    p_buffer->buffer_len = buffer_len;
    p_buffer->lp_data    = (LPBYTE)lp_heap_data; // Cast to LPBYTE
    *pp_out_buffer       = p_buffer;

    retval = TRUE;

CLEANUP_BUFFER_EXIT:
    if (FALSE == retval)
    {
        HeapFree(GetProcessHeap(), 0, p_buffer);
    }
EXIT:
    return retval;
}

BOOL buffer_destroy (buffer_t ** pp_buffer)
{
    BOOL retval = FALSE;

    if ((NULL == pp_buffer) || (NULL == *pp_buffer))
    {
        log_debug("buffer destroy invalid params\n");
        goto EXIT;
    }

    if (NULL == (*pp_buffer)->lp_data)
    {
        log_debug("buffer destroy data was NULL\n");
        goto EXIT;
    }

    if (!HeapFree(GetProcessHeap(), 0, (*pp_buffer)->lp_data))
    {
        log_debug("HeapFree failed\n");
        goto EXIT;
    }

    if (!HeapFree(GetProcessHeap(), 0, *pp_buffer))
    {
        log_debug("HeapFree failed\n");
        goto EXIT;
    }

    *pp_buffer = NULL;

    retval = TRUE;

EXIT:
    return retval;
}

BOOL buffer_append (buffer_t * p_buffer, LPBYTE lp_data, ULONG data_len)
{
    BOOL retval = FALSE;

    if ((NULL == p_buffer) || (NULL == lp_data) || (0 == data_len))
    {
        log_debug("buffer append invalid params\n");
        goto EXIT;
    }

    if ((p_buffer->offset + data_len) > p_buffer->buffer_len)
    {
        // resize buffer
        ULONGLONG new_size = p_buffer->offset + data_len;

        if (ULONG_MAX < new_size)
        {
            log_debug("buffer append overflow\n");
            goto EXIT;
        }

        if (!buffer_resize(p_buffer, (ULONG)new_size))
        {
            log_debug("buffer append resize failed\n");
            goto EXIT;
        }
    }

    memcpy(p_buffer->lp_data + p_buffer->offset, lp_data, data_len);
    p_buffer->offset += data_len;
    retval = TRUE;

EXIT:
    return retval;
}

BOOL buffer_resize (buffer_t * p_buffer, ULONG new_size)
{
    BOOL retval = FALSE;

    if ((NULL == p_buffer) || (0 == new_size) || (p_buffer->buffer_len > new_size))
    {
        log_debug("buffer resize invalid params\n");
        goto EXIT;
    }

    LPVOID lp_new_data = HeapReAlloc(GetProcessHeap(),
                                     HEAP_ZERO_MEMORY,
                                     p_buffer->lp_data,
                                     new_size * sizeof(BYTE));

    if (NULL == lp_new_data)
    {
        log_debug("HeapReAlloc failed\n");
        goto EXIT;
    }

    p_buffer->lp_data = (LPBYTE)lp_new_data; // Cast to LPBYTE
    p_buffer->buffer_len = new_size;
    retval               = TRUE;

EXIT:
    return retval;
}
